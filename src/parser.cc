/*
 * meteodata-git
 * Stanislaw Grams <sjg@fmdx.pl>
 *
 * src/parser.cc
 * See ../LICENSE for license information
 */
#include "../include/parser.hh"

parser_c::parser_c (
  agent_c *agent,
  station_c *station,
  data_c *data)
{
  int status = 0;
  if (!agent || !station || !data) {
    throw std::invalid_argument ("parameters set incorrectly");
  }
  this->station = station;
  this->data    = data;

  if (station->get_number () == 0) {
    status = this->match_station_number ();
    if (status < 0) {
      throw std::length_error ("failed to fetch data. Check connectivity");
    } else if (status > 0) {
      throw std::invalid_argument ("invalid name");
    }
  }

  status = this->agent->datafetcher_execute (*(this->station), this->data);
  if (status != 0) {
    throw new std::length_error ("failed to fetch data. Check connectivity");
  } else if (this->data->buffer.length () <= 0) {
    throw new std::length_error ("data too short");
  }

  this->json = json_c::parse (this->data->buffer);
}

int
parser_c::match_station_number () {
  int status = 0;
  std::string station_name = this->station->get_name ();
  if (station_name.length () <= 0) {
    throw std::invalid_argument ("name set incorrectly");
  }
  status = this->agent->idfetcher_execute (*this->station, this->data);
  if (status != 0) {
    return -1;
  }
  auto id_json = json_c::parse (this->data->buffer);

  for (auto it = id_json.begin (); it != id_json.end (); ++it)
  {
    if ((*it)["c"] == FLAVOUR_METEO_STRING) {
      std::string str_id   = (*it)["id"];
      std::string str_name = (*it)["n"];

      this->station->set_number (std::stoi(str_id));
      this->station->set_name (str_name);
      break;
    }
  }
  this->data->clear ();

  if (this->station->get_number () == 0) {
    return 1;
  }
  return 0;
}

// TODO: finish parsing for precip & wind types
std::string
parser_c::get_measurement (
  datatype_t  datatype,
  time_t      datetime
  )
{
  std::string      retval;
  json_c::iterator json_it;
  json_c           json_tmp;

  float tmp_min = 0;
  float tmp_max = 0;
  std::string tmp_date;
  this->json = json_c::parse (this->data->buffer);

  switch (datatype) {
    // precipitation
    case precip_cur:
      retval += this->json[PATH_PRECIP_STATE];
      break;
    case precip_10min:
      goto GET_UNIT_FOR_PRECIP;
    case precip_hourly:
      goto GET_UNIT_FOR_PRECIP;
    case precip_daily:
GET_UNIT_FOR_PRECIP:
      retval += "mm";
      break;

    // temperature
    case temp_auto:
      json_tmp = ((json_c)this->json[PATH_TEMP_AUTO_REC]);
      json_it  = json_tmp.end () - 1;
      retval   += (*json_it)["value"].dump ();
      goto GET_UNIT_FOR_TEMP;
    case temp_obs:
      json_tmp = ((json_c)this->json[PATH_TEMP_OBS_REC]);
      json_it  = json_tmp.end () - 1;
      retval   += (*json_it)["value"].dump ();
      goto GET_UNIT_FOR_TEMP;
    case temp_auto_min:
      json_tmp = ((json_c)this->json[PATH_TEMP_AUTO_REC]);
      json_it  = json_tmp.begin ();
      tmp_min  = std::atof ((*json_it)["value"].dump ().c_str ());
      tmp_date = (*json_it)["date"].dump();

      for (json_it = json_tmp.begin (); json_it < json_tmp.end (); ++json_it)
      {
        if (std::atof ((*json_it)["value"].dump ().c_str ()) <= tmp_min) {
          tmp_min  = std::atof ((*json_it)["value"].dump ().c_str ());
          tmp_date = (*json_it)["date"].dump();
        }
      }
      retval += std::to_string(std::round(tmp_min * 10) / 10);
      retval += UNIT_TEMPERATURE;
      retval += " at ";
      retval += tmp_date;
      break;
    case temp_auto_max:
      json_tmp = ((json_c)this->json[PATH_TEMP_AUTO_REC]);
      json_it  = json_tmp.begin ();
      tmp_max  = std::atof ((*json_it)["value"].dump ().c_str ());
      tmp_date = (*json_it)["date"].dump();

      for (json_it = json_tmp.begin (); json_it < json_tmp.end (); ++json_it)
      {
        if (std::atof ((*json_it)["value"].dump ().c_str ()) >= tmp_max) {
          tmp_max  = std::atof ((*json_it)["value"].dump ().c_str ());
          tmp_date = (*json_it)["date"].dump();
        }
      }
      retval += std::to_string(std::round(tmp_max * 10) / 10);
      retval += UNIT_TEMPERATURE;
      retval += " at ";
      retval += tmp_date;
      break;
    case temp_obs_min:
      goto GET_UNIT_FOR_TEMP;
    case temp_obs_max:
GET_UNIT_FOR_TEMP:
    retval += "°C";
    break;

    // wind
    case wind_dir_tel:
      goto GET_UNIT_FOR_WIND;
    case wind_dir_obs:
      goto GET_UNIT_FOR_WIND;
    case wind_vel_tel:
      goto GET_UNIT_FOR_WIND;
    case wind_vel_obs:
      goto GET_UNIT_FOR_WIND;
    case wind_vel_max:
      goto GET_UNIT_FOR_WIND;
    case wind_vel_tel_max:
      goto GET_UNIT_FOR_WIND;
    case wind_vel_obs_max:
GET_UNIT_FOR_WIND:
      retval += "m/s";
      break;

    // other
    case info:
      break;
  }

  return retval;
}

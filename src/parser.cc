/*
 * meteodata-git
 * Stanislaw Grams <sjg@fmdx.pl>
 *
 * src/parser.cc
 * See ../LICENSE for license information
 */
#include "../include/parser.hh"
#include <iostream>

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
    if (status != 0) {
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
  std::string station_name = this->station->get_name ();
  if (station_name.length () <= 0) {
    throw std::invalid_argument ("name set incorrectly");
  }
  this->agent->idfetcher_execute (*this->station, this->data);
  auto id_json = json_c::parse (this->data->buffer);

  std::string flavour_string;
  switch (this->station->get_flavour ())
  {
    case meteo:
    flavour_string = "meteo";
    break;
    case hydro:
    flavour_string = "hydro";
    break;
  }

  for (auto it = id_json.begin (); it != id_json.end (); ++it)
  {
    if ((*it)["c"] == flavour_string) {
      std::string str_id   = (*it)["id"];
      std::string str_name = (*it)["n"];

      this->station->set_number (std::stoi(str_id));
      this->station->set_name (str_name);
      break;
    }
  }
  this->data->clear ();

  if (this->station->get_number () == 0) {
    return -1;
  }
  return 0;
}

std::string
parser_c::get_measurement (
  datatype_t datatype
  )
{
  std::string retval;

  switch (datatype) {
    // precipitation
    case precip_cur:
    case precip_10min:
    case precip_hourly:
    case precip_daily:
    retval += "mm";
    break;

    // temperature
    case temp_auto:
    case temp_obs:
    case temp_auto_min:
    case temp_auto_max:
    case temp_obs_min:
    case temp_obs_max:
    retval += "°C";
    break;

    // wind
    case wind_dir_tel:
    case wind_dir_obs:
    case wind_vel_tel:
    case wind_vel_obs:
    case wind_vel_max:
    case wind_vel_tel_max:
    case wind_vel_obs_max:
    retval += "m/s";
    break;

    // water state
    case state_auto:
    case state_obs:
    retval += "mm";
    break;

    // water discharge
    case discharge_auto:
    case discharge_obs:
    retval += "m³/s";
    break;

    // other
    case info:
    break;
  }
  return retval;
}

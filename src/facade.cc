/*
 * meteodata-git
 * Stanislaw Grams <sjg@fmdx.pl>
 *
 * src/parser.cc
 * See ../LICENSE for license information
 */
#include "../include/facade.hh"

void
station_facade_c::set_strategy (flavour_t flavour) {
  if (flavour == meteo) {
    this->strategy = std::make_unique<meteo_strategy_c>(meteo_strategy_c (this->agent));
  } else if (flavour == hydro) {
    this->strategy = std::make_unique<hydro_strategy_c>(hydro_strategy_c (this->agent));
  }
}

void
station_facade_c::find_station (std::string name) {
  if (station->get_number () == 0) {
    this->set_strategy (station->get_flavour ());
    try {
      std::pair<std::string, uint64_t> info = strategy->find_station (name);
      station->set_name (info.first);
      station->set_number (info.second);
    } catch (...) {
      throw std::invalid_argument ("unable to match ID to a requested station!, double-check your inputs!");
    }
  }
}

void
station_facade_c::fill_station_data () {
  this->strategy->fill_station_data (this->station, &this->json, this->agent, this->data);
}

station_c *
station_facade_c::get_station () {
  return this->station;
}

void
meteo_strategy_c::converter (station_c *station, json_c *json) {
  json_c       json_tmp   = *json;
  meteo_data_t meteo_data;

  // precipitation state parser
  json_c json_tmp_precip = json_tmp[PATH_PRECIP_STATE];
  for (const auto &json_it : json_tmp_precip)
  {
    meteo_data.precip_state = false;
    if (std::string (json_it.dump ()).compare (std::string ("\"no-precip\""))) {
      meteo_data.precip_state = true;
    }
    break;
  }

  // precipitation (10min) parser
  json_c json_tmp_precip_10min = json_tmp[PATH_PRECIP_10MIN];
  for (const auto &json_it : json_tmp_precip_10min)
  {
    std::pair<std::tm, float> data;

    std::istringstream (json_it["date"].dump ()) >> std::get_time (&data.first, "\"%Y-%m-%dT%H:%M:%SZ\"");
    data.second = std::atof (json_it["value"].dump ().c_str ());

    meteo_data.precip_10min.push_back (data);
  }

  // precipitation (hourly) parser
  json_c json_tmp_precip_hourly = json_tmp[PATH_PRECIP_HOURLY];
  for (const auto &json_it : json_tmp_precip_hourly)
  {
    std::pair<std::tm, float> data;

    std::istringstream (json_it["date"].dump ()) >> std::get_time (&data.first, "\"%Y-%m-%dT%H:%M:%SZ\"");
    data.second = std::atof (json_it["value"].dump ().c_str ());

    meteo_data.precip_hourly.push_back (data);
  }

  // precipitation (daily) parser
  json_c json_tmp_precip_daily = json_tmp[PATH_PRECIP_DAILY];
  for (const auto &json_it : json_tmp_precip_daily)
  {
    std::pair<std::tm, float> data;

    std::istringstream (json_it["date"].dump ()) >> std::get_time (&data.first, "\"%Y-%m-%dT%H:%M:%SZ\"");
    data.second = std::atof (json_it["value"].dump ().c_str ());

    meteo_data.precip_daily.push_back (data);
  }

  // temperature parser
  json_c json_tmp_temp = json_tmp[PATH_TEMP_AUTO_REC];
  for (const auto &json_it : json_tmp_temp)
  {
    std::pair<std::tm, float> data;

    std::istringstream (json_it["date"].dump ()) >> std::get_time (&data.first, "\"%Y-%m-%dT%H:%M:%SZ\"");
    data.second = std::atof (json_it["value"].dump ().c_str ());

    meteo_data.temp.push_back (data);
  }

  // wind velocity
  json_c json_tmp_wind_vel = json_tmp[PATH_WIND_VEL_TEL_REC];
  for (const auto &json_it : json_tmp_wind_vel)
  {
    std::pair<std::tm, float> data;

    std::istringstream (json_it["date"].dump ()) >> std::get_time (&data.first, "\"%Y-%m-%dT%H:%M:%SZ\"");
    data.second = std::atof (json_it["value"].dump ().c_str ());

    meteo_data.wind_vel.push_back (data);
  }

  // wind velocity (10min)
  json_c json_tmp_wind_vel_10min = json_tmp[PATH_WIND_VEL_10MIN_MAX_REC];
  for (const auto &json_it : json_tmp_wind_vel_10min)
  {
    std::pair<std::tm, float> data;

    std::istringstream (json_it["date"].dump ()) >> std::get_time (&data.first, "\"%Y-%m-%dT%H:%M:%SZ\"");
    data.second = std::atof (json_it["value"].dump ().c_str ());

    meteo_data.wind_vel_10min.push_back (data);
  }

  // wind direction
  json_c json_tmp_wind_dir = json_tmp[PATH_WIND_DIR_TEL_REC];
  for (const auto &json_it : json_tmp_wind_dir)
  {
    std::pair<std::tm, float> data;

    std::istringstream (json_it["date"].dump ()) >> std::get_time (&data.first, "\"%Y-%m-%dT%H:%M:%SZ\"");
    data.second = std::atof (json_it["value"].dump ().c_str ());

    meteo_data.wind_dir.push_back (data);
  }

  // save parsed data
  ((meteo_station_c *)station)->set_data (meteo_data);
}

void
hydro_strategy_c::converter (station_c *station, json_c *json) {
  json_c       json_tmp = *json;
  hydro_data_t hydro_data;

  // water level parser
  json_c json_tmp_level = json_tmp[PATH_WATER_LEVEL];

  for (const auto &json_it : json_tmp_level)
  {
    std::pair<std::tm, float> data;

    std::istringstream (json_it["date"].dump ()) >> std::get_time (&data.first, "\"%Y-%m-%dT%H:%M:%SZ\"");
    data.second = std::atof (json_it["value"].dump ().c_str ());

    hydro_data.level.push_back (data);
  }

  // discharge level parser
  json_c json_tmp_discharge = json_tmp[PATH_WATER_DISCHARGE];

  for (const auto &json_it : json_tmp_discharge)
  {
    std::pair<std::tm, float> data;

    std::istringstream (json_it["date"].dump ()) >> std::get_time (&data.first, "\"%Y-%m-%dT%H:%M:%SZ\"");
    data.second = std::atof (json_it["value"].dump ().c_str ());

    hydro_data.discharge.push_back (data);
  }

  // save parsed data
  ((hydro_station_c *)station)->set_data (hydro_data);
}

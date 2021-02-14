/*
 * meteodata-git
 * Stanislaw Grams <sjg@fmdx.pl>
 *
 * src/station.cc
 * See ../LICENSE for license information
 */
#include "../include/station.hh"

std::string
station_c::get_name (
  )
{
  return this->name;
}

void
station_c::set_name (
  std::string name
  )
{
  if (name.empty ()) {
    throw std::invalid_argument ("name was set incorrectly");
    return;
  }
  this->name = name;
  return;
}

std::string
station_c::get_province (
  )
{
  return this->province;
}

void
station_c::set_province (
  std::string province
  )
{
  if (province.empty ()) {
    throw std::invalid_argument ("province was set incorrectly");
    return;
  }
  this->province = province;
  return;
}

std::string
station_c::get_river (
  )
{
  return this->river;
}

void
station_c::set_river (
  std::string river
  )
{
  if (river.empty ()) {
    throw std::invalid_argument ("river was set incorrectly");
    return;
  }
  this->river = river;
  return;
}

uint64_t
station_c::get_number (
  )
{
  return this->number;
}

void
station_c::set_number (
  uint64_t number
  )
{
  if (number > 0) {
    this->number = number;
  }
}

flavour_t
station_c::get_flavour (
  )
{
  return this->flavour;
}


std::string
meteo_station_c::get_presentation (
  )
{
  return std::string ("meteo presentation!");
}

void
meteo_station_c::set_data (
  meteo_data_t &meteo_data
  )
{
  this->meteo_data = meteo_data;
}

measurement_temp_celsius_c
meteo_station_c::get_cur_temp () {
  measurement_temp_celsius_c rv;
  if (this->meteo_data.temp.size () > 0) {
    rv.set_value (this->meteo_data.temp.back().second);
  } else {
    throw "temperature data unavailable";
  }
  return rv;
}

measurement_temp_kelvin_c
meteo_station_c::get_cur_temp_kelvin () {
  measurement_temp_kelvin_c rv;
  if (this->meteo_data.temp.size () > 0) {
    rv.set_value (this->meteo_data.temp.back().second);
  } else {
    throw "temperature data unavailable";
  }
  return rv;
}

measurement_temp_fahrenheit_c
meteo_station_c::get_cur_temp_fahrenheit () {
  measurement_temp_fahrenheit_c rv;
  if (this->meteo_data.temp.size () > 0) {
    rv.set_value (this->meteo_data.temp.back().second);
  } else {
    throw "temperature data unavailable";
  }
  return rv;
}

measurement_level_c
hydro_station_c::get_cur_level () {
  measurement_level_c rv;
  if (this->hydro_data.level.size () > 0) {
    rv.set_value (this->hydro_data.level.back ().second);
  } else {
    throw "level data unavailable";
  }
  return rv;
}

measurement_discharge_c
hydro_station_c::get_cur_discharge () {
  measurement_discharge_c rv;
  if (this->hydro_data.discharge.size () > 0) {
    rv.set_value (this->hydro_data.discharge.back ().second);
  } else {
    throw "discharge data unavailable";
  }
  return rv;
}

std::string
hydro_station_c::get_presentation (
  )
{
  return std::string ("hydro presentation!");
}

void
hydro_station_c::set_data (
  hydro_data_t &hydro_data
  )
{
  this->hydro_data = hydro_data;
}

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

measurement_impl_c
hydro_station_c::get_cur_level () {
  measurement_impl_c rv;
  if (this->hydro_data.level.size () > 0) {
    rv.set_value (this->hydro_data.level.back ().second);
  } else {
    throw "level data unavailable";
  }
  return rv;
}

void
hydro_station_c::set_data (
  hydro_data_t &hydro_data
  )
{
  this->hydro_data = hydro_data;
}

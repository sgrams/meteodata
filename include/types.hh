/*
 * meteodata-git
 * Stanislaw Grams <sjg@fmdx.pl>
 *
 * src/types.hh
 * See ../LICENSE for license information
 */
#ifndef METEODATA_TYPES_H
#define METEODATA_TYPES_H
#include <chrono>
#include <vector>
#include <algorithm>

typedef enum command {
  verbose  = 0x01,
  location = 0x02,
  date     = 0x03,
  type     = 0x04,
} command_t;

typedef enum flavour {
  meteo,
  hydro
} flavour_t;

typedef enum meteo_datatype {
  // precipitation
  precip_state,
  precip_10min,
  precip_hourly,
  precip_daily,

  // temperature
  temp,
  temp_kelvin,
  temp_fahrenheit,

  temp_min,
  temp_min_kelvin,
  temp_min_fahrenheit,

  temp_max,
  temp_max_kelvin,
  temp_max_fahrenheit,

  // wind velocity
  wind_vel,
  wind_min_vel,
  wind_max_vel,

  // wind velocity (10min max)
  wind_vel_10min_max,
  wind_vel_min_10min_max,
  wind_vel_max_10min_max,

  // wind direction
  wind_dir
} meteo_datatype_t;

typedef enum hydro_datatype {
  // water level
  level,

  // water discharge
  discharge
} hydro_datatype_t;

typedef struct meteo_data {
  // precipitation
  bool precip_state;

  std::vector<std::pair<std::tm, float>> precip_10min;
  std::vector<std::pair<std::tm, float>> precip_hourly;
  std::vector<std::pair<std::tm, float>> precip_daily;

  // temperature
  std::vector<std::pair<std::tm, float>> temp;

  // wind velocity
  std::vector<std::pair<std::tm, float>> wind_vel;

  // wind velocity (10min max)
  std::vector<std::pair<std::tm, float>> wind_vel_10min;

  // wind direction
  std::vector<std::pair<std::tm, int>> wind_dir;
} meteo_data_t;

typedef struct hydro_data {
  // water level
  std::vector<std::pair<std::tm, float>> level;

  // water discharge
  std::vector<std::pair<std::tm, float>> discharge;
} hydro_data_t;
#endif // METEODATA_TYPES_H

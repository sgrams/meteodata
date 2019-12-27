/*
 * meteodata-git
 * Stanislaw Grams <sjg@fmdx.pl>
 *
 * src/types.hh
 * See ../LICENSE for license information
 */
#ifndef METEODATA_TYPES_H
#define METEODATA_TYPES_H

typedef enum datatype {
  // general
  info             = 0x00,

  // precipitation
  precip_cur       = 0x10,
  precip_10min     = 0x11,
  precip_hourly    = 0x12,
  precip_daily     = 0x13,

  // temperature
  temp_auto        = 0x20,
  temp_obs         = 0x21,

  temp_auto_min    = 0x22,
  temp_auto_max    = 0x23,
  temp_obs_min     = 0x24,
  temp_obs_max     = 0x25,

  // wind
  wind_dir_tel     = 0x30,
  wind_dir_obs     = 0x31,
  wind_vel_tel     = 0x32,
  wind_vel_obs     = 0x33,
  wind_vel_max     = 0x34,

  wind_vel_tel_max = 0x35,
  wind_vel_obs_max = 0x36,
} datatype_t;

typedef enum command {
  verbose  = 0x01,
  location = 0x02,
  date     = 0x03,
  type     = 0x04,
  //json     = 0x05
} command_t;
#endif // METEODATA_TYPES_H

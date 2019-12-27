/*
 * meteodata-git
 * Stanislaw Grams <sjg@fmdx.pl>
 *
 * src/commandparser.cc
 * See ../LICENSE for license information
 */
#include "../include/commandparser.hh"

commandparser_c::commandparser_c (
  int argc,
  char *argv[]
  )
{
  this->argc = argc;
  this->argv = argv;
}

char **
commandparser_c::get_argv (
  void
  )
{
  return this->argv;
}

// validation shall be handled externally
int
commandparser_c::get_commands (
  std::vector<std::pair<command_t, std::string>> &command_vec
  )
{
  int  status            = 0;
  int  option_index      = 0;
  bool location_provided = false;
  char c;

  static struct option long_options[] =
  {
    {"help",     no_argument,       0, 'h'},
    {"verbose",  no_argument,       0, 'v'},
    {"date",     required_argument, 0, 'd'},
    {"location", required_argument, 0, 'l'},
    {"type",     required_argument, 0, 't'},
    {0, 0, 0, 0},
  };

  if (argc == 1) {
    status = 1; // no commands provided
    return status;
  }

  while ((c = getopt_long (this->argc, this->get_argv (), "hvl:d:t:", long_options, &option_index)) != -1)
  {
    switch (c) {
      // help
      case 'h':
      return status = 1; // help

      // verbose
      case 'v':
      command_vec.push_back(std::make_pair(verbose, ""));
      break;

      // location
      case 'l':
      location_provided = true;
      command_vec.push_back (std::make_pair (location, std::string (optarg)));
			break;

      // datetime
      case 'd':
      command_vec.push_back (std::make_pair (date, std::string (optarg)));
      break;

      // datatype
      case 't':
      command_vec.push_back (std::make_pair (type, std::string (optarg)));
      break;
    }
  }

  if (!location_provided) {
    return 2;
  }
  return status;
}

void
commandparser_c::get_enum_map (
  std::unordered_map <std::string, datatype_t> &enum_map
  )
{
  // general
  enum_map["info"]             = info;
  // precipitation
  enum_map["precip.cur"]       = precip_cur;
  enum_map["precip.10min"]     = precip_10min;
  enum_map["precip.hourly"]    = precip_hourly;
  enum_map["precip.daily"]     = precip_daily;

  // temperature
  enum_map["temp.auto"]        = temp_auto;
  enum_map["temp.obs"]         = temp_obs;
  enum_map["temp.auto.min"]    = temp_auto_min;
  enum_map["temp.auto.max"]    = temp_auto_max;
  enum_map["temp.obs.min"]     = temp_obs_min;
  enum_map["temp.obs.max"]     = temp_obs_max;

  // wind
  enum_map["wind.dir.tel"]     = wind_dir_tel;
  enum_map["wind.dir.obs"]     = wind_dir_obs;
  enum_map["wind.vel.tel"]     = wind_vel_tel;
  enum_map["wind.vel.obs"]     = wind_vel_obs;
  enum_map["wind.vel.max"]     = wind_vel_max;
  enum_map["wind.vel.tel.max"] = wind_vel_tel_max;
  enum_map["wind.vel.obs.max"] = wind_vel_obs_max;
}

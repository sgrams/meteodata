/*
 * meteodata-git
 * Stanislaw Grams <sjg@fmdx.pl>
 *
 * src/helper.cc
 * See ../LICENSE for license information
 */
#include "../include/helper.hh"
#include "../include/types.hh"

std::string
helper_c::extract_str_from_meteo_enum_map (
  std::unordered_map <std::string, meteo_datatype_t> &enum_map,
  meteo_datatype_t &value
  )
{
  auto const it = std::find_if (enum_map.begin (), enum_map.end (),
    [&value](const std::pair<std::string, meteo_datatype_t> &p) {
      return p.second == value;
    });

  if (it == enum_map.end ()) {
    return std::string ("");
  }

  return std::string (it->first);
}

std::string
helper_c::extract_str_from_hydro_enum_map (
  std::unordered_map <std::string, hydro_datatype_t> &enum_map,
  hydro_datatype_t &value
  )
{
  auto const it = std::find_if (enum_map.begin (), enum_map.end (),
    [&value](const std::pair<std::string, hydro_datatype_t> &p) {
      return p.second == value;
    });

  if (it == enum_map.end ()) {
    return std::string ("");
  }

  return std::string (it->first);
}

void
helper_c::print_help (
  void
  )
{
  std::cout \
    << METEODATA_NAME << " " \
    << METEODATA_VERSION << ": Stanislaw Grams <sjg@fmdx.pl>\n" \
    << "Usage: meteodata <options>\n" \
    << "  -h  --help     print usage\n" \
    << "  -v  --verbose  set verbose mode\n" \
    << "  -l  --location set location [REQUIRED]\n" \
    << "  -t  --type     set datatype [REQUIRED]\n" \

    << "List of available datatypes:\n" \
    << "  - temp  (temperature per 10min, 24h)\n" \
    << "    temp_fahrenheit\n" \
    << "    temp_kelvin\n" \
    << "  - level (water level per 1hr, 3 days)\n" \
    << "Datetime presented in UTC.\n" \
    << "Example call:\n" \
    << "$ meteodata -l Warszawa -t temp\n" \
    << "Zrodlem pochodzenia danych jest Instytut Meteorologii i Gospodarki Wodnej - Panstwowy Instytut Badawczy\n";
}

void
helper_c::verbose (
  std::string msg
  )
{
  if (this->verbose_flag) {
    std::cout << "verbose: " << msg << "\n";
  }
}

void
helper_c::error (
  std::string msg
  )
{
  std::cerr << "error: " << msg << "\n";
}

void
helper_c::message (
  std::string msg
  )
{
  std::cout << msg << "\n";
}

void
helper_c::flip_verbose () {
  this->verbose_flag = !this->verbose_flag;
}

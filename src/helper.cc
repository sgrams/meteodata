/*
 * meteodata-git
 * Stanislaw Grams <sjg@fmdx.pl>
 *
 * src/helper.cc
 * See ../LICENSE for license information
 */
#include "../include/helper.hh"

void
helper_ns::print_help (
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
    << "  -d  --date     set date\n" \
    << "  -t  --type     set datatype\n" \

    << "List of available datatypes: [data for last ... min]\n" \
    << "  info [n/a]; general information\n" \
    << "  precipitation:\n"
    << "    precip.cur    [0     min]; telemetry, current precipitation\n" \
    << "    precip.10min  [60    min]; telemetry, precipitation for last 10 minutes\n" \
    << "    precip.hourly [2880  min]; telemetry, precipitation for last hour\n" \
    << "    precip.daily  [10080 min]; telemetry, precipitation for last 24 hours\n" \
    << "  temperature:\n" \
    << "    temp.auto     [2880 min]; telemetry, temperature\n" \
    << "    temp.obs      [2880 min]; observer, temperature\n" \
    << "    temp.auto.min [2880 min]; telemetry, min temperature\n" \
    << "    temp.auto.max [2880 min]; telemetry, max temperature\n" \
    << "    temp.obs.min  [2880 min]; observer, min temperature\n" \
    << "    temp.obs.max  [2880 min]; observer, max temperature\n" \
    << "  wind:\n" \
    << "    wind.dir.tel     [2880 min]; telemetry, wind direction per 10 minutes\n" \
    << "    wind.dir.obs     [2880 min]; observer, wind direction per 1 hour\n" \
    << "    wind.vel.tel     [2880 min]; telemetry, wind velocity per 10 minutes\n"\
    << "    wind.vel.obs     [2880 min]; observer, wind velocity per 1 hour\n"\
    << "    wind.vel.max     [2880 min]; telemetry, max wind gusts velocity per 10 minutes\n"\
    << "    wind.vel.tel.max [2880 min]; telemetry, max avg wind velocity per 10 minutes\n" \
    << "    wind.vel.obs.max [2880 min]; observer, max avg wind velocity per 10 minutes\n" \
    << "\n" \
    << "Datetime presented in UTC.\n" \
    << "Example call:\n" \
    << "$ meteodata -l Warszawa -t temp.auto -d 20:00\n" \
    << "Zrodlem pochodzenia danych jest Instytut Meteorologii i Gospodarki Wodnej - Panstwowy Instytut Badawczy\n";
}

void
helper_ns::verbose (
  std::string msg,
  bool verbose_flag
  )
{
  if (verbose_flag) {
    std::cout << "verbose: " << msg << "\n";
  }
}

void
helper_ns::error (
  std::string msg
  )
{
  std::cerr << "error: " << msg << "\n";
}

void
helper_ns::message (
  std::string msg
  )
{
  std::cout << msg << "\n";
}

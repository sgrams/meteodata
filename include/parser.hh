/*
 * meteodata-git
 * Stanislaw Grams <sjg@fmdx.pl>
 *
 * src/parser.hh
 * See ../LICENSE for license information
 */
#ifndef METEODATA_PARSER_H
#define METEODATA_PARSER_H
#include <stdexcept>
#include <cstdint>
#include <map>
#include <string>
#include <nlohmann/json.hpp>
#include "types.hh"
#include "agent.hh"
#include "data.hh"
#include "station.hh"

using json_c = nlohmann::json;

#define UNIT_PRECIPITATION   "mm"
#define UNIT_TEMPERATURE     "°C"
#define UNIT_WIND            "m/s"
#define UNIT_WATER_STATE     "cm"
#define UNIT_WATER_DISCHARGE "m³/s"
#define UNIT_EMPTY           ""

class parser_c {
  private:
  agent_c                           *agent;
  station_c                         *station;
  data_c                            *data;
  json_c                            json;

  // private methods
  int  match_station_number ();

  public:
  // constructors
  parser_c (agent_c *agent, station_c *station, data_c *data);

  // methods
  std::string get_measurement (datatype_t datatype);
};
#endif // METEODATA_PARSER_H

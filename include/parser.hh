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
#include <ctime>
#include <nlohmann/json.hpp>
#include "types.hh"
#include "agent.hh"
#include "data.hh"
#include "station.hh"

using json_c = nlohmann::json;

#define PATH_PRECIP_STATE     "state"
#define PATH_TEMP_AUTO_REC    "temperatureAutoRecords"
#define PATH_TEMP_OBS_REC     "temperatureObsRecords"
#define PATH_WIND_DIR_TEL_REC "windDirectionTelRecords"
#define PATH_WIND_DIR_OBS_REC "windDirectionObsRecords"
#define PATH_WIND_VEL_TEL_REC "windVelocityTelRecords"
#define PATH_WIND_VEL_OBS_REC "windVelocityObsRecords"
#define PATH_WIND_VEL_MAX_REC "windMaxVelocityRecords"


#define UNIT_PRECIPITATION   "mm"
#define UNIT_TEMPERATURE     "°C"
#define UNIT_WIND            "m/s"
#define UNIT_WATER_STATE     "cm"
#define UNIT_WATER_DISCHARGE "m³/s"
#define UNIT_EMPTY           ""

#define FLAVOUR_METEO_STRING "meteo"

class parser_c {
  private:
  agent_c   *agent;
  station_c *station;
  data_c    *data;
  json_c    json;

  // private methods
  int  match_station_number ();

  public:
  // constructors
  parser_c (agent_c *agent, station_c *station, data_c *data);

  // methods
  std::string get_measurement (datatype_t datatype, time_t datetime);
};
#endif // METEODATA_PARSER_H

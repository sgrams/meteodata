/*
 * meteodata-git
 * Stanislaw Grams <sjg@fmdx.pl>
 *
 * src/facade.hh
 * See ../LICENSE for license information
 */
#ifndef METEODATA_FACADE_H
#define METEODATA_FACADE_H
#include <stdexcept>
#include <cstdint>
#include <map>
#include <string>
#include <ctime>
#include <nlohmann/json.hpp>
#include <memory>
#include <iostream>
#include "types.hh"
#include "agent.hh"
#include "data.hh"
#include "station.hh"

using json_c = nlohmann::json;

//
// meteo paths
//
// description
#define PATH_NAME     "name"
#define PATH_PROVINCE "province"
#define PATH_RIVER    "river"

// precipitation
#define PATH_PRECIP_STATE     "state"

#define PATH_PRECIP_10MIN     "tenMinutesPrecipRecords"
#define PATH_PRECIP_HOURLY    "hourlyPrecipRecords"
#define PATH_PRECIP_DAILY     "dailyPrecipRecords"

// temperature
#define PATH_TEMP_AUTO_REC    "temperatureAutoRecords"
#define PATH_TEMP_OBS_REC     "temperatureObsRecords"

// wind velocity
#define PATH_WIND_VEL_TEL_REC "windVelocityTelRecords"
#define PATH_WIND_VEL_OBS_REC "windVelocityObsRecords"

// wind velocity (10min max)
#define PATH_WIND_VEL_10MIN_MAX_REC "windMaxVelocityRecords"

// wind direction
#define PATH_WIND_DIR_TEL_REC "windDirectionTelRecords"
#define PATH_WIND_DIR_OBS_REC "windDirectionObsRecords"

//
// hydro paths
//
#define PATH_WATER_LEVEL      "waterStateRecords"
#define PATH_WATER_DISCHARGE  "dischargeRecords"

//
// units
//
#define UNIT_PRECIPITATION       "mm"
#define UNIT_TEMPERATURE_CELSIUS "°C"
#define UNIT_WIND                "m/s"
#define UNIT_WATER_LEVEL         "cm"
#define UNIT_WATER_DISCHARGE     "m³/s"
#define UNIT_EMPTY               ""

//
// flavours
//
#define FLAVOUR_METEO_STRING "meteo"
#define FLAVOUR_HYDRO_STRING "hydro"


class strategy_c {
  protected:
    std::string name;
    agent_c     *agent{nullptr};
    uint64_t    id;

  public:
    strategy_c (agent_c *agent) : agent(agent) {};
    virtual ~strategy_c () {};

    std::pair<std::string, uint64_t> find_station (std::string name) {
      data_c   data;
      int      status = 0;
      std::pair <std::string, uint64_t> info;

      if (name.length () <= 0) {
        throw std::invalid_argument ("name set incorrectly");
      }

      status = this->agent->idfetcher_execute (name, &data);
      if (status != 0) {
        throw new std::invalid_argument ("could not find such a station!");
      }
      auto id_json = json_c::parse (data.buffer);

      for (auto it = id_json.begin (); it != id_json.end (); ++it)
      {
        if ((*it)["c"] == get_flavour_string ()) {
          std::string str_id   = (*it)["id"];
          std::string str_name = (*it)["n"];
          info.first  = str_name;
          info.second = std::stoi(str_id);
          break;
        }
      }

      data.clear ();
      return info;
    };

    void fill_station_data (station_c *station, json_c *json, agent_c *agent, data_c *data) {
      int status = agent->datafetcher_execute (station->get_number (), data, station->get_flavour ());
      if (status != 0) {
        throw new std::length_error ("failed to fetch data. Check connectivity");
      } else if (data->buffer.length () <= 0) {
        throw new std::length_error ("data too short");
      }

      try {
        *json = json_c::parse (data->buffer);
      } catch (...) {
        throw new std::runtime_error ("unable to parse incoming json data - the source may be broken");
      }
      converter (station, json);
    };

  private:
    virtual std::string get_flavour_string () = 0;
    virtual void converter (station_c *station, json_c *json) = 0;
};

class meteo_strategy_c : public strategy_c {
  public:
    meteo_strategy_c (agent_c *agent):
      strategy_c (agent) {};

  private:
    std::string get_flavour_string () {
      return std::string (FLAVOUR_METEO_STRING);
    };

    void converter (station_c *station, json_c *json) override;
};

class hydro_strategy_c : public strategy_c {
  public:
    hydro_strategy_c (agent_c *agent):
      strategy_c (agent) {};

  private:
    std::string get_flavour_string () {
      return std::string (FLAVOUR_HYDRO_STRING);
    };

    void converter (station_c *station, json_c *json) override;
};

class station_facade_c {
  private:
    agent_c   *agent;
    station_c *station;
    data_c    *data;
    json_c    json;
    std::unique_ptr<strategy_c> strategy;

    // private methods
    void set_strategy (flavour_t flavour);

  public:
    // constructors
    station_facade_c (
        agent_c *agent = nullptr,
        station_c *station = nullptr,
        data_c *data = nullptr,
        flavour_t flavour = meteo) {
      this->agent = agent ? agent : agent_c::fetch_instance (); // free mem outside!
      if (flavour == meteo) {
        this->station = station ? station : new meteo_station_c ();
      } else {
        this->station = station ? station : new hydro_station_c ();
      }
      this->data = data ? data : new data_c ();
    };

    ~station_facade_c () {
      delete station;
      delete data;
    }

    // facade methods
    void find_station (std::string name); // calls method from subsystem C
    void fill_station_data (); // calls method from subsystem A and subsystem B
    station_c * get_station ();

};
#endif // METEODATA_FACADE_H

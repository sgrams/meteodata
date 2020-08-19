/*
 * meteodata-git
 * Stanislaw Grams <sjg@fmdx.pl>
 *
 * src/main.cc
 * See ../LICENSE for license information
 */
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

#include "../include/agent.hh"
#include "../include/data.hh"
#include "../include/helper.hh"
#include "../include/station.hh"
#include "../include/facade.hh"
#include "../include/commandparser.hh"
#include "../include/types.hh"

int
main (
  int argc,
  char *argv[]
  )
{
  int             status          = 0;
  bool            datetime_issued = false;
  agent_c         *agent          = nullptr;
  commandparser_c commparser      = commandparser_c (argc, argv);
  helper_c        helper          = helper_c ();

  std::vector<std::pair<command_t, std::string>>    cmd_vec;
  std::vector<std::string>                          location_vec;
  std::vector<meteo_datatype_t>                     meteo_datatype_vec;
  std::vector<hydro_datatype_t>                     hydro_datatype_vec;

  std::unordered_map<std::string, meteo_datatype_t> meteo_enum_map;
  std::unordered_map<std::string, hydro_datatype_t> hydro_enum_map;

  status = commparser.get_commands (cmd_vec);
  if (status < 0) {
    helper.error ("please double check command-line parameters");
    return status;
  } else if (status > 0) {
    helper.print_help ();
    return status;
  }

  commparser.get_meteo_enum_map (meteo_enum_map);
  commparser.get_hydro_enum_map (hydro_enum_map);

  agent = agent_c::fetch_instance ();

  std::vector<std::pair<command_t, std::string>>::iterator cmd_iter;
  for (auto &it: cmd_vec)
  {
    switch (it.first) {
      case verbose:
        helper.flip_verbose ();
        helper.verbose ("verbose mode enabled");
        break;
      case location:
        helper.verbose (it.second + " location selected");
        location_vec.push_back (it.second);
        break;
      case date:
        if (datetime_issued != true) {
          // TODO: enable datetime issuing support
          //datetime_issued = true;
          //datetime = it.second;
        }
        break;
      case type:
        std::unordered_map<std::string, meteo_datatype_t>::iterator meteo_enum_map_it;
        std::unordered_map<std::string, hydro_datatype_t>::iterator hydro_enum_map_it;

        if ((meteo_enum_map_it = meteo_enum_map.find (it.second)) != meteo_enum_map.end ()) {
          meteo_datatype_vec.push_back (meteo_enum_map_it->second);
        } else if ((hydro_enum_map_it = hydro_enum_map.find (it.second)) != hydro_enum_map.end ()) {
          hydro_datatype_vec.push_back (hydro_enum_map_it->second);
        } else {
          helper.error (it.second + " is NOT a valid datatype!");
        }

        break;
    }
  }

  if (meteo_datatype_vec.size () == 0 &&
      hydro_datatype_vec.size () == 0) {
    helper.error ("no datatype provided!");
  }

  station_facade_c *sf = nullptr;
  for (auto &location: location_vec)
  {
    if (meteo_datatype_vec.size () != 0) {
      // try iterating over all items in METEO vector
      sf = new station_facade_c (agent, nullptr, nullptr, meteo);

      try {
        sf->find_station (location);
        sf->fill_station_data ();

        for (auto &datatype: meteo_datatype_vec)
        {
          helper.verbose (
            "Printing data for station " +
            sf->get_station ()->get_name () +
            ", id = " +
            std::to_string (sf->get_station ()->get_number ()) +
            ", datatype = " +
            helper.extract_str_from_meteo_enum_map (meteo_enum_map, datatype)
            );

          meteo_station_c *station = (meteo_station_c *)sf->get_station();
          switch (datatype) {
            case temp:
              helper.message (station->get_cur_temp ().to_string ());
              break;
            case temp_kelvin:
              helper.message (station->get_cur_temp_kelvin ().to_string ());
              break;
            case temp_fahrenheit:
              helper.message (station->get_cur_temp_fahrenheit ().to_string ());
              break;
          }
        }
      } catch (...) {
        helper.error ("unable to prepare meteo measurements for " + location);
      }

      delete sf;
    }
    if (hydro_datatype_vec.size () != 0) {
      // try iterating over all items in HYDRO vector
      sf = new station_facade_c (agent, nullptr, nullptr, hydro);

      try {
        sf->find_station (location);
        sf->fill_station_data ();
        for (auto &datatype: hydro_datatype_vec)
        {
          helper.verbose (
            "Parsing data for station " +
            sf->get_station ()->get_name () +
            ", id = " +
            std::to_string (sf->get_station ()->get_number ()) +
            ", datatype = " +
            helper.extract_str_from_hydro_enum_map (hydro_enum_map, datatype)
            );
          hydro_station_c *station = (hydro_station_c *)sf->get_station ();
          // TODO: replace switch below with an actual function helper (receiving datatype_t)
          switch (datatype) {
            case level:
              helper.message (station->get_cur_level ().to_string ());
              break;
          }
        }
      } catch (...) {
        helper.error ("unable to prepare hydro measurements for " + location);
      }

      delete sf;
    }
  }
  agent_c::delete_instance ();

  return status;
}

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
#include "../include/parser.hh"
#include "../include/commandparser.hh"

int
main (
  int argc,
  char *argv[]
  )
{
  int             status          = 0;
  bool            verbose_flag    = false;
  bool            datetime_issued = false;
  time_t          datetime        = 0;
  commandparser_c commparser      = commandparser_c (argc, argv);
  agent_c         agent;

  std::vector<std::pair<command_t, std::string>> command_vec;
  std::vector<std::string>                       location_vec;
  std::vector<datatype_t>                        datatype_vec;

  status = commparser.get_commands (command_vec);
  if (status < 0) {
    helper_ns::error ("please double check command-line parameters");
    return status;
  } else if (status > 0) {
    helper_ns::print_help ();
    return status;
  }

  for (auto const &it: command_vec)
  {
    switch (it.first) {
      case verbose:
        verbose_flag = true;
        helper_ns::verbose ("verbose mode enabled", verbose_flag);
        break;
      case location:
        helper_ns::verbose (it.second + " location selected", verbose_flag);
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
        std::unordered_map<std::string, datatype_t>           enum_map;
        std::unordered_map<std::string, datatype_t>::iterator enum_map_it;
        commparser.get_enum_map (enum_map);

        if ((enum_map_it = enum_map.find (it.second)) == enum_map.end ()) {
          helper_ns::error (it.second + " invalid datatype!");
        } else {
          datatype_vec.push_back (enum_map_it->second);
        }
        break;
    }
  }

  if (datatype_vec.size () == 0) {
    helper_ns::error ("no datatype provided!");
  }

  for (auto const &loc_it: location_vec)
  {
    parser_c  *parser = nullptr;
    station_c station;
    data_c    data;
    station.set_name (loc_it);

    try {
      parser = new parser_c (&agent, &station, &data);
      for (auto const &dty_it: datatype_vec)
      {
        try {
          helper_ns::verbose (
              "Parsing data for station " +
              station.get_name () +
              ", id = " +
              std::to_string (station.get_number ()) +
              ", datatype = " +
              std::to_string (dty_it),
              verbose_flag
              );
          helper_ns::message (parser->get_measurement (dty_it, datetime));
        } catch (const std::exception& e) {
          helper_ns::error (std::string ("Unable to get measurement: ") + e.what ());
        }
      }
      if (parser != nullptr) {
        delete parser;
      }
    } catch (const std::exception& e) {
      helper_ns::error (std::string ("Unable to create parser for ") + station.get_name () + ": " + e.what ());
    }
  }

  return status;
}

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


void
handle_meteo_datatypes (
  std::vector<meteo_datatype_t>                     &meteo_datatype_vec,
  std::unordered_map<std::string, meteo_datatype_t> &meteo_enum_map,
  helper_c                                          &helper,
  agent_c                                           *agent,
  std::string                                       location
  )
{
  station_facade_c *sf = nullptr;
  std::string      datatype_parsed;

  if (meteo_datatype_vec.size () == 0) {
    return;
  }

  // try iterating over all items in METEO vector
  sf = new station_facade_c (agent, nullptr, nullptr, meteo);

  try {
    sf->find_station (location);
  } catch (...) {
    helper.error ("unable to find the METEO " + location + " location");
    delete sf;
    return;
  }

  try {
    sf->fill_station_data ();
  } catch (...) {
    helper.error ("unable to fill the data for METEO " + location + " location");
    delete sf;
    return;
  }

  try {
    for (auto &datatype: meteo_datatype_vec)
    {
      datatype_parsed = helper.extract_str_from_meteo_enum_map (meteo_enum_map, datatype);

      helper.verbose (
        "Printing data for station " +
        sf->get_station ()->get_name () +
        ", id = " +
        std::to_string (sf->get_station ()->get_number ()) +
        ", datatype = " +
        datatype_parsed
        );

      meteo_station_c *station = (meteo_station_c *)sf->get_station();

      switch (datatype) {
        case precip_state:
          break;
        case precip_10min:
          break;
        case precip_hourly:
          break;
        case precip_daily:
          break;
        case temp:
          helper.message (station->get_cur_temp ().to_string ());
          break;
        case temp_kelvin:
          helper.message (station->get_cur_temp_kelvin ().to_string ());
          break;
        case temp_fahrenheit:
          helper.message (station->get_cur_temp_fahrenheit ().to_string ());
          break;
        case temp_min:
          break;
        case temp_min_kelvin:
          break;
        case temp_min_fahrenheit:
          break;
        case temp_max:
          break;
        case temp_max_kelvin:
          break;
        case temp_max_fahrenheit:
          break;
        case wind_vel:
          break;
        case wind_min_vel:
          break;
        case wind_max_vel:
          break;
        case wind_vel_10min_max:
          break;
        case wind_vel_min_10min_max:
          break;
        case wind_vel_max_10min_max:
          break;
        case wind_dir:
          break;
      }
    }
  } catch (...) {
    helper.error ("unable to prepare " + datatype_parsed + " measurement");
  }

  delete sf;
}

void
handle_hydro_datatypes (
  std::vector<hydro_datatype_t>                     &hydro_datatype_vec,
  std::unordered_map<std::string, hydro_datatype_t> &hydro_enum_map,
  helper_c                                          &helper,
  agent_c                                           *agent,
  std::string                                       location
  )
{
  station_facade_c *sf = nullptr;
  std::string      datatype_parsed;

  if (hydro_datatype_vec.size () == 0) {
    return;
  }

  // try iterating over all items in HYDRO vector
  sf = new station_facade_c (agent, nullptr, nullptr, hydro);

  try {
    sf->find_station (location);
  } catch (...) {
    helper.error ("unable to find the HYDRO " + location + " location");
    delete sf;
    return;
  }

  try {
    sf->fill_station_data ();
  } catch (...) {
    helper.error ("unable to fill the data for HYDRO " + location + " location");
    delete sf;
    return;
  }

  try {
    for (auto &datatype: hydro_datatype_vec)
    {
      hydro_station_c         *station = (hydro_station_c *)sf->get_station ();
      measurement_level_c     level_m;
      measurement_discharge_c discharge_m;

      datatype_parsed = helper.extract_str_from_hydro_enum_map (hydro_enum_map, datatype);

      helper.verbose (
        "Parsing data for station " +
        sf->get_station ()->get_name () +
        ", id = " +
        std::to_string (sf->get_station ()->get_number ()) +
        ", datatype = " +
        datatype_parsed
        );

      switch (datatype) {
        case level:
          level_m = station->get_cur_level ();
          helper.message (level_m.to_string ());
          break;
        case discharge:
          discharge_m = station->get_cur_discharge ();
          helper.message (discharge_m.to_string ());
          break;
        default:
          break;
      }
    }
  } catch (...) {
    helper.error ("unable to prepare " + datatype_parsed + " measurement");
  }

  delete sf;
}

void
handle_presentation (
  )
{
  /*
  meteo_sf = new station_facade_c (agent, nullptr, nullptr, meteo);
  hydro_sf = new station_facade_c (agent, nullptr, nullptr, hydro);

  // Step 1. Obtain METEO station facade and find it's data
  try {
    meteo_sf->find_station (location);
  } catch (...) {
    helper.error ("unable to find the METEO" + location + " location");
    delete sf;
    return;
  }

  try {
    meteo_sf->fill_station_data ();
  } catch (...) {
    helper.error ("unable to fill the data for METEO" + location + " location");
    delete sf;
    return;
  }

  // Step 2. Obtain HYDRO station facade and find it's data
  try {
    hydro_sf->find_station (location);
  } catch (...) {
    helper.error ("unable to find the METEO" + location + " location");
    delete sf;
    return;
  }

  try {
    hydro_sf->fill_station_data ();
  } catch (...) {
    helper.error ("unable to fill the data for METEO" + location + " location");
    delete sf;
    return;
  }

  // Step 3. Present all data found for METEO station
  helper.verbose (
    "Parsing data for METEO station " +
    sf->get_station ()->get_name () +
    ", id = " +
    std::to_string (sf->get_station ()->get_number ())
    );

  // Step 4. Present all data found for HYDRO station
*/
  return;
}

int
main (
  int argc,
  char *argv[]
  )
{
  int             status              = 0;
  bool            presentation_issued = false;
  bool            datetime_issued     = false;
  agent_c         *agent              = nullptr;
  commandparser_c commparser          = commandparser_c (argc, argv);
  helper_c        helper              = helper_c ();

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

  for (auto &it: cmd_vec)
  {
    // sanitize user input (remove all whitespaces)
    it.second.erase (std::remove_if (it.second.begin (), it.second.end (), isspace), it.second.end ());

    switch (it.first) {
      case verbose:
        helper.flip_verbose ();
        break;
      case location:
        helper.verbose (it.second + " location selected");
        location_vec.push_back (it.second);
        break;
      case presentation:
        presentation_issued = true;
        helper.verbose ("presentation issued");
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

  if ((meteo_datatype_vec.size () == 0 && hydro_datatype_vec.size () == 0) &&
      !presentation_issued) {
    helper.error ("no datatype provided!");
  }

  for (auto sep = location_vec.size (); auto &location: location_vec)
  {
    if (!presentation_issued) {
      handle_meteo_datatypes (
        meteo_datatype_vec,
        meteo_enum_map,
        helper,
        agent,
        location
        );

      handle_hydro_datatypes (
        hydro_datatype_vec,
        hydro_enum_map,
        helper,
        agent,
        location
        );
    } else {
      handle_presentation ();
    }

    if (sep > 1) {
      helper.message ("---"); // separates each location
    }
    sep--;
  }

  agent_c::delete_instance ();

  return status;
}

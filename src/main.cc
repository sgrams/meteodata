/*
 * meteodata-git
 * Stanislaw Grams <sjg@fmdx.pl>
 *
 * src/main.cc
 * See ../LICENSE for license information
 */

#include <string>
#include <vector>
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
  int  status                  = 0;
  bool verbose_flag            = false;

  commandparser_c commparser  = commandparser_c (argc, argv);
  agent_c         *agent      = nullptr;
  station_c       *station    = nullptr;
  parser_c        *parser     = nullptr;
  data_c          *data       = nullptr;

  std::vector<std::pair<command_t, std::string>> command_vec;
  /*
  data = new data_c;
  station.set_name (name);
  try {
    parser = new parser_c (&agent, &station, data);
  } catch (const std::exception& e) {
    std::cerr << argv[0] << ": Unable to create parser: " << e.what () << ". Exiting...\n";
  }
  */
  status = commparser.get_commands (command_vec);
  if (status < 0) {
    helper_ns::error ("please double check command-line parameters");
    goto main_CLEANUP;
  } else if (status > 0) {
    helper_ns::print_help ();
    goto main_CLEANUP;
  }

  for (auto it = command_vec.begin (); it != command_vec.end (); ++it)
  {
    switch (it->first) {
      case verbose:
      verbose_flag = true;
      helper_ns::verbose ("verbose mode enabled", verbose_flag);
      break;

      case location:
      helper_ns::verbose (it->second + " location selected", verbose_flag);
      break;

      case date:
      break;

      case type:
      break;
    }
  }

main_CLEANUP:
  if (data != nullptr)       { delete data; }
  if (parser != nullptr)     { delete parser; }
  if (station != nullptr)    { delete station; }
  if (agent != nullptr)      { delete agent; }

  return status;
}

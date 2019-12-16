/*
 * meteodata-git
 * Stanislaw Grams <sjg@fmdx.pl>
 *
 * src/main.cc
 * See ../LICENSE for license information
 */

#include <iostream>
#include <string>
#include "../include/agent.hh"
#include "../include/data.hh"
#include "../include/station.hh"
#include "../include/parser.hh"

int
main (int argc, char *argv[]) {
  int status = 0;

  agent_c   agent;
  parser_c  *parser   = nullptr;

  station_c station;
  data_c    *data     = nullptr;

  std::string name = "LIDZBARK";

  //
  data = new data_c;
  station.set_name (name);
  try {
    parser = new parser_c (&agent, &station, data);
  } catch (const std::exception& e) {
    std::cerr << argv[0] << ": Unable to create parser: " << e.what () << ". Exiting...\n";
  }
  parser->get_measurement (temp_auto);

  if (data != nullptr) {
    delete data;
  }

  if (parser != nullptr) {
    delete parser;
  }

  return status;
}

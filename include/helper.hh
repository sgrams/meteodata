/*
 * meteodata-git
 * Stanislaw Grams <sjg@fmdx.pl>
 *
 * src/helper.hh
 * See ../LICENSE for license information
 */
#ifndef METEODATA_HELPER_H
#define METEODATA_HELPER_H
#include <iostream>
#include <stdexcept>
#include <cstdint>
#include <vector>
#include <string>
#include "types.hh"

#define METEODATA_NAME    "meteodata"
#define METEODATA_VERSION "0.1-git"

namespace helper_ns {
  void print_help (void);
  void verbose    (std::string msg, bool verbose_flag);
  void error      (std::string msg);
  void message    (std::string msg);
}
#endif // METEODATA_HELPER_H

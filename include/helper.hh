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
#include <algorithm>
#include <unordered_map>
#include "types.hh"

#define METEODATA_NAME    "meteodata"
#define METEODATA_VERSION "0.1-git"

class helper_c {
  private:
    bool verbose_flag{false};

  public:
    void print_help ();
    void error      (std::string msg);
    void message    (std::string msg);
    void verbose    (std::string msg);

    void flip_verbose ();

    std::string
    extract_str_from_meteo_enum_map (
      std::unordered_map <std::string, meteo_datatype_t> &enum_map,
      meteo_datatype_t &value
      );

    std::string
    extract_str_from_hydro_enum_map (
      std::unordered_map <std::string, hydro_datatype_t> &enum_map,
      hydro_datatype_t &value
      );
};

#endif // METEODATA_HELPER_H

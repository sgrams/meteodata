/*
 * meteodata-git
 * Stanislaw Grams <sjg@fmdx.pl>
 *
 * src/station.hh
 * See ../LICENSE for license information
 */
#ifndef METEODATA_STATION_H
#define METEODATA_STATION_H
#include <cstdint>
#include <stdexcept>
#include <string>
#include "types.hh"

class station_c {
  private:
  std::string   name;
  uint64_t      number  = 0;

  public:
  std::string   get_name ();
  void          set_name (std::string name);

  uint64_t      get_number ();
  void          set_number (uint64_t number);
};

#endif // METEODATA_STATION_H

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
#include <algorithm>
#include <vector>
#include <sstream>
#include <iomanip>
#include "types.hh"

class measurement_c {
  private:
    float value = 0.00;
  public:
    virtual ~measurement_c () {};

    void set_value (float value) {
      this->value = value;
    };

    float get_value () const {
      return value;
    };

    virtual std::string to_string () const = 0;
};

class measurement_impl_c : public measurement_c {
  public:
    std::string to_string () const override {
      std::stringstream stream;
      stream << std::fixed << std::setprecision (2) << this->get_value ();
      return stream.str ();
    };
};

class measurement_temp_celsius_c: public measurement_c {
  public:
    std::string to_string () const override {
      float celsius = this->get_value ();
      std::stringstream stream;
      stream << std::fixed << std::setprecision (2) << celsius;
      return stream.str () + " °C";
    };
};

class measurement_temp_fahrenheit_c : public measurement_c {
  public:
    std::string to_string () const override {
      float fahrenheit = this->get_value () * 1.8 + 32.0;
      std::stringstream stream;
      stream << std::fixed << std::setprecision (2) << fahrenheit;
      return stream.str () + " °F";
    };
};

class measurement_temp_kelvin_c : public measurement_c {
  public:
    std::string to_string () const override {
      float kelvin = this->get_value () + 293.15;
      std::stringstream stream;
      stream << std::fixed << std::setprecision (2) << kelvin;
      return stream.str () + " °K";
    };
};

class measurement_level_c : public measurement_c {
  public:
    std::string to_string () const override {
      std::stringstream stream;
      stream << std::fixed << std::setprecision (2) << this->get_value ();
      return stream.str () + " cm";
    };
};

class measurement_discharge_c : public measurement_c {
  public:
    std::string to_string () const override {
      std::stringstream stream;
      stream << std::fixed << std::setprecision (2) << this->get_value ();
      return stream.str () + " m³/s";
    };
};

class station_c {
  protected:
    std::string             name;
    std::string             province;
    std::string             river;
    uint64_t                number = 0;
    flavour_t               flavour;

  public:
    std::string  get_name ();
    void         set_name (std::string name);

    std::string  get_province ();
    void         set_province (std::string province);

    std::string  get_river ();
    void         set_river (std::string river);

    uint64_t     get_number ();
    void         set_number (uint64_t number);

    flavour_t    get_flavour ();

    station_c (
      flavour_t flavour
      ) {
      this->flavour = flavour;
    };
    virtual ~station_c () {};

    // common interface
    virtual station_c   *clone () const = 0;
    virtual std::string get_presentation () = 0;
};

class meteo_station_c : public station_c {
  private:
    meteo_data_t meteo_data;

  public:
    meteo_station_c (
      ) : station_c (meteo) {};
    ~meteo_station_c () {};

    measurement_temp_celsius_c    get_cur_temp ();
    measurement_temp_kelvin_c     get_cur_temp_kelvin ();
    measurement_temp_fahrenheit_c get_cur_temp_fahrenheit ();

    station_c *clone () const override {
      return new meteo_station_c (*this);
    };
    std::string get_presentation () override;
    void  set_data (
      meteo_data_t &meteo_data
      );
};

class hydro_station_c : public station_c {
  private:
    hydro_data_t hydro_data;

  public:
    hydro_station_c (
      ) : station_c (hydro)
    {};
    ~hydro_station_c () {};

    measurement_level_c     get_cur_level ();
    measurement_discharge_c get_cur_discharge ();

    station_c *clone () const override {
      return new hydro_station_c (*this);
    };

    std::string get_presentation () override;
    void  set_data (
      hydro_data_t &hydro_data
      );
};
#endif // METEODATA_STATION_H

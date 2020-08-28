/*
 * meteodata-git
 * Stanislaw Grams <sjg@fmdx.pl>
 *
 * src/agent.hh
 * See ../LICENSE for license information
 */
#ifndef METEODATA_AGENT_H
#define METEODATA_AGENT_H
#include <cstdint>
#include <memory>
#include <atomic>
#include <mutex>
#include <curl/curl.h>
#include "types.hh"
#include "data.hh"

#define AGENT_IDFETCHER_URI_PREFIX  "https://hydro.imgw.pl/api/search/?query="
#define AGENT_IDFETCHER_URI_SUFFIX  "&limit=10"
#define AGENT_DATAFETCHER_METEO_URI "https://hydro.imgw.pl/api/station/meteo/?id="
#define AGENT_DATAFETCHER_HYDRO_URI "https://hydro.imgw.pl/api/station/hydro/?id="
#define AGENT_USERAGENT             "meteodata-agent/git"
#define AGENT_TIMEOUT 10

class agent_c {
  private:
    static std::atomic<agent_c *> agent_instance;
    static std::mutex             agent_mutex;

    // private methods
    static size_t curl_callback (char *in, size_t size, size_t nmemb, std::string *out);
    int           curl_retrieve (std::string url, std::string *buffer);

    // private constructors (avoid creating new instances)
    agent_c ();
    ~agent_c ();

  public:
    // methods
    static agent_c *fetch_instance ();
    static void    delete_instance ();

    // fetcher methods
    int idfetcher_execute   (std::string name, data_c *data);
    int datafetcher_execute (uint64_t id, data_c *data, flavour_t flavour);
};
#endif // METEODATA_AGENT_H

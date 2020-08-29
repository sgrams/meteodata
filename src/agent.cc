/*
 * meteodata-git
 * Stanislaw Grams <sjg@fmdx.pl>
 *
 * src/agent.cc
 * See ../LICENSE for license information
 */
#include "../include/agent.hh"
#include <iostream>

std::atomic<agent_c *> agent_c::agent_instance{nullptr};
std::mutex             agent_c::agent_mutex;

agent_c::agent_c ()
{
  curl_global_init (CURL_GLOBAL_DEFAULT);
}

agent_c::~agent_c () {
  curl_global_cleanup ();
}

void
agent_c::delete_instance () {
  agent_c *ptr = agent_instance.load (std::memory_order_acquire);
  if (nullptr != ptr) {
    ptr = agent_instance.load (std::memory_order_relaxed);
    if (nullptr != ptr) {
      delete ptr;
      agent_instance.store (nullptr, std::memory_order_release);
      agent_mutex.unlock ();
    }
  }
}

agent_c *
agent_c::fetch_instance () {
  agent_c *ptr = agent_instance.load (std::memory_order_acquire);
  if (nullptr == ptr) {
    std::lock_guard<std::mutex> lock (agent_mutex);
    ptr = agent_instance.load (std::memory_order_relaxed);
    if (nullptr == ptr) {
      ptr = new agent_c ();
      agent_instance.store (ptr, std::memory_order_release);
    }
  }
  return ptr;
}

size_t
agent_c::curl_callback (
  char *in,
  size_t size,
  size_t nmemb,
  std::string *out
  )
{
  size_t realsize = size * nmemb;

  if (in == nullptr || out == nullptr) {
    throw std::invalid_argument ("parameters set incorrectly");
    return 0;
  }

  out->append (in, realsize);
  return realsize;
}

int
agent_c::curl_retrieve (
  std::string url,
  std::string *buffer
  )
{
  CURLcode status;
  CURL     *conn = nullptr;

  // prepare curl
  conn = curl_easy_init ();
  if (conn == nullptr) {
    return -1;
  }

  // set curl options
  curl_easy_setopt (conn, CURLOPT_URL, url.c_str ());
  curl_easy_setopt (conn, CURLOPT_TIMEOUT, AGENT_TIMEOUT);
  curl_easy_setopt (conn, CURLOPT_WRITEFUNCTION, curl_callback);
  curl_easy_setopt (conn, CURLOPT_WRITEDATA, buffer);
  curl_easy_setopt (conn, CURLOPT_USERAGENT, AGENT_USERAGENT);

  // fetch data
  status = curl_easy_perform (conn);

  // cleanup
  curl_easy_cleanup (conn);

  return (status == CURLE_OK) ? 0 : status;
}

int
agent_c::idfetcher_execute (
  std::string name,
  data_c *data
  )
{
  std::string url;
  char        *name_encoded = nullptr;

  if (data == nullptr || name.length () <= 0) {
    return -1;
  }

  // encode the name
  name_encoded = curl_easy_escape (nullptr, name.c_str (), name.length ());

  // build an url
  url.append (AGENT_IDFETCHER_URI_PREFIX);
  url.append (name_encoded);
  url.append (AGENT_IDFETCHER_URI_SUFFIX);

  // cleanup
  if (name_encoded) {
    curl_free (name_encoded);
  }

  return this->curl_retrieve (url, &data->buffer);
}

int
agent_c::datafetcher_execute (
  uint64_t id,
  data_c   *data,
  flavour_t flavour
  )
{
  std::string url;

  if (data == nullptr || id == 0) {
    return -1;
  }

  // build an url
  if (flavour == meteo) {
    url.append (AGENT_DATAFETCHER_METEO_URI);
  } else if (flavour == hydro) {
    url.append (AGENT_DATAFETCHER_HYDRO_URI);
  }
  url.append (std::to_string (id));

  return this->curl_retrieve (url, &data->buffer);
}

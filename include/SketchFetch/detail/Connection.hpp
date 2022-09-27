#pragma once
#include <fstream>
#include <future>
#include <optional>
#include <span>
#include <string>
#include <variant>

#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/os.h>
#include <nlohmann/json.hpp>

#include "Authentication.hpp"
#include "Util.hpp"
#include "restclient-cpp/connection.h"
#include "restclient-cpp/restclient.h"
using json = nlohmann::json;
namespace SketchFetch
{
/**
 * @brief Handles Connction to Sketchfab and provides get and download functions
 *
 *
 */
class Connection
{
private:
  Authentication::Auth auth;
  using download_t = std::vector<uint8_t>;

public:
  /**
   * @brief initialise rest client and authentication
   */
  Connection(std::string_view username_, std::string_view password_);

  /**
   * @brief initialise rest client
   */
  Connection();
  /**
   * @brief closes the rest client
   */
  ~Connection();

  Connection(const Connection&) = delete;
  Connection& operator=(const Connection&) = delete;
  /**
   * @brief makes a get request to the Sketchfab API and parses the response as json
   */
  auto get(const std::string& query_) const -> std::optional<json>;

  /**
   * @brief downloads data at url
   * @return optional download_t
   */
  auto download(std::string_view url) const -> std::optional<download_t>;

  /**
   * @brief request download uri for models
   * @return error code or download uri
   */
  auto getModelDownloadURI(std::string_view model_uid) const -> std::variant<std::string, int>;

  /**
   * @brief download model and stores it in vector
   * @return optional download_t
   */
  auto downloadModel(std::string_view model_url) const -> std::optional<download_t>;
  auto downloadThumbnails(std::span<std::string> urls) const -> std::optional<std::vector<download_t>>;

  /**
   * @brief Tries to get an access token using username and password
   * @details Uses an already stored token in the sketchfab.auth file.
   * @exception throws on error
   */
  auto setAccess(std::string_view, std::string_view) -> void;
  /**
   * @brief Like setAccess but only uses auth file
   * @exception throws on error
   */
  auto authenticate() -> bool;

  /**
   * @brief get authentication status
   */
  auto getAuthenticated() -> bool;
};

Connection::Connection()
{
  RestClient::init();
}

Connection::Connection(std::string_view username_, std::string_view password_)
    : auth(username_, password_)
{
  RestClient::init();
}

Connection::~Connection()
{
  RestClient::disable();
}

inline auto Connection::get(const std::string& query) const -> std::optional<json>
{
  Util::Timer t;
  RestClient::Connection conn {SketchFabAPI::api_endpoint.data()};
  if (auth.getAuthenticated()) {
    RestClient::HeaderFields header {};
    header["Authorization"] = fmt::format("Bearer {}", auth.getAccessToken());
    conn.SetHeaders(header);
  }

  auto r = conn.get(query);

  if (r.code == 200) {
    try {
      return json::parse(r.body);
    } catch (const std::exception& e) {
      fmt::print(fg(fmt::color::red), "ERROR Parsing Error: {}\n", e.what());
    }
  } else {
    fmt::print(fg(fmt::color::red), "ERROR Response Code: {}\n", r.code);
  }
  return std::nullopt;
}

auto Connection::download(std::string_view url) const -> std::optional<download_t>
{
  Util::Timer t;
  RestClient::Connection conn {url.data()};
  auto r = conn.get("");
  if (r.code == 200) {
    return download_t(r.body.begin(), r.body.end());
  } else {
    fmt::print(fg(fmt::color::red), "ERROR Response Code: {}\n", r.code);
  }
  return std::nullopt;
}

auto Connection::getModelDownloadURI(std::string_view model_uid) const -> std::variant<std::string, int>
{
  std::variant<std::string, int> ret;
  RestClient::HeaderFields header {};
  header["Authorization"] = fmt::format("Bearer {}", auth.getAccessToken());
  RestClient::Connection conn {SketchFabAPI::download_endpoint.data()};
  conn.SetHeaders(header);
  auto r = conn.get(fmt::format("/{}/download", model_uid));
  if (r.code == 200) {
    try {
      auto resp = json::parse(r.body);
      ret = resp.at("gltf").at("url").get<std::string>();  // get options
    } catch (const std::exception& e) {
      fmt::print(fg(fmt::color::red), "ERROR Parsing Error: {}\n", e.what());
    }
  } else {
    ret = r.code;
  }
  return ret;
}

auto Connection::downloadModel(std::string_view model_url) const -> std::optional<download_t>
{
  return download(model_url);
}

auto Connection::downloadThumbnails(std::span<std::string> urls) const -> std::optional<std::vector<download_t>>
{
  std::vector<std::future<std::optional<download_t>>> futures;
  std::vector<download_t> results;
  for (auto&& url : urls) {
    futures.push_back(std::async(std::launch::async, [this, url]() { return download(url); }));
  }
  for (auto&& f : futures) {
    try {
      if (auto result = f.get()) {
        if (result) {
          results.push_back(std::move(*result));
        }
      }
    } catch (const std::exception& e) {
      fmt::print(fg(fmt::color::red), "ERROR Future Get: {}\n", e.what());
    }
  }
  return results.empty() ? std::nullopt : std::make_optional(std::move(results));
}

inline auto Connection::setAccess(std::string_view username_, std::string_view password_) -> void
{
  auth.authenticate(username_, password_);
}

inline auto Connection::authenticate() -> bool
{
  auth.authenticate();
  return auth.getAuthenticated();
}

inline auto Connection::getAuthenticated() -> bool
{
  return auth.getAuthenticated();
}
}  // namespace SketchFetch

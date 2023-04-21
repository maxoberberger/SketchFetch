#pragma once
#include <fstream>
#include <future>
#include <optional>
#include <span>
#include <string>
#include <variant>

#include <nlohmann/json.hpp>

#include "Authentication.h"

namespace SketchFetch
{
using json = nlohmann::json;
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

}  // namespace SketchFetch

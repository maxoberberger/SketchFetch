#pragma once

#include <charconv>
#include <fstream>
#include <string_view>

#include "SketchFabAPI.h"

namespace SketchFetch::Authentication
{
class Auth
{
public:
  Auth() = default;
  Auth(std::string_view, std::string_view);

  /**
   * @brief calls authenticate and set username and password
   */
  auto authenticate(std::string_view, std::string_view) -> void;

  /**
   * @brief tries to authenticate on Sketchfab
   * @details first tires to use a stored token if there is none it will aquire one\
   *          it will refresh the token if it is expired
   * @exception throws on error
   */
  auto authenticate() -> void;
  auto getAuthenticated() const -> bool;
  [[nodiscard]] auto getAccessToken() const -> std::string_view;

private:
  std::string username, password, client_id;
  std::string access_token, refresh_token;
  std::string auth_token {SketchFabAPI::auth_token};
  long long token_valid_until;
  bool authenticated {false};  // True if authentication was successful

  auto getToken() -> void;
  auto refreshToken() -> void;
  auto storeToken() const -> void;
  auto loadToken() -> bool;

  auto sendAndParse(std::string const&) -> void;
};

}  // namespace SketchFetch::Authentication

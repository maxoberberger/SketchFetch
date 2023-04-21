#include "Authentication.h"

#include "Exception.h"
#include "Util.h"
#include "fmt/chrono.h"
#include "fmt/color.h"
#include "fmt/format.h"
#include "fmt/os.h"
#include "restclient-cpp/connection.h"
#include "restclient-cpp/restclient.h"

namespace SketchFetch::Authentication
{

/**
 * @brief Provides access token for SketchFab API access
 * @pre RestClient has to be initialized
 */
Auth::Auth(std::string_view username_, std::string_view password_)
    : username(Util::percentEncode(username_))
    , password(Util::percentEncode(password_))
    , auth_token(SketchFabAPI::auth_token)
{
  authenticate();
};

auto Auth::authenticate() -> void
{
  using namespace std::chrono;
  const bool loaded = loadToken();
  if (loaded && time_point_cast<milliseconds>(system_clock::now()).time_since_epoch().count() < token_valid_until) {
    authenticated = true;
    return;
  } else if (loaded) {
    refreshToken();
  } else {
    getToken();
  }
  authenticated = true;
  storeToken();
}

auto Auth::authenticate(std::string_view username_, std::string_view password_) -> void
{
  username = username_;
  password = password_;
  authenticate();
}

auto Auth::getAuthenticated() const -> bool
{
  return authenticated;
}

auto Auth::getAccessToken() const -> std::string_view
{
  if (!authenticated)
    throw std::runtime_error("Missing Username and Password");
  return std::string_view(access_token);
};

auto Auth::getToken() -> void
{
  const auto query =
      fmt::format("grant_type=password&client_id={}&username={}&password={}", auth_token, username, password);
  sendAndParse(query);
}

auto Auth::refreshToken() -> void
{
  const auto query = fmt::format(
      "grant_type=refresh_token&refresh_token={}&client_id={}&username={}&"
      "password={}",
      refresh_token,
      client_id,
      username,
      password);

  sendAndParse(query);
}

auto Auth::storeToken() const -> void
{
  auto file = fmt::output_file("sketchfab.auth");

  file.print("{};{};{}", access_token, refresh_token, token_valid_until);
}

auto Auth::loadToken() -> bool
{
  std::ifstream file {"sketchfab.auth", std::ios::in};
  std::tm t {};
  if (file.good()) {
    std::getline(file, access_token, ';');
    std::getline(file, refresh_token, ';');
    std::string time_string;
    std::getline(file, time_string, ';');
    std::from_chars(time_string.c_str(), time_string.c_str() + time_string.size(), token_valid_until);
  } else {
    return false;
  }
  return true;
}

auto Auth::sendAndParse(std::string const& query) -> void
{
  RestClient::Connection conn {SketchFabAPI::auth_base.data()};
  const auto response = conn.post(SketchFabAPI::auth_query.data(), query);
  if (response.code == 200) {
    auto json_response = nlohmann::json::parse(response.body);
    access_token = json_response.at("access_token");
    refresh_token = json_response.at("refresh_token");
    const int expires_in = json_response.at("expires_in");
    token_valid_until = (time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now())
                         + std::chrono::seconds {expires_in})
                            .time_since_epoch()
                            .count();
  } else {
    throw authentication_error(fmt::format("Authentication Error:\tCode: {}\n{}\n", response.code, response.body));
  }
}

}  // namespace SketchFetch::Authentication
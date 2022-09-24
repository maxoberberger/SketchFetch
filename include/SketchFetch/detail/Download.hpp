#pragma once

#include <future>
#include <span>
#include <string>
#include <vector>

#include "Connection.hpp"

namespace SketchFetch
{
auto download(std::span<std::string> urls, Connection& conn)
    -> std::optional<std::vector<std::vector<uint8_t>>>
{
  Util::Timer t;
  std::vector<std::future<std::optional<std::vector<uint8_t>>>> futures;
  std::vector<std::vector<uint8_t>> results;
  for (auto&& url : urls) {
    futures.push_back(std::async(
        std::launch::async, [&conn, url]() { return conn.download(url); }));
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
  return results.empty() ? std::nullopt
                         : std::make_optional(std::move(results));
};

}  // namespace SketchFetch

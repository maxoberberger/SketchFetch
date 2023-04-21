#pragma once

#include <filesystem>
#include <string>

#include "detail/Connection.h"
#include "detail/ModelSearch.h"

#ifdef _WIN32
#  ifdef SKETCH_FETCH_EXPORT
#    define SKETCH_FETCH_API __declspec(dllexport)
#  else
#    define SKETCH_FETCH_API __declspec(dllimport)
#  endif
#else
#  define SKETCH_FETCH_API
#endif

namespace SketchFetch
{
enum class SketchFlag : bool
{
  CreateFolder = true,
  DontCreateFolder = false
};

class SKETCH_FETCH_API SketchFetch
{
public:
  using ModelSearchResults = std::optional<std::vector<ModelSearchResult>>;
  using download_t = std::vector<uint8_t>;

  SketchFetch() = default;
  SketchFetch(std::filesystem::path, std::filesystem::path, std::filesystem::path);
  SketchFetch(std::string_view, std::string_view, SketchFlag);

  auto setAccess(std::string_view, std::string_view) -> void;
  auto authenticate() -> bool;
  auto getAuthenticated() -> bool;
  auto setThumbnailFolder(std::filesystem::path const&, bool) -> void;
  auto setModelFolder(std::filesystem::path const&, bool) -> void;
  auto setWorkingFolder(std::filesystem::path const&) -> void;

  auto getWorkingFolder() -> std::filesystem::path;
  auto getModelFolder() -> std::filesystem::path;
  auto getThumbnailFolder() -> std::filesystem::path;

  [[nodiscard]] auto search(ModelSearchQuery const&) const -> std::optional<std::vector<ModelSearchResult>>;
  [[nodiscard]] auto searchJSON(ModelSearchQuery const&) const -> std::optional<json>;

  // auto getThumbnails(ModelSearchResult const&) const
  //     -> std::unordered_map<std::string, download_t>;

  auto getThumbnail(ModelSearchResult const&) const -> std::optional<download_t>;
  auto downloadThumbnail(ModelSearchResult const&) const -> void;

  auto getThumbnail(std::string_view) const -> std::optional<download_t>;
  auto downloadThumbnail(std::string_view, std::string_view) const -> void;

  [[nodiscard]] auto getModel(ModelSearchResult const&) const -> std::optional<download_t>;
  auto downloadModel(std::string_view) const -> void;
  auto downloadModel(ModelSearchResult const&) const -> void;

  auto print() -> void;

private:
  std::filesystem::path working_folder = std::filesystem::current_path();
  std::filesystem::path thumbnail_folder = std::filesystem::current_path().append("thumbnails");
  std::filesystem::path model_folder = std::filesystem::current_path().append("models");
  Connection conn;

  auto storeToDisk(const download_t&, std::filesystem::path) const -> void;
  auto createOrCheckFolder(std::filesystem::path const&, bool) const -> void;
};

}  // namespace SketchFetch
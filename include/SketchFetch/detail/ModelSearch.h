#pragma once
#include <optional>
#include <string>
#include <vector>

namespace SketchFetch
{

// class SketchFetch;  //Forward declaration

enum class date_t : uint8_t
{
  day = 1,
  week = 7,
  month = 31
};

enum class sort_by_t
{
  likeCount,
  nlikeCount,
  viewCount,
  nviewCount,
  publishedAt,
  npublishedAt,
  processedAt,
  nprocessedAt
};

enum class archive_t
{
  gltf,
  usdz,
  glb
};

struct ModelGetResult
{
  std::string uri, uid, name, description;
  bool isDownloadable;
};

struct ModelSearchResult
{
  friend class SketchFetch;
  std::string uri, uid, name, description, thumbnail;
  std::size_t views, likes, vertexCount, faceCount;
  bool isDownloadable;
};

struct ModelSearchQuery
{
  std::optional<std::string> q;
  std::optional<std::string> user;
  std::optional<std::vector<std::string>> tags;
  std::optional<std::vector<std::string>> categories;
  std::optional<date_t> date;
  std::optional<bool> downloadable;
  std::optional<bool> animated;
  std::optional<bool> staffpicked;
  std::optional<bool> sound;
  std::optional<uint32_t> min_face_count;
  std::optional<uint32_t> max_face_count;
  std::optional<std::string> pbr_type;  // dropdown
  std::optional<bool> rigged;
  std::optional<std::string> collection;
  std::optional<sort_by_t> sort_by;  // dropdown
  std::optional<std::string> file_format;
  std::optional<std::string> license;  // dropdown
  std::optional<std::string> max_uv_layer_count;
  std::optional<std::string> available_archive_type;
  std::optional<uint32_t> archives_max_size;
  std::optional<uint32_t> archives_max_face_count;
  std::optional<uint32_t> archives_max_vertex_count;
  std::optional<uint32_t> archives_max_texture_count;
  std::optional<uint32_t> archives_texture_max_resolution;
  std::optional<bool> archives_flavours;
  uint32_t count {24};
  uint32_t cursor {0};

  operator std::string() const;
};

}  // namespace SketchFetch

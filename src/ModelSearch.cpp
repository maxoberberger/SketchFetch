#include "ModelSearch.h"

#include <fmt/format.h>

#include "Util.h"

namespace SketchFetch
{

ModelSearchQuery::operator std::string() const
{
  std::string ret = fmt::format("/search?count={}&cursor={}&type=models", count, cursor);
  if (q)
    ret += fmt::format("&q={}", Util::percentEncode(*q));

  if (user)
    ret += fmt::format("&user={}", *user);

  if (tags) {
    std::string tag_str;
    for (auto& tag : *tags)
      tag_str += fmt::format("{},", tag);
    ret += fmt::format("&tags={}", Util::percentEncode(tag_str));
  }

  if (categories) {
    std::string cat_str;
    for (auto& cat : *categories)
      cat_str += fmt::format("{},", cat);
    ret += fmt::format("&categories={}", Util::percentEncode(cat_str));
  }

  if (date) {
    switch (*date) {
      case date_t::day:
        ret += "&date=1";
        break;
      case date_t::week:
        ret += "&date=7";
        break;
      case date_t::month:
        ret += "&date=31";
        break;
    }
  }

  if (downloadable)
    ret += fmt::format("&downloadable={}", *downloadable);
  if (animated)
    ret += fmt::format("&animated={}", *animated);
  if (staffpicked)
    ret += fmt::format("&staffpicked={}", *staffpicked);
  if (sound)
    ret += fmt::format("&sound={}", *sound);
  if (min_face_count)
    ret += fmt::format("&min_face_count={}", *min_face_count);
  if (max_face_count)
    ret += fmt::format("&max_face_count={}", *max_face_count);
  if (pbr_type)
    ret += fmt::format("&pbr_type={}", *pbr_type);
  if (rigged)  // dropdown TODO:
    ret += fmt::format("&rigged={}", *rigged);
  if (collection)  // dropdown TODO:
    ret += fmt::format("&collection={}", *collection);

  if (sort_by) {
    switch (*sort_by) {
      case sort_by_t::likeCount:
        ret += "&sort_by=likeCount";
        break;
      case sort_by_t::nlikeCount:
        ret += "&sort_by=-likeCount";
        break;
      case sort_by_t::viewCount:
        ret += "&sort_by=viewCount";
        break;
      case sort_by_t::nviewCount:
        ret += "&sort_by=-viewCount";
        break;
      case sort_by_t::publishedAt:
        ret += "&sort_by=publishedAt";
        break;
      case sort_by_t::npublishedAt:
        ret += "&sort_by=-publishedAt";
        break;
      case sort_by_t::processedAt:
        ret += "&sort_by=processedAt";
        break;
      case sort_by_t::nprocessedAt:
        ret += "&sort_by=-processedAt";
        break;
      default:
        break;
    }
  }

  if (file_format)
    ret += fmt::format("&file_format={}", *file_format);
  if (license)  // dropdown TODO:
    ret += fmt::format("&license={}", *license);
  if (max_uv_layer_count)
    ret += fmt::format("&max_uv_layer_count={}", *max_uv_layer_count);
  if (available_archive_type)
    ret += fmt::format("&available_archive_type={}", *available_archive_type);
  if (archives_max_size)
    ret += fmt::format("&archives_max_size={}", *archives_max_size);
  if (archives_max_face_count)
    ret += fmt::format("&archives_max_face_count={}", *archives_max_face_count);
  if (archives_max_vertex_count)
    ret += fmt::format("&archives_max_vertex_count={}", *archives_max_vertex_count);
  if (archives_max_texture_count)
    ret += fmt::format("&archives_max_texture_count={}", *archives_max_texture_count);
  if (archives_texture_max_resolution)
    ret += fmt::format("&archives_texture_max_resolution={}", *archives_texture_max_resolution);
  if (archives_flavours)
    ret += fmt::format("&archives_flavours={}", *archives_flavours);
  fmt::print("{}\n", ret);
  return ret;
}

}  // namespace SketchFetch

template<>
struct fmt::formatter<SketchFetch::ModelSearchResult>
{
  constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) { return ctx.begin(); }
  template<typename FormatContext>
  auto format(SketchFetch::ModelSearchResult const& result, FormatContext& ctx) const -> decltype(ctx.out())
  {
    return fmt::format_to(ctx.out(), "[{};{}]", result.name, result.description);
  }
};
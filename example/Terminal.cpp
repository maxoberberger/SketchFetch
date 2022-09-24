#include <SketchFetch/SketchFetch.hpp>

int main(int, char const**)
{
  SketchFetch::SketchFetch fetchy {};
  fmt::print("{}\n", SketchFetch::Util::percentEncode("#@test"));
  fetchy.print();

  auto result = fetchy.search({.q = "bunny",
                               .tags = {{}},
                               .date = SketchFetch::date_t::month,
                               .downloadable = true,
                               .sort_by = SketchFetch::sort_by_t::nviewCount,
                               .count = 24});

  // fetchy.downloadThumbnail(result->at(4));
}

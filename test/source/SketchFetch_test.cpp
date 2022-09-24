#include "SketchFetch/SketchFetch.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Name is SketchFetch", "[library]")
{
  REQUIRE(SketchFetch::Util::percentEncode("#@test").compare("%23%40test")
          == 0);
}

cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
  SketchFetch
  VERSION 0.1.0
  DESCRIPTION "A C++ Wrapper for the SketchFetch Download and Search API"
  HOMEPAGE_URL "https://example.com/"
  LANGUAGES CXX
  )

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(SketchFetch_SketchFetch SHARED)
add_library(SketchFetch::SketchFetch ALIAS SketchFetch_SketchFetch)

set_property(
    TARGET SketchFetch_SketchFetch PROPERTY
    EXPORT_NAME SketchFetch
)

target_sources(
  SketchFetch_SketchFetch
  PRIVATE

  include/SketchFetch/SketchFetch.h
  include/SketchFetch/detail/Authentication.h
  include/SketchFetch/detail/Connection.h
  include/SketchFetch/detail/Exception.h
  include/SketchFetch/detail/ModelSearch.h
  include/SketchFetch/detail/SketchFabAPI.h
  include/SketchFetch/detail/Util.h

  src/Authentication.cpp
  src/Connection.cpp
  src/ModelSearch.cpp
  src/SketchFetch.cpp
)

target_include_directories(
    SketchFetch_SketchFetch ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
    PRIVATE
    include/SketchFetch
    include/SketchFetch/detail
)

target_compile_definitions(SketchFetch_SketchFetch PRIVATE SKETCH_FETCH_EXPORT)

find_package(CURL CONFIG REQUIRED)
find_package(restclient-cpp CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(libzippp CONFIG REQUIRED)

target_link_libraries(SketchFetch_SketchFetch 
 PRIVATE
 fmt::fmt
 nlohmann_json::nlohmann_json
 restclient-cpp
 libzippp::libzippp
)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Examples ----

if(PROJECT_IS_TOP_LEVEL)
  option(BUILD_EXAMPLES "Build examples tree." "${SketchFetch_DEVELOPER_MODE}")
  if(BUILD_EXAMPLES)
    add_subdirectory(example)
  endif()
endif()

# ---- Developer mode ----

if(NOT SketchFetch_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of SketchFetch"
  )
endif()

include(cmake/dev-mode.cmake)

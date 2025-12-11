cmake_minimum_required(VERSION 4.1)
project(libwebm-py LANGUAGES CXX)

# C++17 is required for nanobind
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# FetchContent for downloading libwebm
include(FetchContent)

# Force Position Independent Code for all targets
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# JSON から依存関係のバージョンを読み込み
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/deps.json DEPS_JSON)

# libwebm の設定を解析
string(JSON LIBWEBM_VERSION GET ${DEPS_JSON} libwebm version)
string(JSON LIBWEBM_URL GET ${DEPS_JSON} libwebm url)

# バージョンから libwebm- prefix を削除
string(REPLACE "libwebm-" "" LIBWEBM_VERSION_NUMBER ${LIBWEBM_VERSION})

message(STATUS "libwebm version: ${LIBWEBM_VERSION}")
message(STATUS "libwebm URL: ${LIBWEBM_URL}")

# Download libwebm from chromium.googlesource.com
FetchContent_Declare(
  libwebm
  GIT_REPOSITORY ${LIBWEBM_URL}
  GIT_TAG        ${LIBWEBM_VERSION}
  GIT_SHALLOW    TRUE
)

# Make libwebm available
FetchContent_MakeAvailable(libwebm)

# Find Python and nanobind
find_package(Python REQUIRED COMPONENTS Interpreter Development.Module)
execute_process(
  COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
  OUTPUT_VARIABLE NB_DIR
  OUTPUT_STRIP_TRAILING_WHITESPACE
  COMMAND_ERROR_IS_FATAL ANY
)
list(APPEND CMAKE_PREFIX_PATH "${NB_DIR}")
find_package(nanobind CONFIG REQUIRED)

# Create Python extension module
nanobind_add_module(
  _webm
  STABLE_ABI
  NB_STATIC
  src/bindings/webm_bindings.cpp
  src/bindings/webm_reader.cpp
  src/bindings/webm_writer.cpp
)

# Link with libwebm
target_link_libraries(_webm PRIVATE webm)

# Include directories
target_include_directories(_webm PRIVATE
  ${libwebm_SOURCE_DIR}
)

# Set properties
set_target_properties(_webm PROPERTIES
  CXX_VISIBILITY_PRESET hidden
  VISIBILITY_INLINES_HIDDEN YES
)

# Install the extension module
install(TARGETS _webm
  LIBRARY DESTINATION .
  RUNTIME DESTINATION .
)

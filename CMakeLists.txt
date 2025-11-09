cmake_minimum_required(VERSION 4.1)
project(libwebm-py LANGUAGES CXX)

# C++17 is required for nanobind
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# FetchContent for downloading libwebm
include(FetchContent)

# Force Position Independent Code for all targets
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Download libwebm from GitHub
FetchContent_Declare(
  libwebm
  GIT_REPOSITORY https://github.com/webmproject/libwebm.git
  GIT_TAG        libwebm-1.0.0.32
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

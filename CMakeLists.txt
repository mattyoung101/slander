cmake_minimum_required(VERSION 3.21)
project(slander
    LANGUAGES C CXX
    VERSION 1.0.0
    DESCRIPTION "SystemVerilog language server"
    HOMEPAGE_URL "https://github.com/mattyoung101/slander"
)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
set(CMAKE_C_STANDARD 11) # C11
set(CMAKE_CXX_STANDARD 20) # C++20
cmake_policy(SET CMP0144 NEW)

include(GNUInstallDirs)
include(CPack)
include(cmake/CPM.cmake)

add_executable(slander
    src/main.cpp
)
target_include_directories(slander PRIVATE include)

# find deps
CPMAddPackage("gh:gabime/spdlog@1.16.0")
CPMAddPackage("gh:fmtlib/fmt#12.1.0")
CPMAddPackage("gh:MikePopoloski/slang@9.1")
CPMAddPackage("gh:martinus/unordered_dense@4.8.1")
CPMAddPackage("gh:nlohmann/json@3.12.0")
CPMAddPackage("gh:CLIUtils/CLI11@2.6.1")

target_link_libraries(slander PRIVATE spdlog::spdlog fmt::fmt slang::slang CLI11::CLI11
    unordered_dense::unordered_dense nlohmann_json::nlohmann_json)

# note on diagnostic colour: https://stackoverflow.com/a/73349744/5007892
target_compile_options(slander PRIVATE "-Wall" "-Wextra" "-Wno-unused-parameter" "-ggdb"
    "-fdiagnostics-color=always")
target_compile_definitions(slander PRIVATE "-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE")

#### LINKER
message(STATUS "Checking if LLD is available")
execute_process(COMMAND "lld" ERROR_VARIABLE LLD_RESULT)

if (LLD_RESULT MATCHES "lld is a generic driver")
    message(STATUS "LLD is available, it will be used")
    target_link_options(slander PRIVATE "-fuse-ld=lld")
else()
    message(STATUS "LLD is NOT available, so we'll use the system default linker")
endif()

## SANITIZERS
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug build, adding sanitizers")
    target_compile_options(slander PRIVATE "-fsanitize=address" "-fsanitize=undefined")
    target_link_options(slander PRIVATE "-fsanitize=address" "-fsanitize=undefined")
endif()

## INSTALL
install(TARGETS slander)

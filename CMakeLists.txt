cmake_minimum_required(VERSION 3.21)
project(slander
    LANGUAGES C CXX
    VERSION 1.0.0
    DESCRIPTION "SystemVerilog language server"
    HOMEPAGE_URL "https://github.com/mattyoung101/slander"
)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
set(CMAKE_C_STANDARD 11) # C11
set(CMAKE_CXX_STANDARD 20) # C++20
cmake_policy(SET CMP0144 NEW)

include(GNUInstallDirs)
include(CPack)
include(cmake/CPM.cmake)

# find deps
CPMAddPackage("gh:gabime/spdlog@1.16.0")
CPMAddPackage("gh:fmtlib/fmt#12.1.0")
CPMAddPackage("gh:MikePopoloski/slang@9.1")
# CPMAddPackage("gh:martinus/unordered_dense@4.8.1")
# CPMAddPackage("gh:nlohmann/json@3.12.0")
CPMAddPackage("gh:CLIUtils/CLI11@2.6.1")
CPMAddPackage("gh:catchorg/Catch2@3.11.0")

add_executable(slander
    src/main.cpp
    src/strategy.cpp
)
target_include_directories(slander PRIVATE include)
target_link_libraries(slander PRIVATE spdlog::spdlog fmt::fmt slang::slang CLI11::CLI11)

add_executable(slander_test
    src/strategy.cpp

    test/remove_process_strategy.cpp
)
target_include_directories(slander_test PRIVATE include)
target_link_libraries(slander_test PRIVATE spdlog::spdlog fmt::fmt slang::slang Catch2::Catch2WithMain)

# note on diagnostic colour: https://stackoverflow.com/a/73349744/5007892
target_compile_options(slander PRIVATE "-Wall" "-Wextra" "-Wno-unused-parameter" "-ggdb"
    "-fdiagnostics-color=always")
target_compile_definitions(slander PRIVATE "-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE")

target_compile_options(slander_test PRIVATE "-Wall" "-Wextra" "-Wno-unused-parameter" "-ggdb"
    "-fdiagnostics-color=always")
target_compile_definitions(slander_test PRIVATE "-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE")

#### LINKER
message(STATUS "Checking if mold is available")
execute_process(COMMAND "mold" ERROR_VARIABLE MOLD_RESULT)

if (MOLD_RESULT MATCHES "option is missing")
    message(STATUS "mold is available, it will be used")
    target_link_options(slander PRIVATE "-fuse-ld=mold")
    target_link_options(slander_test PRIVATE "-fuse-ld=mold")
else()
    message(STATUS "mold is NOT available, so we'll use the system default linker")
endif()

## SANITIZERS
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug build, adding sanitizers")
    target_compile_options(slander PRIVATE "-fsanitize=address" "-fsanitize=undefined")
    target_link_options(slander PRIVATE "-fsanitize=address" "-fsanitize=undefined")

    target_compile_options(slander_test PRIVATE "-fsanitize=address" "-fsanitize=undefined")
    target_link_options(slander_test PRIVATE "-fsanitize=address" "-fsanitize=undefined")
endif()

## INSTALL
install(TARGETS slander)
install(TARGETS slander_test) # TODO do we actually want to do this?

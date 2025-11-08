cmake_minimum_required(VERSION 3.16)
project(discord_cpp_bot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER_LAUNCHER ccache)

include(FetchContent)
include(FindPkgConfig)

# =========================================================
# DEPENDENCIES (auto-download jika tidak ditemukan)
# =========================================================

# DPP
find_package(dpp QUIET CONFIG)
if (NOT dpp_FOUND)
    message(STATUS "Downloading DPP...")
    FetchContent_Declare(
        dpp
        GIT_REPOSITORY https://github.com/brainboxdotcc/DPP.git
        GIT_TAG master
    )
    FetchContent_MakeAvailable(dpp)
endif()

# Opus
pkg_check_modules(OPUS QUIET opus)
if (NOT OPUS_FOUND)
    message(STATUS "Downloading libopus...")
    FetchContent_Declare(
        opus
        GIT_REPOSITORY https://github.com/xiph/opus.git
        GIT_TAG main
    )
    FetchContent_MakeAvailable(opus)
endif()

# Sodium
find_library(SODIUM_LIB sodium)
find_path(SODIUM_INCLUDE sodium.h)
if (NOT SODIUM_LIB OR NOT SODIUM_INCLUDE)
    message(STATUS "Downloading libsodium...")
    FetchContent_Declare(
        sodium
        GIT_REPOSITORY https://github.com/jedisct1/libsodium.git
        GIT_TAG stable
    )
    FetchContent_MakeAvailable(sodium)
endif()

# JSON for Modern C++
find_path(JSON_INCLUDE_DIR nlohmann/json.hpp)
if (NOT JSON_INCLUDE_DIR)
    message(STATUS "Downloading nlohmann/json...")
    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG develop
    )
    FetchContent_MakeAvailable(nlohmann_json)
    set(JSON_INCLUDE_DIR ${nlohmann_json_SOURCE_DIR}/include)
endif()

# MLS++
find_library(MLS_LIB mlspp)
find_path(MLS_INCLUDE_DIR mlspp PATHS /usr/local/include)
if (NOT MLS_LIB OR NOT MLS_INCLUDE_DIR)
    message(STATUS "Downloading MLS++...")
    FetchContent_Declare(
        mlspp
        GIT_REPOSITORY https://github.com/cisco/mlspp.git
        GIT_TAG main
    )
    FetchContent_MakeAvailable(mlspp)
    set(MLS_INCLUDE_DIR ${mlspp_SOURCE_DIR}/include)
    set(MLS_LIB mlspp)
endif()

# =========================================================
# INCLUDE PATHS
# =========================================================
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OPUS_INCLUDE_DIRS}
    ${SODIUM_INCLUDE}
    ${JSON_INCLUDE_DIR}
    ${MLS_INCLUDE_DIR}
)

# =========================================================
# SOURCES
# =========================================================
file(GLOB COMMAND_SOURCES "commands/*.cpp")

# =========================================================
# EXECUTABLES
# =========================================================
add_executable(bot
    main.cpp
    memoryManager.cpp
    command_registry.cpp
    ${COMMAND_SOURCES}
)

add_executable(deploy
    memoryManager.cpp
    deploy.cpp
    command_registry.cpp
    ${COMMAND_SOURCES}
)

# =========================================================
# LINK LIBRARIES
# =========================================================
target_link_libraries(bot PRIVATE dpp::dpp ${OPUS_LIBRARIES} ${SODIUM_LIB} ${MLS_LIB})
target_link_libraries(deploy PRIVATE dpp::dpp ${OPUS_LIBRARIES} ${SODIUM_LIB} ${MLS_LIB})

# =========================================================
# OPTIONAL INSTALL
# =========================================================
# install(TARGETS bot deploy DESTINATION bin)

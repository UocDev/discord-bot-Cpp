cmake_minimum_required(VERSION 3.16)
project(discord_cpp_bot LANGUAGES CXX)

# =========================================================
# COMPILER SETTINGS
# =========================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

# =========================================================
# DEPENDENCIES
# =========================================================
find_package(PkgConfig REQUIRED)

# DPP (Discord++)
find_package(dpp REQUIRED CONFIG)

# Opus (Voice codec)
pkg_check_modules(OPUS REQUIRED opus)

# Sodium (Encryption)
pkg_check_modules(SODIUM REQUIRED libsodium)

# JSON for Modern C++
find_path(JSON_INCLUDE_DIRS "nlohmann/json.hpp"
    HINTS /usr/include /usr/local/include
)

# MLS++ (End-to-End encryption)
find_library(MLS_LIB mlspp HINTS /usr/local/lib)
find_path(MLS_INCLUDE_DIRS "mlspp/common.h"
    HINTS /usr/local/include /usr/include
)

# =========================================================
# INCLUDE PATHS
# =========================================================
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OPUS_INCLUDE_DIRS}
    ${SODIUM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${MLSPP_INCLUDE_DIRS}
)

link_directories(
    ${OPUS_LIBRARY_DIRS}
    ${SODIUM_LIBRARY_DIRS}
)

file(GLOB COMMAND_SOURCES "commands/*.cpp")

# =========================================================
# BOT EXECUTABLE
# =========================================================
add_executable(bot
    main.cpp
    memoryManager.cpp
    command_registry.cpp
    ${COMMAND_SOURCES}
)

target_link_libraries(bot
    PRIVATE
    dpp::dpp
    ${OPUS_LIBRARIES}
    ${SODIUM_LIBRARIES}
    ${MLSPP_LIB}
)

# =========================================================
# DEPLOY EXECUTABLE
# =========================================================
add_executable(deploy
    memoryManager.cpp
    deploy.cpp
    command_registry.cpp
    ${COMMAND_SOURCES}
)

target_link_libraries(deploy
    PRIVATE
    dpp::dpp
    ${OPUS_LIBRARIES}
    ${SODIUM_LIBRARIES}
    ${MLS_LIB}
)

# =========================================================
# OPTIONAL INSTALL
# =========================================================
# install(TARGETS bot deploy DESTINATION bin)

# =========================================================
# LOG INFO (for clarity)
# =========================================================
message(STATUS "DPP Found: ${dpp_DIR}")
message(STATUS "Opus Include: ${OPUS_INCLUDE_DIRS}")
message(STATUS "Sodium Include: ${SODIUM_INCLUDE_DIRS}")
message(STATUS "JSON Include: ${JSON_INCLUDE_DIRS}")
message(STATUS "MLS++ Include: ${MLS_INCLUDE_DIRS}")
message(STATUS "Linked MLS++: ${MLS_LIB}")

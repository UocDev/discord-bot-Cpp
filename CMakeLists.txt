cmake_minimum_required(VERSION 3.16)
project(discord_cpp_bot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER_LAUNCHER ccache)

include(FetchContent)

# =========================================================
# FETCH
# =========================================================
# nlohmann JSON
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${nlohmann_json_SOURCE_DIR}")

# DPP (Discord++)
FetchContent_Declare(
  dpp
  GIT_REPOSITORY https://github.com/brainboxdotcc/DPP.git
  GIT_TAG master
)
FetchContent_MakeAvailable(dpp)

# libsodium
FetchContent_Declare(
  sodium
  GIT_REPOSITORY https://github.com/jedisct1/libsodium.git
  GIT_TAG stable
)
FetchContent_MakeAvailable(sodium)

# libopus
FetchContent_Declare(
  opus
  GIT_REPOSITORY https://github.com/xiph/opus.git
  GIT_TAG v1.5.2
)
FetchContent_MakeAvailable(opus)

# MLS++
FetchContent_Declare(
  mlspp
  GIT_REPOSITORY https://github.com/cisco/mlspp.git
  GIT_TAG main
)
FetchContent_MakeAvailable(mlspp)

# =========================================================
# INCLUDE PATHS
# =========================================================
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${nlohmann_json_SOURCE_DIR}/include
  ${opus_SOURCE_DIR}/include
  ${sodium_SOURCE_DIR}/src/libsodium/include
  ${mlspp_SOURCE_DIR}/include
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
    nlohmann_json::nlohmann_json
    ${OPUS_LIBRARIES}
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
    nlohmann_json::nlohmann_json
    ${OPUS_LIBRARIES}
)

# =========================================================
# OPTIONAL INSTALL
# =========================================================
# install(TARGETS bot deploy DESTINATION bin)

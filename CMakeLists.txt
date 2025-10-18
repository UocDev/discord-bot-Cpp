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
find_package(dpp REQUIRED CONFIG)

# Coba cari libopus dengan pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(OPUS REQUIRED opus)

# =========================================================
# INCLUDE PATHS
# =========================================================
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OPUS_INCLUDE_DIRS}
)

link_directories(
    ${OPUS_LIBRARY_DIRS}
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
)

# =========================================================
# OPTIONAL INSTALL
# =========================================================
# install(TARGETS bot deploy DESTINATION bin)

cmake_minimum_required(VERSION 3.16)
project(discord_cpp_bot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(dpp REQUIRED CONFIG)

# Include folder
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

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

target_link_libraries(bot PRIVATE dpp::dpp)

# =========================================================
# DEPLOY EXECUTABLE
# =========================================================
add_executable(deploy
    memoryManager.cpp
    deploy.cpp
    command_registry.cpp
    ${COMMAND_SOURCES}
)

target_link_libraries(deploy PRIVATE dpp::dpp)

# =========================================================
# OPSIONAL
# =========================================================
# install(TARGETS bot deploy DESTINATION bin)

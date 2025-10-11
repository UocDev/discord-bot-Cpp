cmake_minimum_required(VERSION 3.16)
project(discord_cpp_bot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find D++ (dpp). Make sure dpp is installed on system or provide path.
find_package(dpp REQUIRED CONFIG)

# Sources
file(GLOB COMMAND_SOURCES "commands/*.cpp")
set(SRC
    main.cpp
    deploy.cpp
    command_registry.cpp
    ${COMMAND_SOURCES}
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(bot ${SRC})
# Link dpp once; both deploy and bot are in same binary (entrypoint chosen by main/deploy logic).
target_link_libraries(bot PRIVATE dpp::dpp)

# Optionally: install target to /usr/local/bin
# install(TARGETS bot DESTINATION bin)

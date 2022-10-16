cmake_minimum_required(VERSION 3.17) # FetchContent is available in 3.11+
project(NetStack_RTYPE)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH .)
include(FetchContent)

FetchContent_Declare(asio-cmake
  GIT_REPOSITORY https://github.com/kingsamchen/asio-cmake.git
  GIT_TAG        origin/master
)

# Specify asio version
set(ASIO_CMAKE_ASIO_TAG asio-1-24-0)
FetchContent_MakeAvailable(asio-cmake)


add_executable(${PROJECT_NAME} NetStack.cpp
                            NetStack.hpp)

include_directories (.
                    ./_deps/
                    )

target_link_libraries(${PROJECT_NAME} PRIVATE asio)
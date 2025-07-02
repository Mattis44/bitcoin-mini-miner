cmake_minimum_required(VERSION 3.14)
project(miner LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.h)

add_executable(miner ${SOURCES})
target_include_directories(miner PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(cpr CONFIG REQUIRED)

target_link_libraries(miner
  PRIVATE
    OpenSSL::Crypto
    nlohmann_json::nlohmann_json
    cpr::cpr
)

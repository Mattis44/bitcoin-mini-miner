cmake_minimum_required(VERSION 3.14)
project(blockchain LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.h)
add_executable(blockchain ${SOURCES})

target_include_directories(blockchain PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(nlohmann_json REQUIRED)
find_package(OpenSSL REQUIRED)

target_link_libraries(blockchain PRIVATE nlohmann_json::nlohmann_json OpenSSL::Crypto)

cmake_minimum_required(VERSION 3.12)
project(poc_some_asio_engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(Boost COMPONENTS program_options system thread REQUIRED)
find_package(GTest REQUIRED)

add_subdirectory(src/ConfigCommandLineParser)
add_subdirectory(src/ProtocolExample)
add_subdirectory(src/HttpServerExample)
add_subdirectory(tests/ConfigCommandLineParser)
add_subdirectory(tests/ProtocolExampleTest)

# Главный исполняемый файл в папке src
add_executable(poc_some_asio_engine src/main.cpp)

# Связываем главный исполняемый файл с библиотекой ConfigCommandLineParser
target_link_libraries(poc_some_asio_engine ConfigCommandLineParser HttpServerExample)

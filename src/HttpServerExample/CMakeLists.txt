# Явно указываем зависимости библиотеки ConfigCommandLineParser
add_library(HttpServerExample HttpServer.cpp HttpSession.cpp Worker.cpp)
target_include_directories(HttpServerExample PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(HttpServerExample PRIVATE Boost::boost ProtocolExample)
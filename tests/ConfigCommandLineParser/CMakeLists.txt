# Создать исполняемый файл для тестов
add_executable(ConfigCommandLineParserTest
        ConfigCommandLineParserTest.cpp)

target_include_directories(ConfigCommandLineParser PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(ConfigCommandLineParserTest
        GTest::GTest
        GTest::Main
        ConfigCommandLineParser)
add_test(ConfigCommandLineParserTest ConfigCommandLineParserTest)
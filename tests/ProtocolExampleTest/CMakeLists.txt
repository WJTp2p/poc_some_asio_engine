include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(ProtocolExampleTest
        ProtocolExampleTest.cpp)

target_include_directories(ProtocolExampleTest PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(ProtocolExampleTest
        GTest::GTest
        GTest::Main
        ProtocolExample)

add_test(ProtocolExampleTest ProtocolExampleTest)

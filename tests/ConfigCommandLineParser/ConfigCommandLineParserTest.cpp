#include "gtest/gtest.h"
#include "src/ConfigCommandLineParser/ConfigCommandLineParser.h"

#include <fstream>

namespace poc_some_asio_engine {

    TEST(ConfigCommandLineParserTest, ParseSuccess) {
        const char *argv[] = {"program", "--port=8081", "--io-threads=2", "--work-threads=4"};
        ConfigCommandLineParser parser;
        ParseResult result = parser.parse(4, argv);

        EXPECT_EQ(result, ParseResult::Success);
        EXPECT_EQ(parser.getPort(), 8081);
        EXPECT_EQ(parser.getIOThreads(), 2);
        EXPECT_EQ(parser.getWorkThreads(), 4);
    }

    TEST(ConfigCommandLineParserTest, ParseHelp) {
        const char *argv[] = {"program", "--help"};
        ConfigCommandLineParser parser;
        ParseResult result = parser.parse(2, argv);

        EXPECT_EQ(result, ParseResult::HelpRequested);
    }

    TEST(ConfigCommandLineParserTest, ParseError) {
        const char *argv[] = {"program", "--invalid-option"};
        ConfigCommandLineParser parser;
        ParseResult result = parser.parse(2, argv);

        EXPECT_EQ(result, ParseResult::Error);
    }

    TEST(ConfigCommandLineParserTest, ParseConfigFileSuccess) {
        // Создаем временный конфигурационный файл с ожидаемыми значениями
        std::string configFileName = std::filesystem::temp_directory_path() / "config.cfg";
        std::ofstream configFile(configFileName);
        configFile << "port=9090\nio-threads=3\nwork-threads=5";
        configFile.close();

        const std::string configParam = "--config-file=" + configFileName;
        const char *argv[] = {"program", "--port=8081", configParam.c_str()};
        ConfigCommandLineParser parser;
        ParseResult result = parser.parse(std::size(argv), argv);

        EXPECT_EQ(result, ParseResult::Success);
        EXPECT_EQ(parser.getPort(), 9090); // Значение из config.cfg должно переписать значение из командной строки
        EXPECT_EQ(parser.getIOThreads(), 3);
        EXPECT_EQ(parser.getWorkThreads(), 5);
    }

    TEST(ConfigCommandLineParserTest, ParseConfigFileNotFound) {
        const char *argv[] = {"program", "--config-file=nonexistent.cfg"};
        ConfigCommandLineParser parser;
        ParseResult result = parser.parse(2, argv);

        EXPECT_EQ(result, ParseResult::Error);
    }

    TEST(ConfigCommandLineParserTest, ParseConfigFileInvalid) {
        // Создаем временный конфигурационный файл с некорректным синтаксисом
        std::string configFileName = std::filesystem::temp_directory_path() / "invalid.cfg";
        std::ofstream configFile(configFileName);
        configFile << "invalid_syntax_here";
        configFile.close();

        const std::string configParam = "--config-file=" + configFileName;
        const char *argv[] = {"program", configParam.c_str()};
        ConfigCommandLineParser parser;
        ParseResult result = parser.parse(2, argv);

        EXPECT_EQ(result, ParseResult::Error);
    }
}  // namespace poc_some_asio_engine

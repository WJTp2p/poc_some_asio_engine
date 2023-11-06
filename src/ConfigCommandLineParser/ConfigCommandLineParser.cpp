#include "ConfigCommandLineParser.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <system_error>

namespace poc_some_asio_engine {

    namespace po = boost::program_options;
    namespace fs = std::filesystem;

    constexpr uint16_t DEFAULT_PORT = 8080;
    constexpr size_t DEFAULT_IO_THREADS = 1;
    constexpr size_t DEFAULT_WORK_THREADS = 1;

    ConfigCommandLineParser::ConfigCommandLineParser() noexcept
            : m_port(DEFAULT_PORT), m_ioThreads(DEFAULT_IO_THREADS), m_workThreads(DEFAULT_WORK_THREADS) {}

    ParseResult ConfigCommandLineParser::parse(int argc, const char* const argv[]) {
        // Загрузка параметров из командной строки
        po::options_description desc("Allowed options");
        desc.add_options()
                ("help", "Produce help message")
                ("port,p", po::value<uint16_t>(&m_port)->default_value(DEFAULT_PORT), "Port to listen on")
                ("io-threads,i", po::value<size_t>(&m_ioThreads)->default_value(DEFAULT_IO_THREADS),
                 "Number of IO threads")
                ("work-threads,w", po::value<size_t>(&m_workThreads)->default_value(DEFAULT_WORK_THREADS),
                 "Number of work threads")
                ("config-file,c", po::value<std::string>(), "Path to a config file. Values from the config file override command line values.");

        po::variables_map vm;
        try {
            po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
            po::notify(vm);
        } catch (const po::error &e) {
            std::cerr << "Error parsing command line: " << e.what() << std::endl;
            return ParseResult::Error;
        }

        if (vm.count("help")) {
            std::cout << desc << std::endl;
            return ParseResult::HelpRequested;
        }

        // Если указан параметр config-file, проверяем существование файла
        if (vm.count("config-file")) {
            m_configFile = vm["config-file"].as<std::string>();
            std::error_code ec;
            if (!fs::is_regular_file(m_configFile, ec)) {
                std::cerr << "Config file does not exist or is not a regular file: " << m_configFile << " ("
                          << ec.message() << ")" << std::endl;
                return ParseResult::Error;
            }

            // Загружаем параметры из конфиг-файла
            std::ifstream configStream(m_configFile);
            po::options_description configDesc;
            configDesc.add_options()
                    ("port,p", po::value<uint16_t>(&m_port)->default_value(DEFAULT_PORT), "Port to listen on")
                    ("io-threads,i", po::value<size_t>(&m_ioThreads)->default_value(DEFAULT_IO_THREADS),
                     "Number of IO threads")
                    ("work-threads,w", po::value<size_t>(&m_workThreads)->default_value(DEFAULT_WORK_THREADS),
                     "Number of work threads");
            po::variables_map configVM;
            try {
                po::store(po::parse_config_file(configStream, configDesc, true), configVM);
                po::notify(configVM);
            } catch (const po::error &e) {
                std::cerr << "Error parsing config file: " << e.what() << std::endl;
                return ParseResult::Error;
            }
        }

        return ParseResult::Success;
    }


    uint16_t ConfigCommandLineParser::getPort() const noexcept {
        return m_port;
    }

    size_t ConfigCommandLineParser::getIOThreads() const noexcept {
        return m_ioThreads;
    }

    size_t ConfigCommandLineParser::getWorkThreads() const noexcept {
        return m_workThreads;
    }

} // namespace poc_some_asio_engine

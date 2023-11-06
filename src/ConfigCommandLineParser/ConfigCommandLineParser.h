#pragma once

#include <boost/program_options.hpp>
#include <cstdint>

namespace poc_some_asio_engine {

    enum class ParseResult {
        Success,
        HelpRequested,
        Error
    };

    class ConfigCommandLineParser {
    public:
        ConfigCommandLineParser() noexcept;
        ParseResult parse(int argc, const char* const argv[]);

        uint16_t getPort() const noexcept;
        size_t getIOThreads() const noexcept;
        size_t getWorkThreads() const noexcept;

    private:
        uint16_t m_port;
        size_t m_ioThreads;
        size_t m_workThreads;
        std::string m_configFile;
    };

} // namespace poc_some_asio_engine

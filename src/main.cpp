#include <boost/asio.hpp>
#include <iostream>
#include <thread>

#include "src/ConfigCommandLineParser/ConfigCommandLineParser.h"
#include "src/HttpServerExample/HttpServer.h"

int main(int argc, char* argv[]) {
    poc_some_asio_engine::ConfigCommandLineParser cmdParser;

    if (cmdParser.parse(argc, argv) == poc_some_asio_engine::ParseResult::Error) {
        std::cerr << "Invalid command line arguments. Use --help for usage." << std::endl;
        return 1;
    }

    auto port = cmdParser.getPort();
    auto ioThreads = cmdParser.getIOThreads();
    auto workThreads = cmdParser.getWorkThreads();

    boost::asio::io_context io;
    poc_some_asio_engine::HttpServer server(io, port, ioThreads, workThreads);

    return 0;
}

#pragma once

#include "Worker.h"

#include <boost/asio.hpp>

namespace poc_some_asio_engine {

    class HttpSession;

    class HttpServer {
    public:
        HttpServer(boost::asio::io_context &ioContext, uint16_t port, size_t ioThreads, size_t workThreads);

    private:
        void doAccept();

    private:
        boost::asio::ip::tcp::acceptor m_acceptor;
        Worker m_worker;
    };

}  // namespace poc_some_asio_engine
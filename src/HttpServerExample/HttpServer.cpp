#include "HttpServer.h"
#include "HttpSession.h"

namespace asio = boost::asio;

namespace poc_some_asio_engine {
    HttpServer::HttpServer(boost::asio::io_context &ioContext, uint16_t port, size_t ioThreads, size_t workThreads)
            : m_acceptor(ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
              m_worker(ioContext, ioThreads, workThreads) {
        doAccept();
    }

    void HttpServer::doAccept() {
        m_acceptor.async_accept(
                [this](const auto &ec, auto socket) {
                    if (!ec) {
                        std::make_shared<HttpSession>(std::move(socket), m_worker)->start();
                    }

                    doAccept();
                }
        );
    }

} // poc_some_asio_engine
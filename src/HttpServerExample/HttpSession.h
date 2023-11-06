#pragma once

#include <memory>

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/http.hpp>

namespace poc_some_asio_engine {
    class Worker;

    class HttpSession : public std::enable_shared_from_this<HttpSession> {
    public:
        HttpSession(boost::asio::ip::tcp::socket socket, Worker &worker);
        HttpSession(const HttpSession&) = delete;
        HttpSession& operator=(const HttpSession) = delete;

        void start();

    private:
        void readRequest();
        void processRequest();

    private:
        boost::asio::ip::tcp::socket m_socket;
        Worker &m_worker;
        boost::beast::flat_buffer m_buffer;
        boost::beast::http::request<boost::beast::http::string_body> m_request;
    };

} // poc_some_asio_engine

#include "HttpSession.h"
#include "Worker.h"

#include "src/ProtocolExample/ProtocolExample.h"

#include <iostream>

namespace poc_some_asio_engine {
    HttpSession::HttpSession(boost::asio::ip::tcp::socket socket, Worker &worker) :
            m_socket(std::move(socket)),
            m_worker(worker) {
    }

    void HttpSession::start() {
        readRequest();
    }

    void HttpSession::readRequest() {
        auto self = shared_from_this();
        boost::beast::http::async_read(
                m_socket,
                m_buffer,
                m_request,
                [self, this](auto ec, auto bytes_transferred) {
                    if (!ec) {
                        processRequest();
                    }
                });
    }

    void HttpSession::processRequest() {
        // Парсинг JSON-запроса и обработка с помощью ProtocolExample
        std::string requestJson = m_request.body();
        std::variant<ProtocolError, Request> result = ProtocolExample::parseRequest(requestJson);

        // Выполнение работы в рабочем потоке с использованием Worker
        if (std::holds_alternative<Request>(result)) {
            Request request = std::get<Request>(result);
            m_worker.processRequest(request);
        } else {
            // Ошибка парсинга запроса
            ProtocolError error = std::get<ProtocolError>(result);
            std::cerr << "Error processing request: " << static_cast<int>(error.type) << " - " << error.message << std::endl;
        }
    }
} // poc_some_asio_engine
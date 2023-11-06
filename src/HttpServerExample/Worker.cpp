#include "Worker.h"

#include <boost/core/ignore_unused.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/system/error_code.hpp>

#include <iostream>

namespace poc_some_asio_engine {
    Worker::Worker(boost::asio::io_context &ioContext, size_t ioThreads, size_t workThreads) :
            m_ioContext(ioContext),
            m_workThreads(workThreads) {
        startIoThreads(ioThreads);
    }

    void Worker::processRequest(const Request &request) {
        boost::ignore_unused(request);
        // Пример обработки: пока только выводим данные
        boost::asio::post(m_workThreads, [this] {
            std::lock_guard m_lock{m_mutex};
            std::cout << "Received request";
        });
    }

    void Worker::startIoThreads(size_t ioThreads) {
        for (size_t i = 0; i < ioThreads; ++i) {
            m_IoThreads.emplace_back([this] {
                boost::system::error_code ec;
                m_ioContext.run(ec);
                std::lock_guard m_lock{m_mutex};
                std::cerr << "finished thread" << std::this_thread::get_id();
            });
        }
    }
} // poc_some_asio_engine
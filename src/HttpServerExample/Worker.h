#pragma once

#include <boost/asio.hpp>

#include <thread>

namespace poc_some_asio_engine {
    struct Request;

    class Worker {
    public:
        Worker(boost::asio::io_context &ioContext, size_t ioThreads, size_t workThreads);

        void processRequest(const Request &request);

    private:
        void startIoThreads(size_t ioThreads);

        void runWorkerThread();

        boost::asio::io_context &m_ioContext;
        std::vector<std::thread> m_IoThreads;
        boost::asio::thread_pool m_workThreads;
        std::mutex m_mutex;
    };
} // poc_some_asio_engine

#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <optional>

namespace poc_some_asio_engine {

    struct Request {
        std::string method;
        std::optional<nlohmann::json> params;
        std::string token;
    };

    struct Response {
        std::optional<nlohmann::json> result;
        std::optional<nlohmann::json> resultExtraInfo;
    };

    enum class ProtocolErrorType {
        ParsingError,
        SerializationError,
        ParseRequestError,
        GenerateResponseError
    };

    struct ProtocolError {
        ProtocolErrorType type;
        std::string message;
    };

    class ProtocolExample {
    public:
        ProtocolExample() noexcept;

        static std::variant<ProtocolError, Request> parseRequest(const std::string& requestJson) noexcept;
        static std::variant<ProtocolError, std::string> generateResponse(const Response& response) noexcept;
    };
} // namespace poc_some_asio_engine

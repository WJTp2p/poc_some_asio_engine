#include "ProtocolExample.h"
#include <exception>

namespace poc_some_asio_engine {

    ProtocolExample::ProtocolExample() noexcept {}

    std::variant<ProtocolError, Request> ProtocolExample::parseRequest(const std::string& requestJson) noexcept {
        try {
            nlohmann::json jsonData = nlohmann::json::parse(requestJson);

            if (!jsonData.contains("method") || !jsonData.contains("token")) {
                return ProtocolError{ProtocolErrorType::ParseRequestError, "Request is missing required fields"};
            }

            Request request;
            request.method = jsonData["method"];
            request.token = jsonData["token"];

            if (jsonData.contains("params")) {
                request.params = jsonData["params"];
            }

            return request;
        } catch (const std::exception& e) {
            return ProtocolError{ProtocolErrorType::ParseRequestError, e.what()};
        }
    }

    std::variant<ProtocolError, std::string> ProtocolExample::generateResponse(const Response& response) noexcept {
        try {
            nlohmann::json jsonResponse;
            if (response.result.has_value()) {
                if (auto& value = response.result.value(); value.is_object()) {
                    jsonResponse["result"] = value;
                } else {
                    return ProtocolError{ProtocolErrorType::GenerateResponseError, "result is not JSON object"};
                }
            }

            if (response.resultExtraInfo.has_value()) {
                if (auto& value = response.resultExtraInfo.value(); value.is_object()) {
                    jsonResponse["resultExtraInfo"] = value;
                } else {
                    return ProtocolError{ProtocolErrorType::GenerateResponseError, "resultExtraInfo is not JSON object"};
                }
            }

            return jsonResponse.dump();
        } catch (const std::exception& e) {
            return ProtocolError{ProtocolErrorType::GenerateResponseError, e.what()};
        }
    }
} // namespace poc_some_asio_engine

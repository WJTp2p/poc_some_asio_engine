#include "gtest/gtest.h"
#include "src/ProtocolExample/ProtocolExample.h"

namespace poc_some_asio_engine {

    TEST(ProtocolExampleTest, ParseRequestSuccess) {
        std::string requestJson = R"({"method": "sampleMethod", "params": {"param1": 42}, "token": "token123"})";
        auto result = ProtocolExample::parseRequest(requestJson);
        ASSERT_TRUE(std::holds_alternative<Request>(result));

        Request request = std::get<Request>(result);
        ASSERT_EQ(request.method, "sampleMethod");
        ASSERT_TRUE(request.params.has_value());
        ASSERT_EQ(request.params->at("param1"), 42);
        ASSERT_EQ(request.token, "token123");
    }

    TEST(ProtocolExampleTest, ParseRequestWithMissingParams) {
        std::string requestJson = R"({"method": "sampleMethod", "token": "token123"})";
        auto result = ProtocolExample::parseRequest(requestJson);
        ASSERT_TRUE(std::holds_alternative<Request>(result));

        Request request = std::get<Request>(result);
        ASSERT_EQ(request.method, "sampleMethod");
        ASSERT_FALSE(request.params.has_value());
        ASSERT_EQ(request.token, "token123");
    }

    TEST(ProtocolExampleTest, ParseRequestWithInvalidJSON) {
        std::string requestJson = "Invalid JSON";
        auto result = ProtocolExample::parseRequest(requestJson);
        ASSERT_TRUE(std::holds_alternative<ProtocolError>(result));

        ProtocolError error = std::get<ProtocolError>(result);
        ASSERT_EQ(error.type, ProtocolErrorType::ParseRequestError);
        ASSERT_EQ(error.message, R"([json.exception.parse_error.101] parse error at line 1, column 1: syntax error while parsing value - invalid literal; last read: 'I')");
    }

    TEST(ProtocolExampleTest, GenerateResponseSuccess) {
        Response response;
        response.result.emplace()["key"] = "value";
        response.resultExtraInfo.emplace()["info"] = "extra info";

        auto result = ProtocolExample::generateResponse(response);
        ASSERT_TRUE(std::holds_alternative<std::string>(result));

        std::string jsonResponse = std::get<std::string>(result);
        ASSERT_EQ(jsonResponse, "{\"result\":{\"key\":\"value\"},\"resultExtraInfo\":{\"info\":\"extra info\"}}");
    }

    TEST(ProtocolExampleTest, GenerateResponseWithMissingExtraInfo) {
        Response response;
        response.result.emplace()["key"] = "value";
        response.resultExtraInfo.reset();

        auto result = ProtocolExample::generateResponse(response);
        ASSERT_TRUE(std::holds_alternative<std::string>(result));

        std::string jsonResponse = std::get<std::string>(result);
        ASSERT_EQ(jsonResponse, "{\"result\":{\"key\":\"value\"}}");
    }

    TEST(ProtocolExampleTest, GenerateResponseWithInvalidJSON) {
        Response response;
        response.result = "Invalid JSON";
        response.resultExtraInfo.reset();

        auto result = ProtocolExample::generateResponse(response);
        ASSERT_TRUE(std::holds_alternative<ProtocolError>(result));

        ProtocolError error = std::get<ProtocolError>(result);
        ASSERT_EQ(error.type, ProtocolErrorType::GenerateResponseError);
        ASSERT_EQ(error.message, "result is not JSON object");
    }
}

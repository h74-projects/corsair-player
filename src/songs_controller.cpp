#include "songs_controller.hpp"

namespace m_player{

SongsController::SongsController() 
:m_client(U("http://localhost:5000")) 
{}

void SongsController::get_songs_urls(std::vector<std::string>& a_result, const std::vector<std::string>& queries) {
    for (const auto& query : queries) {
        web::http::uri_builder builder(U("/search"));
        builder.append_query(U("q"), web::uri::encode_data_string(query));

        web::http::http_request request(web::http::methods::POST);
        request.headers().set_content_type(U("application/json"));
        request.set_request_uri(builder.to_uri());

        web::json::value json_data;
        json_data[U("q")] = web::json::value::array({ web::json::value::string(utility::conversions::to_string_t(query)) });

        request.set_body(json_data);

        auto result_task = m_client.request(request)
            .then([](web::http::http_response response) {
                if (response.status_code() == web::http::status_codes::OK) {
                    return response.extract_json();
                } else {
                    throw std::runtime_error("HTTP request failed with status code: " + std::to_string(response.status_code()));
                }
            });

        auto json_response = result_task.get();
        if (json_response.has_field(U("video_links"))) {
            web::json::array video_links = json_response[U("video_links")].as_array();
            for (const auto& link : video_links) {
                if (link.is_string()) {
                    a_result.push_back(utility::conversions::to_utf8string(link.as_string()));
                }
            }
        }
    }
}














std::string SongsController::get_songs_list(const std::string& message){
    web::http::uri_builder builder(U("/chat"));

    web::http::http_request request(web::http::methods::POST);
    request.set_request_uri(builder.to_uri());
    request.headers().set_content_type(U("application/json"));

    web::json::value messageJson;
    messageJson[U("message")] = web::json::value::string(utility::conversions::to_string_t(message));
    request.set_body(messageJson);

    auto response_task = m_client.request(request);

    auto response = response_task.get();
    if (response.status_code() == web::http::status_codes::OK)
    {
        return utility::conversions::to_utf8string(response.to_string());
    }
    else
    {
        throw std::runtime_error("HTTP request failed with status code: " + std::to_string(response.status_code()));
    }
}

}//namespace m_player
#include "songs_controller.hpp"

namespace m_player{

SongsController::SongsController() 
:m_client(U("http://localhost:5000")) 
{}

std::vector<std::string> SongsController::get_songs_urls(const std::vector<std::string>& queries) {
    std::vector<std::string> response_strings;
    
    for (const auto& query : queries) {
        web::http::uri_builder builder(U("/search"));
        builder.append_query(U("q"), query);

        web::http::http_request request(web::http::methods::GET);
        request.set_request_uri(builder.to_uri());

        auto result_task = m_client.request(request)
            .then([](web::http::http_response response) {
                if (response.status_code() == web::http::status_codes::OK) {
                    return response.to_string();
                } else {
                    throw std::runtime_error("HTTP request failed with status code: " + std::to_string(response.status_code()));
                }
            });

        utility::string_t response_str = result_task.get();
        response_strings.push_back(utility::conversions::to_utf8string(response_str));
    }

    return response_strings;
}


std::string SongsController::get_song_list(const std::string& message){
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
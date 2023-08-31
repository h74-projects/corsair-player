#include "songs_controller.hpp"

namespace m_player{

SongsController::SongsController() 
:m_client(U("http://localhost:5000")) 
{}

void SongsController::get_songs_urls(std::vector<std::string>& a_result, std::vector<Song>& queries,SqlMng& a_sql) {
    auto it = queries.begin();
    while (it != queries.end()) {
        web::http::uri_builder builder(U("/search"));
        builder.append_query(U("q"), web::uri::encode_data_string(it->get_search_name()));

        web::http::http_request request(web::http::methods::POST);
        request.headers().set_content_type(U("application/json"));
        request.set_request_uri(builder.to_uri());

        web::json::value json_data;
        json_data[U("q")] = web::json::value::array({ web::json::value::string(utility::conversions::to_string_t(it->get_search_name())) });

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
                    std::string url = utility::conversions::to_utf8string(link.as_string());
                    std::cout << url << std::endl;
                    if(!url.empty()){
                        a_result.push_back(utility::conversions::to_utf8string(link.as_string()));
                        ++it;
                    }else{
                        a_sql.delete_song(*it);
                        it =queries.erase(it);
                    }
                }else{
                    a_sql.delete_song(*it);
                    it = queries.erase(it);
                }
            }
        }else{
            a_sql.delete_song(*it);
            it = queries.erase(it);
        }
    }
}


void SongsController::get_song_url(std::string& song_url, const std::string& query) {
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
        if (video_links[0].is_string()) {
            song_url = utility::conversions::to_utf8string(video_links[0].as_string());
        }
    }
}


std::string SongsController::get_songs_list(const std::string& message) {

    // Construct the JSON message
    web::json::value request;
    std::string message_n_rules = message + " print result in following parsing: songs_name|artists|genere|year. dont add any other text";
    request[U("message")] = web::json::value::string(message_n_rules);

    // Create the HTTP request
    web::http::http_request req(web::http::methods::POST);
    req.headers().set_content_type(U("application/json"));
    req.set_request_uri(U("/chat"));
    req.set_body(request);

    // Perform the request
    web::http::http_response response = m_client.request(req).get();
    if (response.status_code() == web::http::status_codes::OK) {
        web::json::value jsonResponse = response.extract_json().get();
        if (jsonResponse.has_field(U("response"))) {
            return jsonResponse[U("response")].as_string();
        } else {
            throw std::runtime_error("Response field 'response' not found in JSON.");
        }
    } else {
        throw std::runtime_error("HTTP request failed.");
    }
}


std::string SongsController::get_lyrics(const std::string& a_song_name) {
    // Construct the request URI
    web::http::uri_builder builder(U("/lyrics"));
    builder.append_query(U("query"), a_song_name);

    // Create the HTTP request
    web::http::http_request req(web::http::methods::GET);
    req.headers().set_content_type(U("application/json"));
    req.set_request_uri(builder.to_string());

    // Perform the request
    web::http::http_response response = m_client.request(req).get();
    if (response.status_code() == web::http::status_codes::OK) {
        web::json::value jsonResponse = response.extract_json().get();
        if (jsonResponse.has_field(U("lyrics"))) {
           std::string lyrics = jsonResponse[U("lyrics")].as_string();
            if(!lyrics.empty()){
                return lyrics;
            }else{
                return "Sorry - Lyrics not found";
            }
        }else{
            throw std::runtime_error("Response field 'lyrics' not found in JSON.");
        }
    } else {
        throw std::runtime_error("HTTP request failed.");
    }
}


}//namespace m_player
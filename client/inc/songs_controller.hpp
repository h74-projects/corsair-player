#ifndef SONGS_CONTROLLER_HPP
#define SONGS_CONTROLLER_HPP

#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <string>
#include <vector>

namespace m_player {

class SongsController {
public:
    SongsController();
    ~SongsController() = default;

    void get_songs_urls(std::vector<std::string>& a_result, const std::vector<std::string>& queries);
    void get_song_url(std::string& song_url, const std::string& query);
    std::string get_songs_list(const std::string& message);

private:
    web::http::client::http_client m_client;
};

} // namespace m_player

#endif // SONGS_CONTROLLER_HPP

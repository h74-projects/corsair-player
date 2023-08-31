#ifndef SONGS_CONTROLLER_HPP
#define SONGS_CONTROLLER_HPP
#include "song.hpp"
#include "sql_mng.hpp"

#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <string>
#include <vector>

namespace m_player {

class SongsController {
public:
    SongsController();
    ~SongsController() = default;

    void get_songs_urls(std::vector<std::string>& a_result,std::vector<Song>& queries ,SqlMng& a_sql);
    void get_song_url(std::string& a_song_url, const std::string& a_query);
    std::string get_songs_list(const std::string& a_songs_request);
    std::string get_lyrics(const std::string& a_song_name);


private:
    web::http::client::http_client m_client;
};

} // namespace m_player

#endif // SONGS_CONTROLLER_HPP

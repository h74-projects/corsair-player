#ifndef EXECUTER_HPP
#define EXECUTER_HPP
#include <string>
#include <vector>

namespace m_player{

class Executer{

public:
    Executer();
    ~Executer() = default;

    void New_play_list(std::string a_playlist, std::string a_playlist_name);

private:
    void parse_songs(const std::string& songs_string, std::vector<Song>& songs);
    void get_url(std::vector<Song> play_list_vec, std::vector<std::string>url_vec);
    void sent_to_db(std::vector<std::pair<Song, std::string>> a_songs, std::string a_name);
    void down_songs(std::vector<Song> a_songs, std::vector<std::string> a_urls);

private:
    std::string m_url;
    SqlMng m_db;

};
}//namespace m_player

#endif //EXECUTER_HPP
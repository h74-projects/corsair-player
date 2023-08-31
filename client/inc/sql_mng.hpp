#ifndef SQL_MNG_HPP
#define SQL_MNG_HPP

#include <sqlite3.h>
#include <iostream>
#include "song.hpp"

#include <vector>

namespace m_player{

class SqlMng{
public:
explicit SqlMng(const std::string&);
~SqlMng();

void drop_tables();
void delete_song(const Song& song);


bool execute_statment(const std::string&);

void add_song_to_list(const Song& a_song);
bool add_song_to_playlist(Song a_song , std::string a_playlist);
void add_playlist(std::string a_playlist);

int get_song_id_by_name(const std::string& songName);
int get_playlist_id_by_name(const std::string& playlistName);
void get_songs_from_playlist(const std::string& playlistName, std::vector<Song>& songs);
void get_songs_by_condition(std::vector<Song>& a_found_songs, const std::string& artist = nullptr, const std::string& genre = nullptr, int year = 0);


bool is_song_exists(const Song& a_song);
bool is_playlist_exists(std::string a_playlist);

void add_lyrics(std::pair<Song , std::string> lyrics_pair);
void get_playlists(std::vector<std::string>& a_playlists_res);

void print_songs();
void print_song_of_playlists();
void print_playlist_table();


private:
    sqlite3* m_db;
    std::string m_db_name;
};

}//namespace m_player

#endif //SQL_MNG_HPP
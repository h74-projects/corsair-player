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

bool execute_statment(const std::string&);
void add_song_to_table(const std::string& table_name, const Song& a_song);
void remove_all_songs_from_table(const std::string& a_table_name);
void print_all_songs_at_table(const std::string& a_table_name);
bool is_song_exists(const Song& a_song , std::string a_table);

//create playlist
void create_table(const std::string& a_name);
//delete playlist
void drop_table_by_name(const std::string& a_name);
void get_songs_by_condition(std::vector<Song>& a_found_songs, const std::string& artist = nullptr, const std::string& genre = nullptr, int year = 0);



void load_songs(std::unordered_map<std::string, Song>&, std::unordered_map<std::string, std::vector<Song>>&);
void drop_songs_table();








private:
    sqlite3* m_db;
    std::string m_db_name;
};

}//namespace m_player

#endif //SQL_MNG_HPP
#ifndef SQL_MNG_HPP
#define SQL_MNG_HPP

#include <sqlite3.h>
#include <iostream>

namespace m_player{

class SqlMng{
public:

explicit SqlMng(const std::string& a_db_name);
~SqlMng();

bool execute_statment(const std::string&);
void add_song(const std::string& a_song_name , const std::string& a_artist_name);
void remove_all_songs();
void print_all_songs();


private:
    sqlite3* m_db;
    std::string m_db_name;
};

}//namespace m_player

#endif //SQL_MNG_HPP
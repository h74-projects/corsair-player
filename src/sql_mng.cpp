#include "sql_mng.hpp"

namespace m_player{

SqlMng::SqlMng(const std::string& a_db_name)
{
    m_db_name =a_db_name;
    if(sqlite3_open("mydatabase.db", &m_db)){
        std::cerr << "Cannot open database: " << sqlite3_errmsg(m_db) << std::endl;
        return;
    }
    std::string createSongsTable = "CREATE TABLE IF NOT EXISTS songs ("
                                 "song_id INTEGER PRIMARY KEY,"
                                 "song_name TEXT NOT NULL,"
                                 "artist TEXT NOT NULL"
                                 ");";
    execute_statment(createSongsTable);
}

SqlMng::~SqlMng(){
    sqlite3_close(m_db);
}


void SqlMng::add_song(const std::string& a_song_name, const std::string& a_artist_name) {
    std::string insertSongSQL = "INSERT INTO songs (song_name, artist) "
                                "SELECT ?, ? "
                                "WHERE NOT EXISTS (SELECT 1 FROM songs WHERE song_name = ? AND artist = ?);";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, insertSongSQL.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing insert statement." << std::endl;
        return;
    }
    sqlite3_bind_text(stmt, 1, a_song_name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, a_artist_name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, a_song_name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, a_artist_name.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Insert failed with error code: " << rc << std::endl;
    }

    sqlite3_finalize(stmt);
}


void SqlMng::print_all_songs(){
    std::string retrieveSongsSQL = "SELECT song_name, artist FROM songs;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, retrieveSongsSQL.c_str(), -1, &stmt, nullptr);

    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* songName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            const char* artistName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            std::cout << "Song:" << songName << " - Artist:" << artistName << std::endl;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error retrieving songs: " << sqlite3_errmsg(m_db) << std::endl;
    }
}

void SqlMng::remove_all_songs(){
    std::string deleteAllSQL = "DELETE FROM songs;";
    char* error_message = nullptr;

    int rc = sqlite3_exec(m_db, deleteAllSQL.c_str(), nullptr, nullptr, &error_message);

    if (rc != SQLITE_OK) {
        std::cerr << "Error: " << error_message << std::endl;
        sqlite3_free(error_message);
    } else {
        std::cout << "All songs removed" << std::endl;
    }
}


bool SqlMng::execute_statment(const std::string& a_statment){
    char* errMsg = nullptr;
    if (sqlite3_exec(m_db, a_statment.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Error creating table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}


} //namespace m_player
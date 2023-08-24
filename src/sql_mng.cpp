#include "sql_mng.hpp"
#include <cctype> 

namespace m_player{

SqlMng::SqlMng(const std::string& a_db_name)
{
    m_db_name =a_db_name;
    if(sqlite3_open("mydatabase.db", &m_db)){
        std::cerr << "Cannot open database: " << sqlite3_errmsg(m_db) << std::endl;
        return;
    }
    std::string downloads_table = "CREATE TABLE IF NOT EXISTS downloads ("
                                "song_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                "song_name TEXT NOT NULL,"
                                "artist TEXT NOT NULL,"
                                "year INTEGER,"
                                "genre TEXT,"
                                "duration_min INTEGER,"
                                "duration_sec INTEGER"
                                ");";
    execute_statment(downloads_table);

    std::string playlists_table="CREATE TABLE IF NOT EXISTS playlists ("
                                 "song_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                 "playlist_name TEXT NOT NULL"
                                 ");";
    execute_statment(playlists_table);
}

SqlMng::~SqlMng(){
    sqlite3_close(m_db);
}

//Create playlist and insert name into playlists
void SqlMng::create_table(const std::string& a_name){
    std::string createTableSQL = "CREATE TABLE IF NOT EXISTS " + a_name + " ("
                                "song_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                 "song_name TEXT NOT NULL,"
                                 "artist TEXT NOT NULL,"
                                 "year INTEGER,"
                                 "genre TEXT,"
                                 "duration_min INTEGER,"
                                 "duration_sec INTEGER"
                                 ");";

    if (execute_statment(createTableSQL)) {
        std::cout << "Table created successfully: " << a_name << std::endl;

        // Add the playlist name to the playlists table
        std::string addToPlaylistsSQL = "INSERT INTO playlists playlist_name VALUES (?);";
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(m_db, addToPlaylistsSQL.c_str(), -1, &stmt, nullptr);

        if (rc != SQLITE_OK) {
            std::cerr << "Error preparing insert statement." << std::endl;
            return;
        }

        sqlite3_bind_text(stmt, 1, a_name.c_str(), -1, SQLITE_STATIC);

        rc = sqlite3_step(stmt);

        if (rc != SQLITE_DONE) {
            std::cerr << "Insert failed with error code: " << rc << std::endl;
        }

        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to create table: " << a_name << std::endl;
    }

}

//Delete table
void SqlMng::drop_table_by_name(const std::string& a_name) {
    std::string dropTableSQL = "DROP TABLE IF EXISTS " + a_name;

    int rc = sqlite3_exec(m_db, dropTableSQL.c_str(), nullptr, nullptr, nullptr);

    if (rc == SQLITE_OK) {
        std::cout << "Table dropped successfully: " << a_name << std::endl;
    } else {
        std::cerr << "Error dropping table: " << sqlite3_errmsg(m_db) << std::endl;
    }
}



void SqlMng::add_song_to_table(const std::string& table_name, const Song& a_song) {
    if(is_song_exists(a_song , table_name)){
        return;
    }

    if(table_name != "downloads"){
        if(is_song_exists(a_song , "downloads")){
            add_song_to_table("downloads" , a_song);
        }
    }
    std::string insertSongSQL = "INSERT INTO " + table_name + " ("
                               "song_name, artist, year, genre, duration_min, duration_sec"
                               ") "
                               "VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, insertSongSQL.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing insert statement." << std::endl;
        return;
    }

    std::string songNameUtf8 = a_song.get_song_name();
    std::string artistUtf8 = a_song.get_artist_name();
    std::string genreUtf8 = a_song.get_genre();

    sqlite3_bind_text(stmt, 1, songNameUtf8.c_str(), static_cast<int>(songNameUtf8.size()), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, artistUtf8.c_str(), static_cast<int>(artistUtf8.size()), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, a_song.get_year());
    sqlite3_bind_text(stmt, 4, genreUtf8.c_str(), static_cast<int>(genreUtf8.size()), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, a_song.get_duration_minutes());
    sqlite3_bind_int(stmt, 6, a_song.get_duration_seconds());

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Insert failed with error code: " << rc << std::endl;
    }

    sqlite3_finalize(stmt);
}



void SqlMng::print_all_songs_at_table(const std::string& a_table_name){
    std::string retrieveSongsSQL = "SELECT song_name, artist FROM " + a_table_name;
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

bool SqlMng::is_song_exists(const Song& a_song , std::string a_table) {
     std::string query = "SELECT * FROM " + a_table + " WHERE "
                        "song_name = ? AND artist = ? AND year = ? AND genre = ? AND duration_min = ? AND duration_sec = ?;";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing query." << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, a_song.get_song_name().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, a_song.get_artist_name().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, a_song.get_year());
    sqlite3_bind_text(stmt, 4, a_song.get_genre().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, a_song.get_duration_minutes());
    sqlite3_bind_int(stmt, 6, a_song.get_duration_seconds());

    bool exists = (sqlite3_step(stmt) == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return exists;
}

void SqlMng::remove_all_songs_from_table(const std::string& a_table_name){
    std::string deleteAllSQL = "DELETE FROM " + a_table_name;
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
void SqlMng::get_songs_by_condition(std::vector<Song>& a_found_songs, const std::string& artist, const std::string& genre, int year) {    


    if(artist.empty() && genre.empty() && year == 0){
        return ;
    }

    std::string query = "SELECT * FROM downloads WHERE ";
    std::string condition;

    if (!artist.empty()) {
        condition += "artist LIKE '%" + artist + "%'";
    }

    if (!genre.empty()) {
        if (!condition.empty()) {
            condition += " AND ";
        }
        condition += "genre LIKE '%" + genre + "%'";
    }

    if (year != 0) {
        if (!condition.empty()) {
            condition += " AND ";
        }
        condition += "year = " + std::to_string(year);
    }
    query += condition;
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr);

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string song_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string song_artists = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        int year = sqlite3_column_int(stmt, 3);
        std::string genre = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        int duration_min = sqlite3_column_int(stmt, 5);
        int duration_sec = sqlite3_column_int(stmt, 6);

        Song song(id ,song_name, song_artists, year, genre, duration_min, duration_sec);
        a_found_songs.emplace_back(song);
    }
    if (rc != SQLITE_DONE) {
        std::cerr << "Error while retrieving data: " << sqlite3_errmsg(m_db) << std::endl;
    }
    sqlite3_finalize(stmt);
}



// void SqlMng::load_songs(std::unordered_map<std::string, Song>& a_songs,std::unordered_map<std::string, std::vector<Song>>& a_artist_songs) {
//     std::string query = "SELECT * FROM songs;";
//     sqlite3_stmt* stmt;
//     int rc = 0 ;
//     rc = sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr);

//     if (rc != SQLITE_OK) {
//         std::cerr << "Failed to prepare query: " << sqlite3_errmsg(m_db) << std::endl;
//         sqlite3_close(m_db);
//         return;
//     }

//     while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
//         int id = sqlite3_column_int(stmt, 0);
//         std::string song_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
//         std::string song_artists = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

//         Song song(id , song_name ,song_artists,0,0);

//         a_songs.emplace(song_name, song);
//         // Populate the artist map
//         a_artist_songs[song_artists].push_back(song);
//     }

//     if (rc != SQLITE_DONE) {
//         std::cerr << "Error while retrieving data: " << sqlite3_errmsg(m_db) << std::endl;
//     }

//     sqlite3_finalize(stmt);
// }


} //namespace m_player
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

    std::string create_songs_sable = "CREATE TABLE IF NOT EXISTS songs ("
                                   "song_id INTEGER PRIMARY KEY,"
                                   "song_name TEXT,"
                                   "artist TEXT,"
                                   "year INTEGER,"
                                   "genre TEXT,"
                                   "lyrics TEXT);";
    execute_statment(create_songs_sable);
    
    std::string createPlaylistTable = "CREATE TABLE IF NOT EXISTS playlist ("
                                      "playlist_id INTEGER PRIMARY KEY,"
                                      "playlist_name TEXT);";
    execute_statment(createPlaylistTable);

    std::string createPlaylistSongsTable = "CREATE TABLE IF NOT EXISTS songs_of_playlist ("
                                       "playlist_id INTEGER,"
                                       "song_id INTEGER,"
                                       "FOREIGN KEY (playlist_id) REFERENCES playlist(playlist_id)," 
                                       "FOREIGN KEY (song_id) REFERENCES songs(song_id));"; 

    execute_statment(createPlaylistSongsTable);
}

SqlMng::~SqlMng(){
    sqlite3_close(m_db);
}

void SqlMng::print_songs(){
     const char* sql = "SELECT * FROM songs;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(m_db) << std::endl;
        sqlite3_close(m_db);
        return;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int song_id = sqlite3_column_int(stmt, 0);
        const unsigned char* song_name = sqlite3_column_text(stmt, 1);
        const unsigned char* artist = sqlite3_column_text(stmt, 2);
        int year = sqlite3_column_int(stmt, 3);
        const unsigned char* genre = sqlite3_column_text(stmt, 4);
        const unsigned char* lyrics = sqlite3_column_text(stmt, 5);

        std::cout << "Song ID: " << song_id << std::endl;
        std::cout << "Song Name: " << song_name << std::endl;
        std::cout << "Artist: " << artist << std::endl;
        std::cout << "Year: " << year << std::endl;
        std::cout << "Genre: " << genre << std::endl;
        std::cout << "Lyrics: " << (lyrics[0] == '\0') << std::endl;
        std::cout << std::endl;
    }

    if (rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << sqlite3_errmsg(m_db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

void SqlMng::print_song_of_playlists(){
    
    std::string sql = "SELECT * FROM songs_of_playlist;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(m_db) << std::endl;
        sqlite3_close(m_db);
        return;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int playlist_id = sqlite3_column_int(stmt, 0);
        int song_id = sqlite3_column_int(stmt, 1);

        std::cout << "Playlist ID: " << playlist_id << std::endl;
        std::cout << "Song ID: " << song_id << std::endl;
        std::cout << std::endl;
    }

    if (rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << sqlite3_errmsg(m_db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

void SqlMng::print_playlist_table(){
    std::string sql = "SELECT * FROM playlist;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(m_db) << std::endl;
        sqlite3_close(m_db);
        return;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int playlist_id = sqlite3_column_int(stmt, 0);
        const unsigned char* playlist_name = sqlite3_column_text(stmt, 1);

        std::cout << "Playlist ID: " << playlist_id << std::endl;
        std::cout << "Playlist Name: " << playlist_name << std::endl;
        std::cout << std::endl;
    }

    if (rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << sqlite3_errmsg(m_db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

void SqlMng::add_song_to_list(const Song& a_song)
{
    if(is_song_exists(a_song)){
        return;
    }
    std::string add_song_query = "INSERT INTO songs "
                     "(song_name, artist, year, genre, lyrics) "
                     "VALUES (?, ?, ?, ?, ?)";

    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(m_db, add_song_query.c_str(), -1, &stmt, nullptr);

    if (result != SQLITE_OK) {
        std::cerr << "Error preparing insert statement: " << sqlite3_errmsg(m_db) << std::endl;
        return ;
    }

    sqlite3_bind_text(stmt, 1, a_song.get_song_name().c_str(), -1, SQLITE_TRANSIENT );
    sqlite3_bind_text(stmt, 2, a_song.get_artist_name().c_str(), -1, SQLITE_TRANSIENT );
    sqlite3_bind_int(stmt, 3, a_song.get_year());
    sqlite3_bind_text(stmt, 4, a_song.get_genre().c_str(), -1, SQLITE_TRANSIENT );
    sqlite3_bind_text(stmt, 5, a_song.get_lyrics().c_str(), -1, SQLITE_TRANSIENT);


    result = sqlite3_step(stmt);

    if (result != SQLITE_DONE) {
        std::cerr << "Error executing insert statement: " << sqlite3_errmsg(m_db) << std::endl;
    } else {
        std::cout << "Insert statement executed successfully." << std::endl;
    }

    sqlite3_finalize(stmt);
}

void SqlMng::add_playlist(std::string a_playlist)
{
     std::string sql = "INSERT OR IGNORE INTO playlist "
                     "(playlist_name) "
                     "VALUES (?);";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(m_db) << std::endl;
        sqlite3_close(m_db);
        return;
    }

    sqlite3_bind_text(stmt, 1, a_playlist.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << sqlite3_errmsg(m_db) << std::endl;
    } else {
        std::cout << "Insert statement executed successfully." << std::endl;
    }

    sqlite3_finalize(stmt);
}

void SqlMng::drop_tables() {
    std::string dropSongsTable = "DROP TABLE IF EXISTS songs;";
    sqlite3_exec(m_db, dropSongsTable.c_str(), nullptr, nullptr, nullptr);
    
    std::string dropPlaylistTable = "DROP TABLE IF EXISTS playlist;";
    sqlite3_exec(m_db, dropPlaylistTable.c_str(), nullptr, nullptr, nullptr);
    
    std::string dropPlaylistSongsTable = "DROP TABLE IF EXISTS songs_of_playlist;";
    sqlite3_exec(m_db, dropPlaylistSongsTable.c_str(), nullptr, nullptr, nullptr);
}

int SqlMng::get_playlist_id_by_name(const std::string& playlistName) {
    std::string selectPlaylistQuery = "SELECT playlist_id "
                                      "FROM playlist "
                                      "WHERE playlist_name = ?;";

    sqlite3_stmt* stmt;
    int playlistId = -1; // Initialize with a default value

    if (sqlite3_prepare_v2(m_db, selectPlaylistQuery.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare playlist retrieval query: " << sqlite3_errmsg(m_db) << std::endl;
        return playlistId;
    }

    if (sqlite3_bind_text(stmt, 1, playlistName.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
        std::cerr << "Failed to bind playlist name: " << sqlite3_errmsg(m_db) << std::endl;
        sqlite3_finalize(stmt);
        return playlistId;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        playlistId = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return playlistId;
}

int SqlMng::get_song_id_by_name(const std::string& songName) {
    std::string selectSongIdQuery = "SELECT song_id "
                                    "FROM songs "
                                    "WHERE song_name = ?;";

    sqlite3_stmt* stmt;
    int songId = -1; // Initialize with a default value

    if (sqlite3_prepare_v2(m_db, selectSongIdQuery.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare song ID retrieval query: " << sqlite3_errmsg(m_db) << std::endl;
        return songId;
    }

    if (sqlite3_bind_text(stmt, 1, songName.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
        std::cerr << "Failed to bind song name: " << sqlite3_errmsg(m_db) << std::endl;
        sqlite3_finalize(stmt);
        return songId;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        songId = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return songId;
}

void SqlMng::get_songs_by_condition(std::vector<Song>& a_found_songs, const std::string& artist, const std::string& genre, int year) {    


    if(artist.empty() && genre.empty() && year == 0){
        return ;
    }

    std::string query = "SELECT * FROM songs WHERE ";
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
        std::string lyrics = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));

        a_found_songs.emplace_back(id ,song_name, song_artists, year, genre, lyrics);
    }
    if (rc != SQLITE_DONE) {
        std::cerr << "Error while retrieving data: " << sqlite3_errmsg(m_db) << std::endl;
    }
    sqlite3_finalize(stmt);
}

bool SqlMng::is_song_exists(const Song& a_song) {
     std::string query = "SELECT * FROM songs WHERE "
                        "song_name = ? AND artist = ? AND year = ? AND genre = ?;";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing query." << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, a_song.get_song_name().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, a_song.get_artist_name().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, a_song.get_year());
    sqlite3_bind_text(stmt, 4, a_song.get_genre().c_str(), -1, SQLITE_TRANSIENT);

    bool exists = (sqlite3_step(stmt) == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return exists;
}

bool SqlMng::is_playlist_exists(std::string a_playlist){
     const char* sql = "SELECT COUNT(*) FROM playlist WHERE playlist_name = ?;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(m_db) << std::endl;
        sqlite3_close(m_db);
        return false;
    }

    sqlite3_bind_text(stmt, 1, a_playlist.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    bool exists = false;

    if (rc == SQLITE_ROW) {
        exists = (sqlite3_column_int(stmt, 0) > 0);
    } else if (rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << sqlite3_errmsg(m_db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return exists;
}

bool SqlMng::add_song_to_playlist(Song a_song, std::string a_playlist) {

    if(!is_song_exists(a_song)){
        add_song_to_list(a_song);
    }

    if(!is_playlist_exists(a_playlist)){
        add_playlist(a_playlist);
    }

    // if(is_songs_in_playlist)(
    //     return false;
    // )

    std::string insertQuery = "INSERT INTO songs_of_playlist (playlist_id, song_id) "
                              "VALUES ((SELECT playlist_id FROM playlist WHERE playlist.playlist_name = ?), "
                              "(SELECT song_id FROM songs WHERE songs.song_name = ? AND songs.artist = ? AND songs.year = ?));";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(m_db, insertQuery.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare insertion query: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }

    // Bind parameters

    sqlite3_bind_text(stmt, 1, a_playlist.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, a_song.get_song_name().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, a_song.get_artist_name().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, a_song.get_year());

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to execute insertion query: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }

    return true;
}

void SqlMng::get_songs_from_playlist(const std::string& playlistName, std::vector<Song>& songs) {
    std::string selectQuery = "SELECT *  FROM songs "
                              "INNER JOIN songs_of_playlist ON songs.song_id = songs_of_playlist.song_id "
                              "INNER JOIN playlist ON songs_of_playlist.playlist_id = playlist.playlist_id "
                              "WHERE playlist.playlist_name = ?;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(m_db, selectQuery.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare select query: " << sqlite3_errmsg(m_db) << std::endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, playlistName.c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string songName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string artist = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        int year = sqlite3_column_int(stmt, 3);
        std::string genre = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        std::string lyrics = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));

        songs.emplace_back(id, songName, artist, year, genre, lyrics);
    }

    sqlite3_finalize(stmt);
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


void SqlMng::add_lyrics(std::pair<Song , std::string> lyrics_pair)
{
    std::string insertQuery = "UPDATE songs SET lyrics = ? "
                            "WHERE song_name = ? AND artist = ? "
                            "AND genre = ? AND year = ?;";
                
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(m_db, insertQuery.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare insertion query: " << sqlite3_errmsg(m_db) << std::endl;
        return ;
    }

    sqlite3_bind_text(stmt, 1, lyrics_pair.second.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, lyrics_pair.first.get_song_name().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, lyrics_pair.first.get_artist_name().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, lyrics_pair.first.get_genre().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, lyrics_pair.first.get_year());

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to execute insertion query: " << sqlite3_errmsg(m_db) << std::endl;

    }
}

void SqlMng::get_playlists(std::vector<std::string>& a_playlists_res){
        std::string sql = "SELECT * FROM playlist;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(m_db) << std::endl;
        sqlite3_close(m_db);
        return;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char* playlist_name = sqlite3_column_text(stmt, 1);
        a_playlists_res.emplace_back(reinterpret_cast<const char*>(playlist_name));

    }

    if (rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << sqlite3_errmsg(m_db) << std::endl;
    }
    sqlite3_finalize(stmt);
}


void SqlMng::delete_song(const Song& song) {
    std::string deleteFromPlaylistTable = "DELETE FROM songs_of_playlist WHERE song_id = (SELECT song_id FROM songs WHERE "
        "song_name LIKE ? AND "
        "artist LIKE ? AND "
        "year LIKE ? AND "
        "genre LIKE ?) ";
    
    sqlite3_stmt* playlistStmt;
    if (sqlite3_prepare_v2(m_db, deleteFromPlaylistTable.c_str(), -1, &playlistStmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(playlistStmt, 1, song.get_song_name().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(playlistStmt, 2, song.get_artist_name().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(playlistStmt, 3, song.get_year());
        sqlite3_bind_text(playlistStmt, 4, song.get_genre().c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(playlistStmt) != SQLITE_DONE) {
            std::cerr << "SQL error: " << sqlite3_errmsg(m_db) << std::endl;
        }

        sqlite3_finalize(playlistStmt);
    } else {
        std::cerr << "SQL error: " << sqlite3_errmsg(m_db) << std::endl;
    }

    std::string deleteFromSongsTable = "DELETE FROM songs WHERE "
        "song_name LIKE ? AND "
        "artist LIKE ? AND "
        "year LIKE ? AND "
        "genre LIKE ? ";

    sqlite3_stmt* songsStmt;
    if (sqlite3_prepare_v2(m_db, deleteFromSongsTable.c_str(), -1, &songsStmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(songsStmt, 1, song.get_song_name().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(songsStmt, 2, song.get_artist_name().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(songsStmt, 3, song.get_year());
        sqlite3_bind_text(songsStmt, 4, song.get_genre().c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(songsStmt) != SQLITE_DONE) {
            std::cerr << "SQL error: " << sqlite3_errmsg(m_db) << std::endl;
        }

        sqlite3_finalize(songsStmt);
    } else {
        // Handle prepare error
    }
}


}

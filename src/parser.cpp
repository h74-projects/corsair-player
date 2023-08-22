#include "parser.hpp"
#include <string>

namespace m_player{

void Parser::parseSongs(const std::string& input ,std::vector<Song>& a_songs) {
    size_t startPos = 0;
    size_t endPos = input.find('\n');
    while (endPos != std::string::npos) {
        std::string songData = input.substr(startPos, endPos - startPos);
        startPos = endPos + 1;
        endPos = input.find('\n', startPos);

        Song song;
        size_t pos = songData.find('-');
        song.set_name(songData.substr(0, pos));
        size_t nextPos = songData.find('-', pos + 1);
        song.set_artist(songData.substr(pos + 1, nextPos - pos - 1));
        pos = nextPos;
        nextPos = songData.find('-', pos + 1);
        std::string year = songData.substr(pos + 1, nextPos - pos - 1);
        std::string cleanedYear = "";
        for (char c : year) {
            if (c != '\"') {
                cleanedYear += c;
            }
        }
        song.set_year(std::stoi(cleanedYear));
        pos = nextPos;
        song.set_genre(songData.substr(pos + 1));

        a_songs.push_back(song);
    }
}

void Parser::printSong(const Song& a_song){
    std::cout << "Song ID: " << a_song.get_song_id() << std::endl;
        std::cout << "Song Name: " << a_song.get_song_name() << std::endl;
        std::cout << "Artist: " << a_song.get_artist_name() << std::endl;
        std::cout << "Year: " << a_song.get_year() << std::endl;
        std::cout << "Genre: " << a_song.get_genre() << std::endl;
        std::cout << "Duration: " << a_song.get_duration_minutes() << " min " << a_song.get_duration_seconds() << " sec" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
}


void Parser::printSongs(const std::vector<Song>& a_songs) {
    for (const Song& song : a_songs) {
        std::cout << "Song ID: " << song.get_song_id() << std::endl;
        std::cout << "Song Name: " << song.get_song_name() << std::endl;
        std::cout << "Artist: " << song.get_artist_name() << std::endl;
        std::cout << "Year: " << song.get_year() << std::endl;
        std::cout << "Genre: " << song.get_genre() << std::endl;
        std::cout << "Duration: " << song.get_duration_minutes() << " min " << song.get_duration_seconds() << " sec" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
    }
}

}//namespace m_player
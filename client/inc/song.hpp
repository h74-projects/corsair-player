#ifndef SONG_HPP
#define SONG_HPP

#include <sqlite3.h>
#include <iostream>
#include <unordered_map>


namespace m_player{

class Song{
public:
    Song(const int&, const std::string& a_song_name, const std::string& a_song_artists, const int& a_year, const std::string& a_genre, std::string a_lyrics);
    ~Song();
    Song();


    std::string get_song_name() const;
    std::string get_artist_name() const;
    int get_song_id() const;
    int get_year()const;
    std::string get_genre()const;
    std::string get_lyrics()const;
    std::string get_search_name()const;


    void set_id(int);
    void set_name(std::string); 
    void set_artist(std::string);
    void set_year(int);
    void set_genre(std::string);
    void set_lyrics(std::string a_lyrics);

    void print() const;

private:
    int m_song_id;
    std::string m_song_name;
    std::string m_artist;
    int m_year;
    std::string m_genre;
    std::string m_lyrics;

};

}//namespace m_player

#endif //SONG_HPP
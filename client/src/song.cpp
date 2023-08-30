#include "song.hpp"
#include <vector>

namespace m_player{

Song::Song(const int& a_song_id, const std::string& a_song_name, const std::string& a_song_artists, const int& a_year, const std::string& a_genre, std::string a_lyrics)
    :m_song_id(a_song_id)
    ,m_song_name(a_song_name)
    ,m_artist(a_song_artists)
    ,m_year(a_year)
    ,m_genre(a_genre)
    ,m_lyrics(a_lyrics)
{
}

Song::Song() 
    :m_song_id(0) 
    ,m_song_name("")
    ,m_artist("")
    ,m_year(0)
    ,m_genre("")
    ,m_lyrics("")
{
}



Song::~Song(){}



std::string Song::get_song_name()const
{
    return m_song_name;
}

std::string Song::get_artist_name()const 
{
    return m_artist;
}

int Song::get_song_id()const
{
    return m_song_id;
}

int Song::get_year()const
{
    return m_year;
}
std::string Song::get_genre()const
{
    return m_genre;
}
void Song::set_id(int a_song_id) {
    m_song_id = a_song_id;
}

void Song::set_name(std::string a_song_name) {
    m_song_name = std::move(a_song_name);
}

void Song::set_artist(std::string a_artist) {
    m_artist = std::move(a_artist);
}

void Song::set_year(int a_year) {
    m_year = a_year;
}

void Song::set_genre(std::string a_genre) {
    m_genre = std::move(a_genre);
}


void Song::set_lyrics(std::string a_lyrics)
{
    m_lyrics = a_lyrics;
}

std::string Song::get_lyrics()const
{
    return m_lyrics;
}

std::string Song::get_search_name()const
{
    return m_song_name + " - " + m_artist;
}


void Song::print() const
{
     std::cout << "Song ID: " << m_song_id << "\n";
    std::cout << "Song Name: " << m_song_name << "\n";
    std::cout << "Artist: " << m_artist << "\n";
    std::cout << "Year: " << m_year << "\n";
    std::cout << "Genre: " << m_genre << "\n";
    std::cout << "lyrics:" << m_lyrics << "\n";
}

} //namespace m_player
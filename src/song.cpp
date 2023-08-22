#include "song.hpp"
#include <vector>

namespace m_player{

Song::Song(const int& a_song_id, const std::string& a_song_name, const std::string& a_song_artists, const int& a_year, const std::string& a_genre, int a_duration_min, int a_duration_sec)
    :m_song_id(a_song_id)
    ,m_song_name(a_song_name)
    ,m_artist(a_song_artists)
    ,m_year(a_year)
    ,m_genre(a_genre)
    ,m_duration(a_duration_min, a_duration_sec)
{
}

Song::Song() 
    :m_song_id(0) 
    ,m_song_name("")
    ,m_artist("")
    ,m_year(0)
    ,m_genre("")
    ,m_duration(0, 0) // Initialize duration to 0 minutes and 0 seconds
{
}



Song::~Song(){}


int Song::get_duration_minutes() const
{
    return m_duration.m_minutes;
}

int Song::get_duration_seconds() const
{
    return m_duration.m_seconds;
}

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

void Song::set_duration_minutes(int a_duration_in_min) {
    m_duration.m_minutes = a_duration_in_min;
}

void Song::set_duration_seconds(int a_duration_in_sec) {
    m_duration.m_seconds = a_duration_in_sec;
}



} //namespace m_player
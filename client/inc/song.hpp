#ifndef SONG_HPP
#define SONG_HPP

#include <sqlite3.h>
#include <iostream>
#include <unordered_map>

struct Duration {
    int m_minutes;
    int m_seconds;

    Duration(int a_min , int a_sec)
    :m_minutes(a_min)
    ,m_seconds(a_sec)
    {
    }
};

namespace m_player{

class Song{
public:
Song(const int& a_id,const std::string& a_song_name, const std::string& a_song_artists, const int& a_year, const std::string& a_genre, int a_duration_min, int a_duration_sec);
~Song();
Song();

    int get_duration_minutes() const;
    int get_duration_seconds() const;
    std::string get_song_name() const;
    std::string get_artist_name() const;
    int get_song_id() const;
    int get_year()const;
    std::string get_genre()const;

    void set_id(int);
    void set_name(std::string); 
    void set_artist(std::string);
    void set_year(int);
    void set_genre(std::string);
    void set_duration_minutes(int a_duration_in_min);
    void set_duration_seconds(int a_duration_in_sec);

private:
    int m_song_id;
    std::string m_song_name;
    std::string m_artist;
    int m_year;
    std::string m_genre;
    Duration m_duration;

};

}//namespace m_player

#endif //SONG_HPP
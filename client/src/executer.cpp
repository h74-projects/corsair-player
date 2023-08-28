#include "executer.hpp"
#include "blocking_queue.hpp"
#include <Poco/ThreadPool.h>
//#include "parser.hpp"
#include "sql_mng.hpp"
#include "songs_controller.hpp"

#include <iostream>

namespace {

class RunnableFunction : public Poco::Runnable {
public:
    using FunctionType = std::function<void()>;

    RunnableFunction(const FunctionType& a_function) : m_function(a_function) {}

    void run() override {
        m_function();
    }
private:
    FunctionType m_function;
};
}

namespace m_player{

Executer::Executer
: m_url {"44.218.10.253"}
, m_db("my_db")
{
}

void Executer::parse_songs(const std::string& songs_string, std::vector<Song>& songs) 
{
    std::istringstream iss(songs_string);
    std::string line;

    while (std::getline(iss, line)) {
        std::istringstream line_stream(line);
        int id, year, duration_min, duration_sec;
        std::string song_name, song_artists, genre;

        line_stream >> id;
        std::getline(line_stream, song_name, '/');
        std::getline(line_stream, song_artists, '/');
        line_stream >> year >> genre >> duration_min >> duration_sec;

        Song song(id, song_name, song_artists, year, genre, duration_min, duration_sec);

        songs.emplace_back(song);
    }
}

void Executer::get_url(std::vector<Song> play_list_vec, std::vector<std::string>url_vec)
{
    std::vector<std::string> name_and_art;
    for (const auto& song : play_list_vec) {
        name_and_art.emplace_back(song.get_song_name() + " " + song.get_artist_name);
    }
    get_songs_url(url_vec, name_and_art);
}


void Executer::New_play_list(std::string a_playlist, std::string a_playlist_name)
{
    std::string list_of_songs = get_songs_list(a_playlist);
    std::vector<Song> play_list_vec;
    parse_songs(list_of_songs, play_list_vec);
    std::vector<std::string> url_vec;
    get_url(play_list_vec, url_vec);
    sent_to_db(play_list_vec, a_playlist_name);
    m_db.print_songs();
    //down_songs(play_list_vec, url_vec);   
}

// void Executer::New_Song(std::string a_playlist)
// {
//     std::string list_of_songs = get_songs_list(a_playlist);
//     std::vector<std::pair<Song, std::string>> play_list_vec;
//     parse_songs(list_of_songs, play_list_vec);
//     sent_to_db(play_list_vec);
//     down_songs(play_list_vec);   
// }

void Executer::sent_to_db(std::vector<std::pair<Song, std::string>> a_songs, std::string a_name)
{
    for (const auto& song : a_songs) {
        m_db.add_song_to_playlist(song, a_name);
    }
}

void Executer::down_songs(std::vector<Song> a_songs, std::vector<std::string> a_urls)
{
    if (a_songs.size() != a_url.size()) {
        std::cerr << "not possible downlaod list\n";
        return;
    }

    BlockingQueue<std::pair<std::string, std::string>> queue;

    for (size_t i = 0; i < a_songs.size(); ++i) {
        queue.emplace_back(a_songs[i], a_urls[i]);

    }

    therad_count = 3; 
    //TODO function to get num of cores

    // Create a blocking queue to hold the songs
    Poco::ThreadPool threadPool(therad_count);
    //  Downloader dwn();
    auto worker_function = [&queue]() {
        while (true) {
            std::string song;
            if (queue.dequeue(song)) {
                std::cout << song  << std::endl;
            //  dwn.run(song, "../assets/songs");
            } else {
                break; 
            }
        }
    };

    for (size_t i = 0; i < therad_count; ++i) {
        std::shared_ptr<RunnableFunction> runnable = std::make_shared<RunnableFunction>(worker_function);
        threadPool.start(*runnable); // Pass the MyRunnable instance by reference
    }

    threadPool.joinAll();
}

}//namespace m_player

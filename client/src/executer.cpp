#include "executer.hpp"
#include <Poco/ThreadPool.h>
#include "downloader.hpp"
#include "blocking_queue.hpp"
#include "songs_controller.hpp"
#include "song.hpp"
#include <iostream>
#include <thread>

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

Executer::Executer()
:m_url("44.218.10.253")
,m_db("my_db")
{
}

void Executer::parse_songs(const std::string& songs_string, std::vector<Song>& songs) 
{
   
    std::istringstream iss(songs_string);
    std::string line;

    while (std::getline(iss, line)) {
        std::string name, artists, genre, yearStr;
        std::stringstream ss(line);

        std::getline(ss, name, '|');
        std::getline(ss, artists, '|');
        std::getline(ss, genre, '|');
        std::getline(ss, yearStr, '|');
        yearStr.erase(std::remove_if(yearStr.begin(), yearStr.end(), [](char c) { return !std::isdigit(c); }), yearStr.end());
        int year ;
        if(!yearStr.empty()){
            year = std::stoi(yearStr);

        }

        songs.emplace_back(0,name, artists, year,genre,0,0);
    }

}

void Executer::get_url(std::vector<Song>& play_list_vec, std::vector<std::string>& url_vec)
{   
    SongsController ctrl; 
    std::vector<std::string> name_and_art;
    for (const auto& song : play_list_vec) {
        name_and_art.emplace_back(song.get_song_name() + " - " + song.get_artist_name());
    }
    ctrl.get_songs_urls(url_vec, name_and_art);
}


void Executer::new_play_list(std::string a_playlist, std::string a_playlist_name)
{
    //Return songs list from gpt
    SongsController ctrl; 
    std::string list_of_songs = ctrl.get_songs_list(a_playlist);

    //Parsing song into play_list_vec
    std::vector<Song> play_list_vec;
    parse_songs(list_of_songs, play_list_vec);

    //Retrive vector with urls
    std::vector<std::string> url_vec;
    get_url(play_list_vec, url_vec);
    
    //Save songs in db
    sent_to_db(play_list_vec, a_playlist_name);
    m_db.print_songs();

    down_songs(play_list_vec ,url_vec );

}

void Executer::sent_to_db(std::vector<Song> a_songs, std::string a_name)
{
    for (const auto& song : a_songs) {
        m_db.add_song_to_playlist(song, a_name);
    }
}

void Executer::down_songs(std::vector<Song> a_songs, std::vector<std::string> a_urls)
{
    if (a_songs.size() != a_urls.size()) {
        std::cerr << "not possible downlaod list\n";
        return;
    }

    BlockingQueue<std::pair<std::string, std::string>> queue;

    for (size_t i = 0; i < a_songs.size(); ++i) {
        queue.enqueue(std::make_pair(a_songs[i].get_song_name()+ " - " +a_songs[i].get_artist_name() , a_urls[i]));

    }
    create_songs_file();
    size_t therad_count = 3; 
    //TODO function to get num of cores

    // Create a blocking queue to hold the songs
    Poco::ThreadPool threadPool(therad_count);
    auto worker_function = [&queue]() {
        while (true) {
            std::pair<std::string, std::string> song_and_url;
            if(queue.dequeue(song_and_url)) {
                std::cout << song_and_url.first  << std::endl;
                download(song_and_url.second, song_and_url.first);
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

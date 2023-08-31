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
        songs.emplace_back(0, name, artists, year, genre, "");
    }

}

void Executer::get_url(std::vector<Song>& play_list_vec, std::vector<std::string>& url_vec)
{   
    SongsController ctrl; 

    ctrl.get_songs_urls(url_vec, play_list_vec , m_db);
}



void Executer::new_play_list(std::string a_playlist, std::string a_playlist_name)
{
    //Return songs list from gpt
    SongsController ctrl; 
    std::string list_of_songs = ctrl.get_songs_list(a_playlist);

    //Parsing song into play_list_vec
    std::vector<Song> play_list_vec;
    parse_songs(list_of_songs, play_list_vec);

    
    //Save songs in db
    sent_to_db(play_list_vec, a_playlist_name);
    m_db.print_songs();

    //Retrive vector with urls
    std::vector<std::string> url_vec;
    get_url(play_list_vec, url_vec);

    down_songs(play_list_vec ,url_vec );

}

void Executer::sent_to_db(std::vector<Song>& a_songs, std::string a_name)
{
    for (auto it = a_songs.begin(); it != a_songs.end();) {
        if (m_db.is_song_exists(*it)) {
            m_db.add_song_to_playlist(*it, a_name);
            it = a_songs.erase(it); //Erase return the next iterator
        } else {
            m_db.add_song_to_playlist(*it, a_name);
            ++it;
        }
    }
}


void Executer::down_songs(std::vector<Song> a_songs, std::vector<std::string> a_urls)
{   
    std::cout << " songs size " << a_songs.size() << " url size: " <<  a_urls.size() << std::endl;
    if (a_songs.size() != a_urls.size()) {
        std::cerr << "not possible downlaod list\n";
        return;
    }

    BlockingQueue<std::pair<Song, std::string>> queue;

    for (size_t i = 0; i < a_songs.size(); ++i) {
        queue.enqueue(std::make_pair(a_songs[i] , a_urls[i]));
    

    }
    create_songs_file();
    //size_t therad_count = std::thread::hardware_concurrency(); 
    size_t therad_count = 3;
    std::cout << therad_count << std::endl;
    
    Poco::ThreadPool threadPool(therad_count);
    BlockingQueue<std::pair<Song, std::string>> song_n_lyrics_queue;
    
    auto worker_function = [&queue , &song_n_lyrics_queue]() {
        while (true) {
            std::pair<Song, std::string> song_and_url;
            if(queue.dequeue(song_and_url)) {                
                download(song_and_url.second, song_and_url.first.get_search_name());
                SongsController ctrl;
                song_n_lyrics_queue.enqueue(std::make_pair(song_and_url.first,ctrl.get_lyrics(song_and_url.first.get_search_name())));
            } else {
                break; 
            }
        }
    };

    for (size_t i = 0; i < therad_count; ++i) {
        std::shared_ptr<RunnableFunction> runnable = std::make_shared<RunnableFunction>(worker_function);
        threadPool.start(*runnable); 
    }

    threadPool.joinAll();



    std::pair<Song , std::string> lyrics_pair;
    while(song_n_lyrics_queue.dequeue(lyrics_pair)){
        m_db.add_lyrics(lyrics_pair);
    }
}



}//namespace m_player

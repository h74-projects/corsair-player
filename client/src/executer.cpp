#include "executer.hpp"
#include "blocking_queue.hpp"
#include <Poco/ThreadPool.h>
#include "gpt_requests.hpp"
#include "parser.hpp"
#include "sql_mng.hpp"

#include <iostream>

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

namespace m_player{



void Executer::multi_execute(std::string a_requset, std::string a_playlist_name, size_t a_therad_count) 
{
    m_player::GptRequest request;
    std::string answer = request.generate_gpt_request(a_requset);

    m_player::Parser parser;
    std::vector<m_player::Song> songs; 
    parser.parseSongs(answer, songs);

    m_player::SqlMng sqlmng("my_db");
    for(m_player::Song song : songs){
        sqlmng.add_song_to_list( song);
        //TODO -fix line just for compiling
        if(a_playlist_name.empty()){}

        parser.printSong(song);
    }
    BlockingQueue<std::string> queue;

    for (auto song : songs) {
        queue.enqueue(song.get_song_name() + " " + song.get_artist_name());
    }

    // Create a blocking queue to hold the songs
    Poco::ThreadPool threadPool(a_therad_count);
    //  Downloader dwn();
    auto workerFunction = [&queue]() {
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

    for (size_t i = 0; i < a_therad_count; ++i) {
        std::shared_ptr<RunnableFunction> runnable = std::make_shared<RunnableFunction>(workerFunction);
        threadPool.start(*runnable); // Pass the MyRunnable instance by reference
    }

    threadPool.joinAll();
}

}//namespace m_player

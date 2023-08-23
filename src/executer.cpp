#include "executer.hpp"
#include "blocking_queue.hpp"
#include <Poco/ThreadPool.h>

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


void Executer::thread_pool(const std::vector<std::string>& a_songs, size_t a_therad_count) {
    Poco::ThreadPool threadPool(a_therad_count);

    // Create a blocking queue to hold the songs
    BlockingQueue<std::string> queue;

    for (auto str : a_songs) {
        queue.enqueue(str);
    }

    auto workerFunction = [&queue]() {
        while (true) {
            std::string song;
            if (queue.dequeue(song)) {
                std::cout << song  << std::endl;
            //  Link link(song)
            //  std::string url= link.get_link()
            //  Downloader dwn(song, url);
            //  dwn.download();

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

#ifndef BLOCKING_QUEUE_HPP
#define BLOCKING_QUEUE_HPP
#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class BlockingQueue {
public:
    void enqueue(const T& a_item) 
    {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_queue.push(a_item);
        }
        m_cv.notify_all();
    }

    bool dequeue(T& a_item) 
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (!m_cv.wait_for(lock, std::chrono::seconds(1), [this] { return !m_queue.empty(); })) {
            return false; //queue is empty
        }
        a_item = m_queue.front();
        m_queue.pop();
        return true;
    }
 
    bool empty()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cv;
};

#endif //BLOCKING_QUEUE_HPP

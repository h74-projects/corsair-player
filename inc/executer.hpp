#ifndef EXECUTER_HPP
#define EXECUTER_HPP
#include <string>
#include <vector>

namespace m_player{

class Executer{

public:
Executer() = default;
~Executer() = default;
void thread_pool(const std::vector<std::string>& a_songs, size_t a_therd_count);

private:


};
}//namespace m_player

#endif //EXECUTER_HPP
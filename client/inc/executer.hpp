#ifndef EXECUTER_HPP
#define EXECUTER_HPP
#include <string>
#include <vector>

namespace m_player{

class Executer{

public:
Executer() = default;
~Executer() = default;
void multi_execute(std::string a_requset, std::string a_name, size_t a_therd_count);

private:


};
}//namespace m_player

#endif //EXECUTER_HPP
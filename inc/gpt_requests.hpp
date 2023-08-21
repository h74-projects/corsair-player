#ifndef GPT_REQUESTS_HPP
#define GPT_REQUESTS_HPP
#include <string>


namespace m_player{

class GptRequest{
public:

GptRequest() = default;
~GptRequest() = default;
//will parse respons by song_name-artist_name
std::string generate_gpt_request(std::string a_request);

};

}//namespace m_player

#endif //GPT_REQUESTS_HPP
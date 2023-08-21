#include "gpt_requests.hpp"
#include "sql_mng.hpp"
#include <iostream>

int main(){
    m_player::GptRequest request;
    m_player::SqlMng sqlmng("my_db");
    std::string answer = request.generate_gpt_request("give top 10 trap songs");
    std::cout <<answer << std::endl;

    //sqlmng.remove_all_songs();
    sqlmng.add_song("people" , "libianca");
    sqlmng.print_all_songs();


    return 0;
}
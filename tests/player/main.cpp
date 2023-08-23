#include "gpt_requests.hpp"
#include "sql_mng.hpp"
#include "parser.hpp"
#include "executer.hpp"
//#include "music_search.hpp"
#include <iostream>

void drop_tables(){
    m_player::SqlMng sqlmng("my_db");
    sqlmng.drop_table_by_name("downloads");
}

void add_songs_to_play_list(){
    m_player::GptRequest request;

    m_player::SqlMng sqlmng("my_db");

    //create 10 songs
    std::string answer = request.generate_gpt_request("give top 10 trap songs");

    //parse them into vector
    m_player::Parser parser;
    std::vector<m_player::Song> songs; 
    parser.parseSongs(answer , songs);
    //create table if not exists
    sqlmng.create_table("trap");

    //add songs into trap table(and into downloads as default)
    for(m_player::Song song : songs){
        sqlmng.add_song_to_table( "trap" , song);
        sqlmng.add_song_to_table( "downloads" , song);

        parser.printSong(song);
    }

    //print songs by drake(from downloads)
    std::vector<m_player::Song> conditional_songs; 
    sqlmng.get_songs_by_condition(conditional_songs,"Drake","", 0);
    parser.printSongs(conditional_songs);

}

void print_all_songs(){
    m_player::Parser parser;

    m_player::SqlMng sqlmng("my_db");
    sqlmng.print_all_songs_at_table("downloads");


    std::vector<m_player::Song> conditional_songs; 
    sqlmng.get_songs_by_condition(conditional_songs,"Drake","", 0);
    parser.printSongs(conditional_songs);

}

void executer_test(){
    m_player::GptRequest request;
    m_player::Parser parser;
    std::vector<m_player::Song> songs; 

    std::string answer = request.generate_gpt_request("give top 10 trap songs");
    parser.parseSongs(answer,songs);
    std::vector<std::string> songs_search;
    for(auto song : songs){
        songs_search.emplace_back(song.get_song_name() + "" +  song.get_artist_name());
    } 
    m_player::Executer exe;

    exe.thread_pool(songs_search , 2);
}

void need_testing(){
    //sqlmng.remove_all_songs();
    //sqlmng.drop_songs_table();
    // sqlmng.add_song("right now" , "akon");
    // sqlmng.add_song("lonely" , "akon");

    // std::cout << std::boolalpha <<  sqlmng.is_song_exists("people" , "libianca") << std::endl;
    // std::cout  <<  sqlmng.is_song_exists("people" , "someelse") << std::endl;
    /// std::cout  <<  sqlmng.is_song_exists("else" , "libianca") << std::endl;

    // m_player::Music_Search search;
    //search.print_maps();
}



int main(){

    //drop_tables();

    //add_songs_to_play_list();

    //print_all_songs();

    executer_test();

    
    return 0;
}
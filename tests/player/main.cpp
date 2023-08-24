#include "gpt_requests.hpp"
#include "sql_mng.hpp"
#include "parser.hpp"
#include "executer.hpp"
#include "song.hpp"
//#include "music_search.hpp"
#include <iostream>


void add_songs_to_play_list(){
   
    m_player::SqlMng sqlmng("my_db");

    //create 10 songs
     m_player::GptRequest request;
    std::string answer = request.generate_gpt_request("give top 10 trap songs");

    //parse them into vector
    m_player::Parser parser;
    std::vector<m_player::Song> songs; 
    parser.parseSongs(answer , songs);


    //add songs into trap table(and into downloads as default)
    for(m_player::Song song : songs){
        sqlmng.add_song_to_playlist(song , "trap");
        parser.printSong(song);
    }
    songs.clear();
    sqlmng.get_songs_from_playlist("trap" ,songs);

    for(m_player::Song song : songs){
        parser.printSong(song);
    }

}

void check_if_songs_in_list(){
    m_player::SqlMng sqlmng("my_db");
    std::vector<m_player::Song> songs; 
    sqlmng.get_songs_from_playlist("trap" ,songs);
}



void insert_sing_song_test(){
    {
        m_player::SqlMng sqlmng("my_db");
        sqlmng.drop_tables();

    }
    m_player::SqlMng sqlmng("my_db");
    m_player::Song song(0 , "Traffic" , "Reggie" , 2023 , "Chill" ,2 , 32);
    sqlmng.add_song_to_list(song);
    sqlmng.print_songs();
    sqlmng.print_playlist_table();
    
}

void sql_test(){
    // {
    //     m_player::SqlMng sqlmng("my_db");
    // sqlmng.drop_tables();

    // }

    m_player::GptRequest request;
    std::string answer = request.generate_gpt_request("give top 10 Chill songs");

    m_player::SqlMng sqlmng("my_db");

    m_player::Parser parser;
    std::vector<m_player::Song> songs; 
    parser.parseSongs(answer , songs);


    //add songs into trap table(and into downloads as default)
    for(m_player::Song song : songs){
        sqlmng.add_song_to_playlist(song , "Chill");
        parser.printSong(song);
    }

    std::cout << " Chill list:" << std::endl;
    songs.clear();
    sqlmng.get_songs_from_playlist("Chill",songs);

    for(m_player::Song song : songs){
        parser.printSong(song);
    }

    std::cout << " Trap list:" << std::endl;
    songs.clear();
    sqlmng.get_songs_from_playlist("Trap",songs);

    for(m_player::Song song : songs){
        parser.printSong(song);
    }

    // sqlmng.print_song_of_playlists();
    // sqlmng.print_playlist_table();


}

void executer_test(){
    // m_player::GptRequest request;
    // m_player::Parser parser;
    // std::vector<m_player::Song> songs; 

    // std::string answer = request.generate_gpt_request("give top 10 trap songs");
    // parser.parseSongs(answer,songs);
    // std::vector<std::string> songs_search;
    // for(auto song : songs){
    //     songs_search.emplace_back(song.get_song_name() + "" +  song.get_artist_name());
    // } 
    m_player::Executer exe;

    exe.multi_execute("give me top 10 hip hop songs ", "hip hop" , 2);
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


    //add_songs_to_play_list();

    //check_if_songs_in_list();
    //executer_test();


    sql_test();

    
    return 0;
}
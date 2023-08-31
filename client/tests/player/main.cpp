#include "sql_mng.hpp"
#include "executer.hpp"
#include "song.hpp"
#include "downloader.hpp"
//#include "music_search.hpp"
#include <iostream>
#include "songs_controller.hpp"


// void add_songs_to_play_list(){
   
//     m_player::SqlMng sqlmng("my_db");

//     //create 10 songs
//      m_player::GptRequest request;
//     std::string answer = request.generate_gpt_request("give top 10 trap songs");

//     //parse them into vector
//     m_player::Parser parser;
//     std::vector<m_player::Song> songs; 
//     parser.parseSongs(answer , songs);


//     //add songs into trap table(and into downloads as default)
//     for(m_player::Song song : songs){
//         sqlmng.add_song_to_playlist(song , "trap");
//         parser.printSong(song);
//     }
//     songs.clear();
//     sqlmng.get_songs_from_playlist("trap" ,songs);

//     for(m_player::Song song : songs){
//         parser.printSong(song);
//     }

// }

// void check_if_songs_in_list(){
//     m_player::SqlMng sqlmng("my_db");
//     std::vector<m_player::Song> songs; 
//     sqlmng.get_songs_from_playlist("trap" ,songs);
// }



// void insert_sing_song_test(){
//     {
//         m_player::SqlMng sqlmng("my_db");
//         sqlmng.drop_tables();

//     }
//     m_player::SqlMng sqlmng("my_db");
//     m_player::Song song(0 , "Traffic" , "Reggie" , 2023 , "Chill" ,2 , 32);
//     sqlmng.add_song_to_list(song);
//     sqlmng.print_songs();
//     sqlmng.print_playlist_table();
    
// }



// void sql_test(){


//     m_player::SqlMng sqlmng("my_db");

//     m_player::Parser parser;
//     std::vector<m_player::Song> songs; 
//    // parser.parseSongs(answer , songs);


//     //add songs into trap table(and into downloads as default)
//     for(m_player::Song song : songs){
//         sqlmng.add_song_to_playlist(song , "Chill");
//         parser.printSong(song);
//     }

//     std::cout << " Chill list:" << std::endl;
//     songs.clear();
//     sqlmng.get_songs_from_playlist("Chill",songs);

//     for(m_player::Song song : songs){
//         parser.printSong(song);
//     }

//     std::cout << " Trap list:" << std::endl;
//     songs.clear();
//     sqlmng.get_songs_from_playlist("Trap",songs);

//     for(m_player::Song song : songs){
//         parser.printSong(song);
//     }

//     // sqlmng.print_song_of_playlists();
//     // sqlmng.print_playlist_table();


// }


void clean_db(){
    m_player::SqlMng sqlmng("my_db");
    sqlmng.drop_tables();
}

// void url_test(){
//     std::vector<std::string> topSongs;

//     // Insert the names of the top 10 songs
//     topSongs.push_back("Bohemian Rhapsody");
//     topSongs.push_back("Like a Rolling Stone");
//     topSongs.push_back("Imagine");
//     topSongs.push_back("Billie Jean");
//     topSongs.push_back("Stairway to Heaven");
//     topSongs.push_back("Hey Jude");
//     topSongs.push_back("Smells Like Teen Spirit");
//     topSongs.push_back("What's Going On");
//     topSongs.push_back("Johnny B. Goode");
//     topSongs.push_back("Hotel California");
//     std::vector<std::string> results;

//     m_player::SongsController songs_crl;
//     songs_crl.get_songs_urls(results, topSongs) ;
//     for(auto song : results){
//         std::cout << song << std::endl;
//     }

// }


void executer_test(){
    m_player::Executer exe;
    exe.new_play_list("Give me top 10 Fairuz songs" , "Fairuz");
}

void sql_print(){
    m_player::SqlMng sql("my_db");
    m_player::Song song(0 ,"Lam Only Fakhr El Donia" , "Fairuz" , 1976 ,"Lebanese" , "" );
    sql.delete_song(song);
    sql.print_songs();
    sql.print_song_of_playlists();
    sql.print_playlist_table();
    std::vector<m_player::Song> song_vec;
    sql.get_songs_from_playlist("Fairuz", song_vec);
    std::cout << "Fairuz playlist:" << std::endl;
    for(auto song: song_vec){
        song.print();
    }
}



int main(){
    //sql_print();
    clean_db();
    executer_test();
    sql_print();

    // single_url_test();
    //url_test();
    return 0;
}
#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include "song.hpp"

namespace m_player{

class Parser{

public:
void parseSongs(const std::string& input ,std::vector<Song>& a_songs);
void printSongs(const std::vector<Song>&);
void printSong(const Song&);


};

}//namespace m_player

#endif //PARSER_HPP
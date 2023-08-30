#ifndef DOWNLOADER_HPP
#define DOWNLOADER_HPP

#include <string>

namespace m_player{

bool download(std::string a_link, std::string a_file_name);
void create_songs_file();

}//namespace m_player
#endif //DOWNLOADER_HPP

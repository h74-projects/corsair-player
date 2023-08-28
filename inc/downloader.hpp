#ifndef DOWNLOADER_HPP
#define DOWNLOADER_HPP

#include <string>

namespace m_player{

class Downloader{
public:
    Downloader();

    bool download(std::string a_song, std::string a_path = "../assets/songs/");
    bool run(std::string a_song, std::string a_path);
    // std::string get_link();
    
private:
    bool generate_link(std::string& a_result);

private:
    std::string m_link;
};

std::string find_url(const std::string& a_query);
std::string trim(const std::string& str);

}//namespace m_player
#endif //DOWNLOADER_HPP

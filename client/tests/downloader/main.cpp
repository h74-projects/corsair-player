#include "downloader.hpp"
#include <iostream>

int main()
{
    std::string song = "hanan ben ari - tutim";
    std::string dir = "songs";
    m_player::Downloader d(song, dir);
    if(d.download())
    {
        std::cout << "Success!" << '\n';
    }else{
        std::cout << "Fail!" << '\n';
    }

    return 0;
}
#include "downloader.hpp"
#include <iostream>

int main()
{
    std::string song = "hanan ben ari - aluf haolam";
    std::string dir = "songs";
    m_player::Downloader d;
    if(d.run(song, dir))
    {
        std::cout << "Success!" << '\n';
    }else{
        std::cout << "Fail!" << '\n';
    }

    return 0;
}
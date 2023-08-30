#include "downloader.hpp"
#include <iostream>

const std::string songs_path = "../../assets/songs";

namespace m_player{

void create_songs_file(){
    std::string dir_cmnd = "mkdir \"" + songs_path + "\"";
    std::system(dir_cmnd.c_str());
}

bool download(std::string a_link, std::string a_file_name){
    std::cout << a_link << std::endl;
    std::string output = songs_path + "/" + a_file_name + ".%(ext)s";
    std::string command = "yt-dlp -x --audio-format mp3 -o \"" + output + "\" \"" + a_link + "\"";
    
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Failed to open pipe." << std::endl;
        return false;
    }

    std::cout << "Processing...";

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        std::cout << ".";
        if (!buffer)
        {
            return false;
        }
    }

    int status = pclose(pipe);
    if (status == -1) {
        std::cerr << "Failed to close pipe." << std::endl;
        return false;
    }
    std::cout << "Done!\n";
    return true;
}


}//namespace m_player
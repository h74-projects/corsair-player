#include "downloader.hpp"
#include <iostream>


namespace m_player{

Downloader::Downloader(std::string a_song, std::string a_path)
: m_path(a_path)
, m_file_name(a_song)
{
    std::string result = find_url(a_song);
    if(result != "Fail"){
        generate_link(result);
    }
    std::cout <<m_link<< '\n';
}


bool Downloader::download()
{
    std::string output = m_path + "/" + m_file_name + ".%(ext)s";
    std::string command = "yt-dlp -x --audio-format mp3 -o \"" + output + "\" \"" + m_link + "\"";
    std::string dir_cmnd = "mkdir \"" + m_path + "\"";
    std::system(dir_cmnd.c_str());

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
        // std::cout << buffer;
    }

    int status = pclose(pipe);
    if (status == -1) {
        std::cerr << "Failed to close pipe." << std::endl;
        return false;
    }
    std::cout << "Done!\n";
    return true;
}

std::string find_url(const std::string& a_song)
{
    std::string command = "python3 seleniumScript.py '" + a_song + "'";

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Failed to run command." << std::endl;
        return "Fail";
    }

    char buffer[128];
    std::string result = "";

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result += buffer;
    }

    pclose(pipe);
    return result;
}



bool Downloader::generate_link(std::string& a_string)
{

    if (!a_string.empty() && a_string != "No search result URLs found."){
        a_string = trim(a_string);
        // std::cout << "URL of the first search result:555 " << a_string << std::endl;
        m_link = a_string;
    } else{
        std::cout << "No search result URLs found2." << std::endl;
    }

    return true;
}

// std::string Downloader::get_link()
// {
//     return m_link;
// }

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n");
    size_t last = str.find_last_not_of(" \t\n");
    if (first == std::string::npos || last == std::string::npos) {
        return "";
    }
    return str.substr(first, last - first + 1);
}
}//namespace m_player



/*
#include <iostream>
#include <cstdlib>
#include <thread>
#include <sys/wait.h>


int main() {
    const char* youtubeUrl = "https://www.youtube.com/watch?v=-w-2K9U8UIs&pp=ygUKZ29sZGVuIGJveQ%3D%3D";
    const char* command = "yt-dlp -x --audio-format mp3 -o \"goldy888.mp3\"";

    // Combine the command and YouTube URL
    std::string fullCommand = std::string(command) + " " + youtubeUrl;

    // Execute the command
    std::system(fullCommand.c_str());




    return 0;
}*/

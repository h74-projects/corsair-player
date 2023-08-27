#include "downloader.hpp"
#include <iostream>


namespace m_player{

Downloader::Downloader()
: m_link("")
{
    
}

bool Downloader::run(std::string a_song, std::string a_path)
{
    std::string result = find_url(a_song);
    if(result != "Fail"){
        generate_link(result);
    }
    download(a_song, a_path);
    std::cout <<m_link<< '\n';
    
    return true;
}

bool Downloader::download(std::string a_song, std::string a_path)
{
    std::string output = a_path + "/" + a_song + ".%(ext)s";
    std::string command = "yt-dlp -x --audio-format mp3 -o \"" + output + "\" \"" + m_link + "\"";
    
    std::string dir_cmnd = "mkdir \"" + a_path + "\"";
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
        
        m_link = a_string;
    } else{
        std::cout << "No search result URLs found2." << std::endl;
    }

    return true;
}

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n");
    size_t last = str.find_last_not_of(" \t\n");
    if (first == std::string::npos || last == std::string::npos) {
        return "";
    }
    return str.substr(first, last - first + 1);
}
}//namespace m_player





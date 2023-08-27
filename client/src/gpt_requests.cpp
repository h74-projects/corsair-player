#include "gpt_requests.hpp"
#include <iostream>
#include <thread>
#include <chrono>


namespace m_player{

std::string GptRequest::generate_gpt_request(std::string a_request) {
    std::string command = "python3 ai_request.py '" + a_request + "'";

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Failed to run command." << std::endl;
        return "";
    }

    char buffer[512];
    std::string result = "";

    while (fgets(buffer, 512, pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);

    return result;
}

}//namespace m_player


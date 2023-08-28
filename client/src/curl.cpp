#include <iostream>
#include <fstream>
#include <map>
#include <json/json.h> 
#include <curl/curl.h> 

void saveReqToJson(const std::string& string, const std::string& file_name) 
{
    std::map<std::string, std::string> data;
    data["massege"] = stringValue;

    Json::Value json_data;
    for (const auto& entry : data) {
        json_data[entry.first] = entry.second;
    }

    std::ofstream json_file(file_name);
    if (json_file.is_open()) {
        json_file << json_data;
        json_file.close();
    } else {
        std::cerr << "Unable to open file: " << file_name << std::endl;
    }
}

int SendWndGetDataFromUrl(const std::string& file_name, const std::string& url, int type) 
{
    FILE* fp = fopen(file_name.c_str(), "rb");
    if (!fp) {
        std::cerr << "Failed to open JSON file." << std::endl;
        return 1;
    }

    switch (type) {
        case 1:
            url += /chat;
            break;
        case 2:
            url += /search
            break;
        case 3:
            url += /lyrics
            break;

    }

    C CURL* curl = curl_easy_init();
    if (curl) {
        //struct curl_slist* headers = nullptr;
        //headers = curl_slist_append(headers, "Content-Type: application/json");
        //curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_READDATA, fp);
        curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)-1);
        
        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            return 1;
        }
        
        // Now 'response' contains the server's response
        std::cout << "Server Response: " << response << std::endl;
        
        curl_easy_cleanup(curl);
    }

    fclose(fp);
    return 0;
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    std::string* response = static_cast<std::string*>(userp);
    response->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}







int main() {
    std::string inputString = "Hello, this is a sample string.";
    std::string outputJsonFile = "data.json";

    saveStringToJson(inputString, outputJsonFile);

    return 0;
}
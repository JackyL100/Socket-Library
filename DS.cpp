#include "TCP_SOCKET.hpp"

TCP_SOCKET::DS::DS(std::string &&msg) 
{
    data = msg;
}

TCP_SOCKET::DS::DS(const char* msg) {
    data = msg;
}

void TCP_SOCKET::DS::rewrite(const std::string &&new_data) {
    data = std::move(new_data);
}

TCP_SOCKET::DS TCP_SOCKET::DS::operator+(const std::string &str) {
    data += str;
}

std::string TCP_SOCKET::DS::view_raw_data() {
    return data;
}

std::unordered_map<std::string_view, std::string_view> TCP_SOCKET::DS::parse() {
    std::unordered_map<std::string_view, std::string_view> unpacked_data;
    bool toggle = true;
    std::stringstream ss(data);  
    std::string prev_word;
    std::string word;
    while (ss >> word) { 
        if (toggle) {
            unpacked_data[word];
            prev_word = word;
        } else {
            unpacked_data[prev_word] = word;
        }
        toggle = !toggle;
    }
    return unpacked_data;
}
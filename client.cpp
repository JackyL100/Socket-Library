#include "TCP_SOCKET.hpp"

TCP_SOCKET::client::client(std::string hostname, int server_port) {
    struct sockaddr_in serv_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR OPENING SOCKET");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, hostname.c_str(), &serv_addr.sin_addr);
    if (connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) error("ERROR CONNECTING");
}

void TCP_SOCKET::client::receive(const int&& bufSize) {
    char buf[bufSize];
    int n;
    std::string str = "";
    do {
        n = recv(sockfd, buf, bufSize, 0);
        if (n < 0) {
        error("ERROR RECEIVING");
        } else {
            str += buf;
        }
    } while (n == bufSize);
    received = std::move(str);
}

void TCP_SOCKET::client::sendRequest(DS& msg) {
    if (send(sockfd, msg.view_raw_data().c_str(), msg.view_raw_data().size(), 0) < 0) {error("ERROR SENDING");}
}

std::string TCP_SOCKET::client::getReceived() {
    return received;
}
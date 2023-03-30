#include "TCP_SOCKET.hpp"

TCP_SOCKET::server::server(int port, int max_clients) {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("SERVER ERROR OPENING SOCKET");
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    if (bind(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {error("ERROR ON BINDING");}
    listen(sockfd, max_clients);
}

void TCP_SOCKET::server::kill() {
    alive = false;
}

TCP_SOCKET::DS TCP_SOCKET::server::get_new_message() {
    TCP_SOCKET::DS new_message = incoming_messages.front();
    incoming_messages.pop();
    return new_message;
}

void TCP_SOCKET::server::broadcast(DS server_message) {
    for (int i = 0; i < connected_clients.size(); i++) {
        if (send(i, server_message.view_raw_data().c_str(), server_message.view_raw_data().size(), 0) < 0) {error("ERROR SENDING SERVER MESSAGE");}
    }
}

void TCP_SOCKET::server::reading(int client_socket) {
    char buf[DS::MAX_DATA_SIZE];
    if (recv(client_socket, buf, DS::MAX_DATA_SIZE - 1,0) < 0) error("ERROR READING FROM CLIENT");
    incoming_messages.emplace(buf);
}

void TCP_SOCKET::server::accepting() {
    struct sockaddr_in cli_addr;
    socklen_t clilen;
    clilen = sizeof(cli_addr);
    int clisock = accept(sockfd, (sockaddr *)&cli_addr, &clilen);
    if (clisock < 0) {
        error("ERROR ON ACCEPT");
    } else {
        std::cout << "Accepted client\n";
    }
    readingThreads.push_back(std::thread([&](){while(alive){reading(clisock); std::this_thread::sleep_for(std::chrono::milliseconds(5));}}));
}
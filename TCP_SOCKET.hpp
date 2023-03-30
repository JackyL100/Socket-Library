#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string>
#include <string_view>
#include <vector>
#include <deque>
#include <utility>
#include <thread>
#include <bits/stdc++.h>
#include <unordered_map>
#include <queue>

#pragma once

namespace TCP_SOCKET {

    void error(const char* msg) {
        perror(msg);
        exit(1);
    }

    class DS {
        private:
            std::string data;
        public:
            static const int MAX_DATA_SIZE = 255;

            DS(std::string &&msg);

            DS(const char* msg);

            std::string view_raw_data();

            void rewrite(const std::string &&new_data);

            DS operator+(const std::string &str);

            std::unordered_map<std::string_view, std::string_view> parse();
    };  

    class client {
        private:
            int sockfd;  
            std::string received;        
        public:
            client(std::string hostname, int server_port);

            void receive(const int&& bufSize);

            void sendRequest(DS& msg);

            std::string getReceived();
    };

    class server {
        public:
            std::vector<int> connected_clients;

            std::queue<DS> incoming_messages;

            server(int port, int max_clients);

            void kill();

            void broadcast(DS server_message);

            DS get_new_message();
        private:
            std::vector<std::thread> readingThreads;

            std::thread sendingThread;

            std::thread acceptingThread;

            socklen_t clilen;

            struct sockaddr_in serv_addr, cli_addr;

            int sockfd;

            bool alive;

            void reading(int client_socket);

            void accepting();
    };
}
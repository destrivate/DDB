#include "server.hpp"
#include <vector>
#include <string>
#include <thread> 
#include <iostream>
#include <sstream>   

namespace ddb::server {
    void Server::handler(int socket){
        char buf[4096];
        
        while (true){
            int bytes = recv(socket,buf,sizeof(buf),0);
            if (bytes <= 0) {
                close(socket);
                break; 
            }

            std::string text = std::string(buf,bytes);
            std::vector<std::string> body;

            std::istringstream stream(text);
            std::string word;


            while (stream >> word) body.push_back(word);
            if (body.empty()) {
                continue; 
            }
            if(body.size() >= 2){
                ssize_t bs = 0;
                if (body[0] == "set"){
                    if(body.size() < 3){
                        continue;
                    }
                    std::string res = _core->set(body[1],body[2]);
                    bs = send(socket, res.c_str(), res.length(), 0);

                    
                }
                else if (body[0] == "get"){
                    std::string res = _core->get(body[1]);
                    bs = send(socket,res.c_str(),res.length(),0);
                }
                else if (body[0] == "del"){
                    std::string res = _core->del(body[1]);
                    bs = send(socket,res.c_str(),res.length(),0);
                }else{
                    std::string res = "SyntaxError";
                    bs = send(socket,res.c_str(),res.length(),0);
                }

                if(bs == -1){
                    close(socket);
                    break;
                }
                
            }

            
        }
    }
    Server::Server(storage::Core& core,const int& port) {
        _core = &core;
        _socket = socket(AF_INET,SOCK_STREAM,0);
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;

        bind(_socket,(sockaddr*)&addr, sizeof(addr));

        listen(_socket,SOMAXCONN);
        
    }
    void Server::start(){
        while (true) {
            sockaddr_in client;
            socklen_t clientSize = sizeof(client);

            int clientSocket = accept(_socket, (sockaddr*)&client, &clientSize);
            if (clientSocket == -1) {
                std::cerr << "Accept failed." << std::endl; 
                break;
            }
            int flag = 1;
            setsockopt(clientSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int));
            std::thread(&Server::handler, this, clientSocket).detach();
        }
    }
    Server::~Server() {
        close(_socket);
    }
}

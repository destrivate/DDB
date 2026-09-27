#pragma once
#include "../storage/ddb.hpp"
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/tcp.h>

namespace server{
	class Server {
        private:
            void handler(int socket);
            int _socket;
	public:
            storage::Core* _core;
            Server(storage::Core& core,const int& port);
            void start();
            ~Server();
                   
	};
	
}




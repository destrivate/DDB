#pragma once
#include "ddb/ddb.hpp"
#include <string>
#include <thread> 
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#pragma comment(lib, "Ws2_32.lib")  

namespace server{
	class Server {
        private:
            void handler(int socket);
            int _socket;
	public:
            ddb::Core* _core;
            Server(ddb::Core& core,const int& port);
            void start();
            ~Server();
                   
	};
	
}




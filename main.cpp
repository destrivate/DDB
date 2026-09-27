#include <fstream>
#include <sstream>
#include "server/server.hpp"
#include "storage/ddb.hpp"

constexpr int PORT = 9122;

using namespace storage;
using namespace server;

void loadData(Core& core);

int main() {
    Core core;
    loadData(core);
    Server server(core,PORT); 

    server.start();

    return 0;
}

void loadData(Core& core){
    std::ifstream file("default.txt");

    if(!file.is_open()){
        return;
    }

    std::string line;

    while(std::getline(file,line)){
        std::stringstream ss(line);
        std::string key,value;

        std::getline(ss, key, '|'); 
        std::getline(ss, value);

        core.set(key,value);
    }

    file.close();

}

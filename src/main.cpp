#include <fstream>
#include <sstream>
#include "server/server.hpp"
#include "storage/ddb.hpp"
#include "../include/config_utility.hpp"
#include "../include/logger.hpp"

using namespace ddb;
using namespace include;

void write_default_data(storage::Core&,const std::string);

int main() {
    config_utility::Config config("config");

    printf("=======\n\n\tAuthor - Destrivate\n\tGitHub - https://github.com/destrivate/DDB\n\tCopyright (c) 2026 DDB\n\n=======\n\n");
    logger::info("The server listens to the port: " + config.get_str("PORT","9122"));

    storage::Core core;

    write_default_data(core,config.get_str("PATH_DEFAULT","default.txt"));

    server::Server server(core,config.get_int("PORT",9122)); 

    server.start(); 

    return 0;
}

void write_default_data(storage::Core& core,const std::string file_path){
    std::ifstream file(file_path);

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

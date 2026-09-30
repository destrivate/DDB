#include "../config/config.hpp"

namespace config {
    DefaultManager::DefaultManager(storage::Core& core) : _core(core){
        
    }

    DefaultManager::~DefaultManager(){

    }

    void DefaultManager::write_default_data(std::string file_path){
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

            this->_core.set(key,value);
        }

        file.close();
    }
    
}
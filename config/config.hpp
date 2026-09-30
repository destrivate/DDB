#pragma once

#include <string>
#include <fstream>
#include <sstream>   
#include "../storage/ddb.hpp"

namespace config {
    class DefaultManager {
        private:
            storage::Core& _core;
        public:
            DefaultManager(storage::Core& core);
            ~DefaultManager();
            void write_default_data(std::string file_path);
    };
    class ConfigManager {

    };
}
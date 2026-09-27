#pragma once
#include <unordered_map> 
#include <string>
#include <shared_mutex>
#include <shared_mutex>

namespace storage {
    class Core {
        private:
            std::unordered_map<std::string, std::string> db;
            mutable std::shared_mutex mutex_;
        public:
            std::string set(const std::string& key,const std::string& value);
            std::string get(const std::string& key);
            std::string del(const std::string& key);
    };
}
#include "ddb.hpp"
#include <mutex>    

namespace ddb::storage {
    std::string Core::set(const std::string& key, const std::string& value) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        if (db.count(key) > 0) {
            db[key] = value;
            return "Updated";
        }
        db[key] = value;
        return "Created";
    }


    std::string Core::get(const std::string& key){
        std::shared_lock<std::shared_mutex> lock(mutex_);
        if(db.count(key) > 0){
            return db[key];
        }      
        return "NoneValue";
    }
    std::string Core::del(const std::string& key){
        std::unique_lock<std::shared_mutex> lock(mutex_);
        if(db.count(key) > 0){
            db.erase(key);
            return "Success";
        }
        return "NoneValue";
    }
}

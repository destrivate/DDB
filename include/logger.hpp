#pragma once


#include <string>

namespace include::logger {

    inline void log(const std::string& level, const std::string& message) {
        printf("[%s] %s\n", level.c_str(), message.c_str());
    }

    inline void info(const std::string& message) {
        log("INFO", message);
    }

    inline void warning(const std::string& message) {
        log("WARNING", message);
    }

    inline void error(const std::string& message) {
        log("ERROR", message);
    }

}

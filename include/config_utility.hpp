#pragma once

#include <fstream>
#include <string>
#include <map>

namespace include::config_utility {
    class Config {
        private:
            std::map<std::string, std::string> _data;

        public:
            Config(const std::string& path) {
                std::ifstream f(path);
                if (!f.is_open()) return;
                std::string line;
                while (std::getline(f, line)) {
                    if (line.empty() || line[0] == '#') continue;
                    auto pos = line.find('=');
                    if (pos == std::string::npos) continue;
                    _data[line.substr(0, pos)] = line.substr(pos + 1);
                }
            }

            std::string get_str(const std::string& k, const std::string& d = "") const {
                auto it = _data.find(k);
                return it != _data.end() ? it->second : d;
            }

            int get_int(const std::string& k, int d = 0) const {
                auto it = _data.find(k);
                return it != _data.end() ? std::stoi(it->second) : d;
            }

            bool get_bool(const std::string& k, bool d = false) const {
                auto it = _data.find(k);
                return it != _data.end() ? (it->second == "true" || it->second == "1") : d;
            }
    };   
}

#include <fstream>
#include <sstream>
#include "server/server.hpp"
#include "storage/ddb.hpp"
#include "config/config.hpp"

constexpr int PORT = 9122;

using namespace storage;
using namespace server;
using namespace config;

void loadData(Core& core);

int main() {
    Core core;
    DefaultManager defaultManager(core);
    defaultManager.write_default_data("default.txt");

    Server server(core,PORT); 

    server.start();

    return 0;
}

#!/bin/bash
set -e

g++ -Wall -Wextra -Werror -O3 -I include src/main.cpp src/server/*.cpp src/storage/*.cpp -o build/server_app   

chmod +x ./build/server_app

./build/server_app


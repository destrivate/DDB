#!/bin/bash
set -e

g++ -Wall -Wextra -Werror -O3 -I. main.cpp config/*.cpp server/*.cpp storage/*.cpp -o server_app

chmod +x ./server_app

./server_app


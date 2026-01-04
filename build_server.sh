#!/bin/bash
# Script to build server
cd "$(dirname "$0")"
rm -f build/server
g++ -o build/server src/server/Server.cpp \
    -I/usr/include/cppconn \
    -Isrc -Isrc/controllers -Isrc/models -Isrc/repository -Isrc/utils -Idata \
    -lmysqlcppconn -lpthread -std=c++11 -Wall

if [ $? -eq 0 ]; then
    echo "Server built successfully: build/server"
else
    echo "Build failed"
    exit 1
fi

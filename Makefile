# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I/usr/include/cppconn -Isrc
LDFLAGS = -lmysqlcppconn -lpthread

# Directories
SRC_DIR = src
BUILD_DIR = build
DATA_DIR = data

# Source files
SERVER_SRC = $(SRC_DIR)/server/Server.cpp
CLIENT_SRC = $(SRC_DIR)/client/Client.cpp
MAIN_SRC = $(SRC_DIR)/main.cpp

# Output binaries
SERVER_BIN = $(BUILD_DIR)/server
CLIENT_BIN = $(BUILD_DIR)/client
MAIN_BIN = $(BUILD_DIR)/main

# Default target
.PHONY: all
all: $(BUILD_DIR) $(SERVER_BIN) $(CLIENT_BIN)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build server
$(SERVER_BIN): $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

# Build client
$(CLIENT_BIN): $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

# Build main (if needed)
$(MAIN_BIN): $(MAIN_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

# Clean build files
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*.o $(SERVER_BIN) $(CLIENT_BIN) $(MAIN_BIN)
	rm -f server_logs.txt

# Clean all
.PHONY: cleanall
cleanall:
	rm -rf $(BUILD_DIR)
	rm -f server_logs.txt

# Run server
.PHONY: run-server
run-server: $(SERVER_BIN)
	./$(SERVER_BIN)

# Run client
.PHONY: run-client
run-client: $(CLIENT_BIN)
	./$(CLIENT_BIN)

# Run main
.PHONY: run-main
run-main: $(MAIN_BIN)
	./$(MAIN_BIN)

# Help
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all         - Build server and client (default)"
	@echo "  clean       - Remove build files and logs"
	@echo "  cleanall    - Remove build directory entirely"
	@echo "  run-server  - Build and run server"
	@echo "  run-client  - Build and run client"
	@echo "  run-main    - Build and run main"
	@echo "  help        - Show this help message"

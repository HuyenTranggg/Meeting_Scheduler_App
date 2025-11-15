#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include <unistd.h>      // Dùng cho hàm close()
#include <sys/socket.h>  // Thư viện socket chính
#include <netinet/in.h>  // Dùng cho cấu trúc sockaddr_in

using namespace std;

class Server {
public:
    void init();
    void run();
    void handleClient(int clientSocket);

private:
    int serverSocket;
    vector<string> logs;

    void logMessage(const string& message) {
        logs.push_back(message);
        ofstream logFile("server_logs.txt", ios::app);
        logFile << message << endl;
        logFile.close();
    }
};

void Server::init() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0); // IPPROTO_TCP thường là 0

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);
}

void Server::run() {
    cout << "Server running on port 8080...\n";
    while (true) {
        int clientSocket = accept(serverSocket, NULL, NULL);
        thread(&Server::handleClient, this, clientSocket).detach();
    }
}

void Server::handleClient(int clientSocket) {
    char buffer[1024];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        string message(buffer);
        logMessage("Received: " + message);

        string response = "Server received your message.";
        send(clientSocket, response.c_str(), response.size(), 0);
    }
    close(clientSocket);
}

int main() {
    Server server;
    server.init();
    server.run();
    return 0;
}

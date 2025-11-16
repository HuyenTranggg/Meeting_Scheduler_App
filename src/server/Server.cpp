#include "../Status.h"
#include "../controllers/UserController.h"
#include "../models/Response.h"
#include "../models/User.h"
#include "../repository/UserRepository.h"
#include "../utils/MessageUtils.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include <unistd.h>      // Dùng cho hàm close()
#include <sys/socket.h>  // Thư viện socket chính
#include <netinet/in.h>  // Dùng cho cấu trúc sockaddr_in
#define PORT 8080
#define BUFFER_SIZE 1024

using namespace std;

UserController userController;
UserRepository userRepo;

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
    // Tạo socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0); // IPPROTO_TCP thường là 0
    if (serverSocket < 0) {
        cerr << "Error creating socket" << endl;
        exit(1);
    }

    // Cấu hình địa chỉ server
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Error binding socket" << endl;
        close(serverSocket);
        exit(1);
    }

    if (listen(serverSocket, 5) < 0) {
        cerr << "Error listening on socket" << endl;
        close(serverSocket);
        exit(1);
    }
}

void Server::run() {
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    cout << "Server running on port 8080...\n";
    while (true) {
        // Chấp nhận kết nối từ client
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);
        if (clientSocket < 0) {
            cerr << "Error accepting connection" << endl;
            continue;
        }
        // Tạo thread mới để xử lý client
        thread(&Server::handleClient, this, clientSocket).detach();
    }
}

// Hàm ghi log vào file
void logToFile(const std::string &message) {
    ofstream logFile("server_logs.txt", ios::app);
    if (logFile.is_open()) {
        logFile << message << std::endl;
        logFile.close();
    }
}

vector<string> splitString(const string &str, char delimiter) {
    vector<string> tokens;
    string token;
    stringstream ss(str);

    // Sử dụng getline để tách chuỗi
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

bool endsWith(const string &str, const string &suffix) {
    if (str.size() >= suffix.size()) {
        return str.substr(str.size() - suffix.size()) == suffix;
    }
    return false;
}

void processClientRequest(int clientSocket, const string &request) {
    string response;
    string command = request.substr(0, request.find('|'));
    vector<string> result = splitString(request, '|');
    Response res;

    if (command == "REGISTER") {
        res = userController.registerAccount(request);
    } else if (command == "LOGIN") {
        string username = result[1];
        string password = result[2];
        res = userController.login(username, password);
    } else {
        response = MessageUtils::createMessage(Status::UNKNOWN_ERROR, "Invalid request");
    }

    response = to_string(res.getStatus()) + "|" + res.getMessage();

    // Gửi phản hồi về client
    int totalSize = response.size();
    int offset = 0;
    // Gửi chuỗi theo từng phần nhỏ
    while (offset < totalSize) {
        // Tính toán độ dài của phần gửi
        int partSize = min(BUFFER_SIZE, totalSize - offset);

        // Gửi phần chuỗi
        int bytesSent = send(clientSocket, response.c_str() + offset, partSize, 0);
        string sentResponse(response.c_str() + offset, bytesSent);
        // Ghi log response vào file
        logToFile("Server response: " + sentResponse);

        if (bytesSent < 0) {
            cerr << "Error sending data!" << endl;
            break;
        }

        // Cập nhật offset để gửi phần tiếp theo
        offset += bytesSent;
    }
}

void Server::handleClient(int clientSocket) {
    char buffer[BUFFER_SIZE];
    string partialMessage;
    while (true) {
        // Nhận message từ client
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (bytesReceived <= 0) {
            break; // Ngắt kết nối
        }

        string request(buffer, bytesReceived);
        cout << "Received: " << request << endl;

        // Ghi log request vào file
        logToFile("Received: " + request);

        // Xử lý yêu cầu từ client
        if (endsWith(request, "|<END>")) {
            partialMessage += request;
            partialMessage = partialMessage.substr(0, partialMessage.length() - 6);
            processClientRequest(clientSocket, partialMessage); // Process the full message
            partialMessage.clear();                             // Clear buffer for next message
        } else {
            partialMessage += request; // Append to the existing buffer
        }
    }
    close(clientSocket);
}

int main() {
    Server server;
    server.init();
    server.run();
    return 0;
}

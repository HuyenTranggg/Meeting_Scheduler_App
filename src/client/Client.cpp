#include <iostream>
#include <string>
#include <unistd.h>      // Dùng cho hàm close()
#include <sys/socket.h>  // Thư viện socket chính
#include <arpa/inet.h>   // Dùng cho hàm inet_addr
#include <netinet/in.h>  // Dùng cho cấu trúc sockaddr_in
#include <cerrno>        // Dùng cho việc kiểm tra lỗi
#include <cstring>       // Dùng cho strerror

using namespace std;

class Client {
public:
    void init();
    void connectToServer();
    void sendMessage(const string& message);
    void receiveMessage();
    int clientSocket;
};

void Client::init() {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) { // Lỗi trả về -1
        cerr << "Socket creation failed: " << strerror(errno) << endl;
        exit(1);
    }
}

void Client::connectToServer() {
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int result = connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (result < 0) { // Lỗi trả về -1
        cerr << "Connect failed: " << strerror(errno) << endl;
        close(clientSocket);
        exit(1);
    }
    cout << "Connected to the server successfully!" << endl;
}

void Client::sendMessage(const string& message) {
    int result = send(clientSocket, message.c_str(), message.size(), 0);
    if (result < 0) { // Lỗi trả về -1
        cerr << "Send failed: " << strerror(errno) << endl;
        close(clientSocket);
        exit(1);
    }
    cout << "Message sent: " << message << endl;
}

void Client::receiveMessage() {
    char buffer[1024];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        cout << "Received from server: " << buffer << endl;
    }
    else {
        cerr << "Receive failed: " << strerror(errno) << endl;
    }
}

int main() {
    Client client;
    client.init();
    client.connectToServer();
    client.sendMessage("Hello, Server from Ubuntu!");
    client.receiveMessage();
    close(client.clientSocket);
    return 0;
}
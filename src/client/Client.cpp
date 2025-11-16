#include <iostream>
#include <string>
#include <unistd.h>      // Dùng cho hàm close()
#include <sys/socket.h>  // Thư viện socket chính
#include <arpa/inet.h>   // Dùng cho hàm inet_addr
#include <netinet/in.h>  // Dùng cho cấu trúc sockaddr_in
#include <cerrno>        // Dùng cho việc kiểm tra lỗi
#include <cstring>       // Dùng cho strerror

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

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
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

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
    char buffer[BUFFER_SIZE];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        cout << "Received from server: " << buffer << endl;
    }
    else {
        cerr << "Receive failed: " << strerror(errno) << endl;
    }
}
void registerView(Client client) {
    int choice;
    string role, username, password, first_name, last_name;

    // Chọn vai trò
    cout << "Please choose your role:" << endl;
    cout << "1. Student\n2. Teacher \n";
    cin >> choice;
    cin.ignore();

    switch (choice) {
    case 1:
        role = "student";
        break;
    case 2:
        role = "teacher";
        break;
    default:
        cout << "Please choose a valid option." << endl;
        return;
    }

    // Nhập thông tin đăng ký
    cout << "Please enter your username: ";
    getline(cin, username);
    cout << "Please enter your password: ";
    getline(cin, password);
    cout << "Please enter your first name: ";
    getline(cin, first_name);
    cout << "Please enter your last name: ";
    getline(cin, last_name);

    // Tạo message register và gửi đến server
    string registerCommand = "REGISTER|" + username + "|" + password + "|" + role + "|" + first_name + "|" + last_name + "|<END>";
    client.sendMessage(registerCommand);
    client.receiveMessage();

}

void loginView(Client client) {
    string username, password;
    cout << "Please enter your username: ";
    getline(cin, username);
    cout << "Please enter your password: ";
    getline(cin, password);

    // Tạo message login và gửi đến server
    string loginCommand = "LOGIN|" + username + "|" + password + "|<END>";
    client.sendMessage(loginCommand);
    client.receiveMessage();
}

void handleUserCommand(Client client) {
    bool loop = true;
    while (loop) {
        int choice;
        // Chọn lenh
        cout << "Please choose one number:" << endl;
        cout << "1. Login\n2. Register\n3. Exit \n";
        cin >> choice;
        cin.ignore();
        switch (choice) {
            case 1:
                loginView(client);
                break;
            case 2:
                registerView(client);
                break;
            case 3:
                loop = false;
                break;
            default:
                cout << "Please choose a valid option." << endl;
                return;
        }
    }
}

int main() {
    Client client;
    client.init();
    client.connectToServer();
    handleUserCommand(client);
    // client.sendMessage("Hello, Server from Ubuntu!");
    // client.receiveMessage();
    close(client.clientSocket);
    return 0;
}
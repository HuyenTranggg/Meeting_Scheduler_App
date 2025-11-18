#include "../controllers/TeacherResponseController.h"
#include "../controllers/UserController.h"

#include "../views/TeacherView.h"
#include "../models/Timeslot.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
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

void handleTeacherMenu();
void handleDeclareTimeSlot();

// Global variables
int clientSocket = -1;
int user_id = 0;
string role = "none";
TeacherView teacherView;

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

void logout() {
    user_id = 0;
    role = "none";
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

string sendRequestToServer(const string &command) {
    int totalSize = command.size();
    int offset = 0;

    // Gửi chuỗi theo từng phần nhỏ
    while (offset < totalSize) {
        // Tính toán độ dài của phần gửi
        int partSize = min(BUFFER_SIZE, totalSize - offset);

        // Gửi phần chuỗi
        int bytesSent = send(clientSocket, command.c_str() + offset, partSize, 0);

        if (bytesSent < 0) {
            cerr << "Lỗi khi gửi dữ liệu!" << endl;
            break;
        }

        // Cập nhật offset để gửi phần tiếp theo
        offset += bytesSent;
    }

    // Nhận phản hồi từ server
    char buffer[BUFFER_SIZE];
    string response;
    while (true) {
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (bytesReceived <= 0) {
            break; // Kết thúc khi không nhận thêm dữ liệu hoặc có lỗi
        }
        // Chuyển buffer thành string
        string partResponse(buffer, bytesReceived);

        // Thêm phần phản hồi vào response
        response += partResponse;

        if (endsWith(partResponse, "|<END>")) {
            response = response.substr(0, response.length() - 6);
            break;
        }
    }

    return response.empty() ? "100" : response;
}

// Student
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

// Parse login response và lưu user_id, role
// Response format: 0|message|user_id|role
void parseLoginResponse(const string& response) {
    vector<string> tokens = splitString(response, '|');
    if (tokens.size() > 0) {
        int status = stoi(tokens[0]);
        if (status == 0 && tokens.size() >= 4) {
            user_id = stoi(tokens[2]);
            role = tokens[3];
            cout << tokens[1] << "! Welcome, " << role << " (ID: " << user_id << ")" << endl;
        } else {
            cout << "Login failed!" << endl;
            if (tokens.size() > 1) {
                cout << tokens[1] << endl;
            }
        }
    }
}

void loginView(Client client) {
    string username, password;
    cout << "Please enter your username: ";
    getline(cin, username);
    cout << "Please enter your password: ";
    getline(cin, password);

    // Tạo message login và gửi đến server
    string loginCommand = "LOGIN|" + username + "|" + password + "|<END>";
    string response = sendRequestToServer(loginCommand);
    cout << "Server response: " << response << "|<END>"<<endl;
    
    // Parse response để lấy user_id và role
    parseLoginResponse(response);
}

void handleUserCommand(Client client) {
    bool loop = true;
    while (loop) {
        int choice;
        cout << "Please choose one number:" << endl;
        cout << "1. Login\n2. Register\n3. Exit \n";
        cin >> choice;
        cin.ignore();
        switch (choice) {
            case 1:
                loginView(client);
                // Nếu login thành công, vào menu tương ứng
                if (user_id > 0) {
                    if (role == "teacher") {
                        handleTeacherMenu();
                    } else if (role == "student") {
                        cout << "Student menu chua duoc trien khai!" << endl;
                    }
                }
                break;
            case 2:
                registerView(client);
                break;
            case 3:
                loop = false;
                break;
            default:
                cout << "Please choose a valid option." << endl;
                break;
        }
    }
}

// Teacher
void handleDeclareTimeSlot() {
    Timeslot ts = teacherView.showDeclareTimeSlots(user_id);
    string request = "DECLARE_TIME_SLOT|" + ts.toStringDeclare() + "|<END>";
    string response = sendRequestToServer(request);
    string status = response.substr(0, response.find('|'));
    if (status == "0") {
        vector<string> tokens = splitString(response, '|');
        cout << tokens[1] << endl;
    } else if (status == "13") {
        vector<string> tokens = splitString(response, '|');
        cout << tokens[1] << endl;
    }
}

void handleTeacherMenu() {
    int choice = teacherView.showMenu();
    switch (choice) {
    case 0:
        logout();
        break;
    case 1:
        handleDeclareTimeSlot();
        handleTeacherMenu();
        break;

    default:
        break;
    }
}

int main() {
    Client client;
    client.init();
    client.connectToServer();
    clientSocket = client.clientSocket; // Gán socket vào biến global
    handleUserCommand(client);
    // client.sendMessage("Hello, Server from Ubuntu!");
    // client.receiveMessage();
    close(client.clientSocket);
    return 0;
}

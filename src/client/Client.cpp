#include <iostream>
#include <string>
#include <unistd.h>      // Dùng cho hàm close()
#include <sys/socket.h>  // Thư viện socket chính
#include <arpa/inet.h>   // Dùng cho hàm inet_addr
#include <netinet/in.h>  // Dùng cho cấu trúc sockaddr_in
#include <cerrno>        // Dùng cho việc kiểm tra lỗi
#include <cstring>       // Dùng cho strerror
#include "../views/UserView.h"
#include "../views/TeacherView.h"
#include "../models/Timeslot.h"
#include <vector>
#include <sstream>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

using namespace std;

int clientSocket;
struct sockaddr_in serverAddr;
TeacherView teacherView;
int user_id = 0;
string role = "none";
UserView userView;
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

void connectToServer() {
    // Tạo socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        cerr << "Socket creation failed: " << strerror(errno) << endl;
        exit(1);
    }

    // Cấu hình địa chỉ server
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    // Kết nối tới server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Connect failed: " << strerror(errno) << endl;
        close(clientSocket);
        exit(1);
    }
    cout << "Connected to the server successfully!" << endl;
}

bool endsWith(const string &str, const string &suffix) {
    if (str.size() >= suffix.size()) {
        return str.substr(str.size() - suffix.size()) == suffix;
    }
    return false;
}

string sendRequestToServer(const string &command) {
    // send(clientSocket, command.c_str(), command.size(), 0);
    int totalSize = command.size();
    int offset = 0;

    // Gửi chuỗi theo từng phần nhỏ
    while (offset < totalSize) {
        // Tính toán độ dài của phần gửi
        int partSize = min(BUFFER_SIZE, totalSize - offset);

        // Gửi phần chuỗi
        int bytesSent = send(clientSocket, command.c_str() + offset, partSize, 0);

        if (bytesSent < 0) {
            cerr << "Send failed: " << strerror(errno) << endl;
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

        cout << "Server Response: " + response << endl;
        if (endsWith(partResponse, "|<END>")) {
            response = response.substr(0, response.length() - 6);
            break;
        }
    }

    return response.empty() ? "100" : response;
}
void handleUserMenu();

void logout() {
    user_id = 0;
    role = "none";
}
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
void handleTeacherMenu(){
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

void handleStudentMenu(){}
void handleLogin() {
    map<string, string> info = userView.showLogin();
    string loginCommand = "LOGIN|" + info["username"] + "|" + info["password"] + "|<END>";
    cout << "Request Sent: " + loginCommand << endl;
    string response = sendRequestToServer(loginCommand);
    
    // Phân tích phản hồi từ server
    vector<string> result = splitString(response, '|');
    if (result.size() >= 4 && result[0] == "0" && !result[2].empty()) {
        user_id = stoi(result[2]);
        role = result[3];
        cout << user_id << " - " << role << endl;
        response = result[0] + "|" + result[1];
    } else if (result.size() >= 2 && (result[0] == "1" || result[0] == "2")) {
        cout << result[1] << endl;
        handleLogin();
    } else {
        logout();
    }
    if (role == "teacher" && user_id != 0) {
        handleTeacherMenu();
    } else if (role == "student" && user_id != 0) {
        handleStudentMenu();
    }
}

void handleRegister() {
    map<string, string> info = userView.showRegisterA(); // Lấy thông tin đăng ký
    string registerCommand = "REGISTER|" + info["username"] + "|" + info["password"] + "|" + info["role"] + "|" +
                             info["first_name"] + "|" + info["last_name"] + "|<END>";
    cout << "Request Sent: " + registerCommand << endl;
    // Gửi lệnh đăng ký tới server
    string response = sendRequestToServer(registerCommand);
    
    // Phân tích phản hồi từ server
    vector<string> result = splitString(response, '|');
    if (result.size() >= 4 && result[0] == "0" && !result[2].empty()) {
        vector<string> tokens = splitString(response, '|');
        cout << tokens[1] << endl;
        user_id = stoi(tokens[2]);
        role = tokens[3];
        if (role == "teacher" && user_id != 0) {
            handleTeacherMenu();
        } else if (role == "student" && user_id != 0) {
            handleStudentMenu();
        }
    } else if (result.size() >= 2 && result[0] == "3") {
        cout << result[1] << endl;
    }
}

void handleUserMenu() {
    int choice = userView.showMenu();
    switch (choice) {
    case 1:
        handleLogin();
        handleUserMenu();
        break;
    case 2:
        handleRegister();
        handleUserMenu();
        break;
    default:
        break;
    }
}

void closeConnection() { close(clientSocket); }

int main() {
    
    connectToServer();
    handleUserMenu();
    closeConnection();
}

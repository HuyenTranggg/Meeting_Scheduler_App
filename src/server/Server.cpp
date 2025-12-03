#include "../Status.h"
#include "../controllers/UserController.h"
#include "../controllers/TeacherResponseController.h"
#include "../models/Response.h"
#include "../models/User.h"
#include "../repository/UserRepository.h"
#include "../utils/MessageUtils.h"
#include "../controllers/TeacherResponseController.h"
#include "../controllers/StudentResponseController.h"
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <map>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include <unistd.h>      // Dùng cho hàm close()
#include <sys/socket.h>  // Thư viện socket chính
#include <netinet/in.h>  // Dùng cho cấu trúc sockaddr_in
#define PORT 8080
#define BUFFER_SIZE 2024

using namespace std;

UserController userController;
TeacherResponseController teacherResponseController;
StudentResponseController studentResponseController;
UserRepository userRepo;
// Hàm ghi log vào file
void logToFile(const string &message) {
    ofstream logFile("server_logs.txt", ios::app);
    if (logFile.is_open()) {
        logFile << message << std::endl << std::endl;
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
    } else if (command == "DECLARE_TIME_SLOT") {
        res = teacherResponseController.declareTimeslot(request);
    } else if (command == "FETCH_ALL_TEACHER") {
        res = studentResponseController.getAllTeacher();
    } else if (command == "BOOK_MEETING") {
        res = studentResponseController.bookMeeting(request);
    } else if (command == "VIEW_FREE_TIME_SLOTS") {
        int teacher_id = stoi(result[1]);
        res = teacherResponseController.viewTimeslots(teacher_id);
    } else if (command == "EDIT_SLOT") {

        string params = "";
        size_t first_pipe = request.find('|');
        if (first_pipe != string::npos) {
            params = request.substr(first_pipe + 1);
        }
        res = teacherResponseController.updateTimeslot(params);
    } else if (command == "VIEW_MEETINGS_STUDENT") {
        res = studentResponseController.viewMeetingsStudent(request);
    } else if (command == "CANCEL_APPOINTMENT") {
        res = studentResponseController.cancelMeeting(request);
    } else if (command == "VIEW_MEETING_STUDENT") {
        res = studentResponseController.viewMeetingStudent(request);
    } else {
        response = MessageUtils::createMessage(Status::UNKNOWN_ERROR, "Invalid request");
    }

    response = to_string(res.getStatus()) + "|" + res.getMessage();
    cout << "Server response: " << response << endl;

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
        logToFile("Server response: " + sentResponse);

        if (bytesSent < 0) {
            cerr << "Error sending data!" << endl;
            break;
        }

        // Cập nhật offset để gửi phần tiếp theo
        offset += bytesSent;
    }
}

void handleClient(int clientSocket) {
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

    // Đóng kết nối khi xử lý xong
    close(clientSocket);
}

void startServer() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    // Tạo socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        cerr << "Error creating socket" << endl;
        return;
    }

    // Cấu hình địa chỉ server
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Gắn socket vào địa chỉ
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Error binding socket" << endl;
        close(serverSocket);
        return;
    }

    // Lắng nghe kết nối
    if (listen(serverSocket, 5) < 0) {
        cerr << "Error listening on socket" << endl;
        close(serverSocket);
        return;
    }

    cout << "Server running on port 8080...\n";

    while (true) {
        // Chấp nhận kết nối từ client
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);
        if (clientSocket < 0) {
            cerr << "Error accepting connection" << endl;
            continue;
        }

        // Tạo thread mới để xử lý client
        thread(handleClient, clientSocket).detach();
    }

    // Đóng socket server
    close(serverSocket);
}

int main() {
    startServer();
    return 0;
}

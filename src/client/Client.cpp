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
#include "../views/StudentView.h"
#include "../models/Meeting.h"
#include "../controllers/StudentController.h"
#include <vector>
#include <sstream>

#include "../controllers/TeacherController.h"
#include "../controllers/UserController.h"

#include "../utils/MessageUtils.h"
#include "../views/TeacherView.h"
#include "../views/UserView.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 2500

using namespace std;

int clientSocket;
struct sockaddr_in serverAddr;
TeacherView teacherView;
StudentView studentView;
StudentController studentController;
int user_id = 0;
string role = "none";

UserView userView;
UserController userController;
TeacherController teacherController;

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

// Hàm an toàn để chuyển string sang int
int safeStoi(const string &str, int defaultValue = 0) {
    try {
        if (str.empty()) return defaultValue;
        return stoi(str);
    } catch (const invalid_argument &e) {
        cerr << "Invalid argument for stoi: '" << str << "'" << endl;
        return defaultValue;
    } catch (const out_of_range &e) {
        cerr << "Out of range for stoi: '" << str << "'" << endl;
        return defaultValue;
    }
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

// Teacher

void handleUpdateTimeslot(const Timeslot &timeslot) {
    Timeslot tsUpdate = teacherView.showUpdateTimeslot(timeslot);
    string request = "EDIT_SLOT|" + to_string(user_id) + "|" + tsUpdate.toStringUpdate() + "|<END>";
    string response = sendRequestToServer(request);
    string status = response.substr(0, response.find('|'));
    if (status == "0") {
        vector<string> tokens = splitString(response, '|');
        cout << tokens[1] << endl;
    }
}

void handleViewTimeslots() {
    string request = "VIEW_FREE_TIME_SLOTS|" + to_string(user_id) + "|<END>";
    string response = sendRequestToServer(request);
    string status = response.substr(0, response.find('|'));
    if (status == "0") {
        map<string, vector<Timeslot>> timeslots = teacherController.viewTimeslots(response);
        Timeslot ts = teacherView.showTimeslots(timeslots);
        if (ts.getId() == -1) {
            return;
        }
        // Detail Timeslot
        int choice = teacherView.showTimeslot(ts);
        if (choice == 0) {
            handleViewTimeslots();
        } else {
            handleUpdateTimeslot(ts);
            handleViewTimeslots();
        }

    } else if (status == "4") {
        vector<string> tokens = splitString(response, '|');
        cout << tokens[1] << endl;
    } else if (status == "15") {
        vector<string> tokens = splitString(response, '|');
        cout << tokens[1] << endl;
    }
}

void handleTeacherViewMeeting(const int &meeting_id) {
    string request = "VIEW_MEETING|" + to_string(meeting_id) + "|<END>";
    string response = sendRequestToServer(request);
    string status = response.substr(0, response.find('|'));
    if (status == "0") {
        pair<Meeting, vector<User>> meetingDetail = teacherController.getMeetingFromResponse(response);
        int choice = teacherView.showMeeting(meetingDetail.first, meetingDetail.second);
        if (choice == 0) {
            return;
        }
    } else if (status == "12") {
        return;
    }
}

void handleTeacherViewMeetings() {
    string request = "VIEW_MEETINGS|" + to_string(user_id) + "|<END>";
    string response = sendRequestToServer(request);
    string status = response.substr(0, response.find('|'));
    if (status == "0") {
        map<string, vector<Meeting>> meetings = teacherController.getMeetingsFromResponse(response);
        Meeting meeting = teacherView.showMeetings(meetings);
        if (meeting.getId() == -1) {
            return;
        }
        // Detail Meeting
        handleTeacherViewMeeting(meeting.getId());
        handleTeacherViewMeetings();
    } else if (status == "16") {
        vector<string> tokens = splitString(response, '|');
        cout << tokens[1] << endl;
    }
}

void handleTeacherViewMeetingsByDate() {
    // Get date from user
    string day, month, year;
    cout << "Nhap ngay can xem (dd mm yyyy):" << endl;
    cout << "Ngay: ";
    cin >> day;
    cout << "Thang: ";
    cin >> month;
    cout << "Nam: ";
    cin >> year;
    cin.ignore();
    
    string date = year + "-" + month + "-" + day;
    
    string request = "VIEW_MEETINGS|" + to_string(user_id) + "|" + date + "|<END>";
    string response = sendRequestToServer(request);
    string status = response.substr(0, response.find('|'));
    
    if (status == "0") {
        vector<pair<Meeting, vector<int>>> meetings = teacherController.getMeetingsByDateFromResponse(response);
        if (meetings.empty()) {
            cout << "Khong co lich hen nao trong ngay " << day << "/" << month << "/" << year << endl;
            return;
        }
        
        Meeting selectedMeeting = teacherView.showMeetingsByDate(meetings, date);
        if (selectedMeeting.getId() == -1) {
            return;
        }
        
        // Detail Meeting
        handleTeacherViewMeeting(selectedMeeting.getId());
        // Return to date view after viewing details
        handleTeacherViewMeetingsByDate();
    } else {
        cout << "Loi khi lay danh sach lich hen" << endl;
    }
}

void handleTeacherViewHistoryMeeting(const int &meeting_id) {
    string request = "VIEW_MEETING|" + to_string(meeting_id) + "|<END>";
    string response = sendRequestToServer(request);
    string status = response.substr(0, response.find('|'));
    if (status == "0") {
        pair<Meeting, vector<User>> meetingDetail = teacherController.getMeetingFromResponse(response);
        int choice = teacherView.showMeetingHistory(meetingDetail.first, meetingDetail.second);
        if (choice == 0) {
            return;
        }
    }
}

void handleTeacherViewHistory(const int &student_id) {
    string request = "VIEW_HISTORY|" + to_string(user_id) + "|" + to_string(student_id) + "|<END>";
    string response = sendRequestToServer(request);
    string status = response.substr(0, response.find('|'));
    if (status == "0") {
        map<string, map<string, vector<pair<Meeting, vector<User>>>>> meetings =
            teacherController.getMeetingsInWeeksFromResponse(response);
        Meeting meeting = teacherView.showHistory(meetings);
        if (meeting.getId() == -1) {
            return;
        }
        // Detail Meeting
        handleTeacherViewHistoryMeeting(meeting.getId());
        handleTeacherViewHistory(student_id);
    } else if (status == "18") {
        vector<string> tokens = splitString(response, '|');
        cout << tokens[1] << endl;
    }
}

void handleTeacherSeeStudentList() {
    string request = "FETCH_ALL_STUDENT|" + to_string(user_id) + "|<END>";
    string response = sendRequestToServer(request);
    string status = response.substr(0, response.find("|"));
    if (status == "0") {
        vector<User> students = teacherController.seeStudentList(response);
        int studentId = teacherView.showStudentList(students);
        if (studentId == -1) {
            return;
        }
        handleTeacherViewHistory(studentId);
        handleTeacherSeeStudentList();
    } else if (status == "21") {
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
    case 2:
        handleViewTimeslots();
        handleTeacherMenu();
        break;
    case 3:
        handleTeacherViewMeetingsByDate();
        handleTeacherMenu();
        break;
    case 4:
        handleTeacherSeeStudentList();
        handleTeacherMenu();
        break;
    default:
        break;
    }
}

// Student Menu Handlers
void handleBookMeeting(const Meeting &meeting) {
    string request = "BOOK_MEETING|" + to_string(user_id) + "|" + to_string(meeting.getTimeslotId()) + "|" +
                     meeting.getType() + "|<END>";
    string response = sendRequestToServer(request);
    string status = response.substr(0, response.find('|'));
    if (status == "0") {
        vector<string> tokens = splitString(response, '|');
        cout << tokens[1] << endl;
    } else if (status == "9") {
        vector<string> tokens = splitString(response, '|');
        cout << tokens[1] << endl;
    } else if (status == "7") {
        vector<string> tokens = splitString(response, '|');
        cout << tokens[1] << endl;
    } else if (status == "16") {
        vector<string> tokens = splitString(response, '|');
        cout << tokens[1] << endl;
    } else if (status == "11") {
        vector<string> tokens = splitString(response, '|');
        cout << tokens[1] << endl;
    } else if (status == "17") {
        vector<string> tokens = splitString(response, '|');
        cout << tokens[1] << endl;
    } else if (status == "12") {
        vector<string> tokens = splitString(response, '|');
        cout << tokens[1] << endl;
    } 
}

void handleCancelMeeting(const int &meeting_id) {
    string request = "CANCEL_APPOINTMENT|" + to_string(user_id) + "|" + to_string(meeting_id) + "|<END>";
    string response = sendRequestToServer(request);
    string status = response.substr(0, response.find('|'));
    vector<string> tokens = splitString(response, '|');
    cout << tokens[1] << endl;
}

void handleViewTimeslotsOfTeacher(const int &teacher_id, const string &teacherName) {
    string request = "VIEW_FREE_TIME_SLOTS|" + to_string(teacher_id) + "|<END>";
    string response = sendRequestToServer(request);
    string status = response.substr(0, response.find('|'));
    if (status == "0") {
        map<string, vector<Timeslot>> timeslots = studentController.viewTimeslots(response);
        Timeslot ts = studentView.showTimeslots(timeslots, teacherName);
        if (ts.getId() == -1) {
            return;
        }
        // Detail Timeslot
        Meeting meeting = studentView.showBookMeeting(ts, teacherName);
        if (meeting.getId() == -1) {
            return;
        } else {
            handleBookMeeting(meeting);
            handleViewTimeslotsOfTeacher(teacher_id, teacherName);
        }

    } else if (status == "15") {
        vector<string> tokens = splitString(response, '|');
        cout << tokens[1] << endl;
    } else if (status == "4") {
        vector<string> tokens = splitString(response, '|');
        cout << tokens[1] << endl;
    }
}
void handleViewAllTeacher() {
    string request = "FETCH_ALL_TEACHER|<END>";
    string response = sendRequestToServer(request);
    string status = response.substr(0, response.find("|"));
    if (status == "0") {
        vector<User> teachers = studentController.getAllTeacher(response);
        pair<string, int> teacher = studentView.showAllTeacher(teachers);
        if (teacher.second == -1) {
            return;
        }
        handleViewTimeslotsOfTeacher(teacher.second, teacher.first);
        handleViewAllTeacher();
    } else if (status == "15") {
        vector<string> tokens = splitString(response, '|');
        cout << tokens[1] << endl;
    }
}

void handleStudentViewMeeting(const int &meeting_id) {
    string request = "VIEW_MEETING_STUDENT|" + to_string(meeting_id) + "|<END>";
    string response = sendRequestToServer(request);
    string status = response.substr(0, response.find('|'));
    if (status == "0") {
        pair<Meeting, User> meetingDetail = studentController.getMeetingFromResponse(response);
        int choice = studentView.showMeeting(meetingDetail.first, meetingDetail.second);
        if (choice == 0) {
            return;
        } else if (choice == 1) {
            handleCancelMeeting(meeting_id);
        }
    } else if (status == "5") {
        // Meeting không tồn tại
        vector<string> tokens = splitString(response, '|');
        cout << tokens[1] << endl;
        // Không làm gì thêm, sẽ quay về handleStudentViewMeetings và yêu cầu chọn lại
    }
}

void handleStudentViewMeetings() {
    string request = "VIEW_MEETINGS_STUDENT|" + to_string(user_id) + "|<END>";
    string response = sendRequestToServer(request);
    string status = response.substr(0, response.find("|"));
    if (status == "0") {
        map<string, map<string, vector<pair<Meeting, User>>>> meetings =
            studentController.getMeetingsInWeeksFromResponse(response);
        int meeting_id = studentView.showMeetingsInWeeks(meetings);
        if (meeting_id == -1) {
            return;  // User chọn 0 để quay lại
        }
        handleStudentViewMeeting(meeting_id);
        handleStudentViewMeetings();
    } else if (status == "8") {
        vector<string> tokens = splitString(response, '|');
        cout << tokens[1] << endl;
    }
}

void handleStudentMenu() {
    int choice = studentView.showMenu();
    switch (choice) {
    case 0:
        logout();
        break;
    case 1:
        handleViewAllTeacher();
        handleStudentMenu();
        break;
    case 2:
        handleStudentViewMeetings();
        handleStudentMenu();
        break;

    default:
        break;
    }
}

void handleLogin() {
    map<string, string> info = userView.showLogin();
    string loginCommand = "LOGIN|" + info["username"] + "|" + info["password"] + "|<END>";
    string response = sendRequestToServer(loginCommand);
    // Phân tích phản hồi từ server
    vector<string> result = splitString(response, '|');
    if (result.size() >= 4 && result[0] == "0" && !result[2].empty()) {
        user_id = safeStoi(result[2]);
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

    // Gửi lệnh đăng ký tới server
    string response = sendRequestToServer(registerCommand);

    // Phân tích phản hồi từ server
    vector<string> result = splitString(response, '|');
    if (result.size() >= 4 && result[0] == "0" && !result[2].empty()) {
        vector<string> tokens = splitString(response, '|');
        cout << tokens[1] << endl;
        user_id = safeStoi(tokens[2]);
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

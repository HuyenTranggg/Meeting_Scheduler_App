#include "../controllers/ClientController.h"
#include "../controllers/ResponseController.h"
#include "../controllers/TeacherController.h"
#include "../controllers/StudentController.h"
#include "../controllers/UserController.h"

#include "../utils/MessageUtils.h"
#include "../views/StudentView.h"
#include "../views/TeacherView.h"
#include "../views/UserView.h"
#include <QApplication>
#include <QWidget>
#include <QInputDialog>
#include <QDate>
#include "../views/teacherviews/timeslotdialog.h"
#include "../views/teacherviews/viewtimeslotsdialog.h"
#include "../views/teacherviews/update_timeslot_dialog.h"
#include "../views/teacherviews/teacherviewmeetingsdialog.h"
#include "../views/teacherviews/meetingdetaildialog.h"
#include "../views/teacherviews/meetinghistorydialog.h"
#include "../views/teacherviews/TeacherMenuWidget.h"
#include "../views/teacherviews/viewmeetinghistorydialog.h"
#include "../views/teacherviews/viewweeklymeetingsdialog.h"
#include "../views/userviews/loginwidget.h"
#include "../views/userviews/mainmenu.h"
#include "../views/userviews/registerwidget.h"
#include "../views/studentviews/ViewAllTeacherWidget.h"
#include "../views/studentviews/teacherlistviewer.h"
#include "../views/studentviews/timeslotcalendar.h"
#include "../views/studentviews/meetingbooking.h"
#include "../views/studentviews/meetingcalendarviewer.h"
#include "../views/studentviews/MeetingDialog.h"
#include "../views/studentviews/StudentMenu.h"
#include <QMessageBox>
#include <QDialog>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>

#include <algorithm>
#include <arpa/inet.h>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <utility>
#include <vector>
#include <QScreen>
#include <QGuiApplication>

// QUAN TRỌNG: Thay bằng địa chỉ IP thực của máy server
// Để lấy IP server: chạy "hostname -I" hoặc "ipconfig" trên máy server
#define SERVER_IP "172.11.10.55"  // <-- THAY ĐỔI ĐỊA CHỈ NÀY
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

using namespace std;

int clientSocket;
struct sockaddr_in serverAddr;
TeacherView teacherView;
StudentView studentView;
UserView userView;
UserController userController;
ClientController clientController;
ResponseController responseController;
TeacherController teacherController;
StudentController studentController;
int user_id = 0;
string role = "none";

// Forward declarations
class TeacherMenuWidget;
extern TeacherMenuWidget* g_teacherMenuWidget;

// Helper function to center widget on screen
void centerWidget(QWidget* widget) {
    if (!widget) return;
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - widget->width()) / 2;
    int y = (screenGeometry.height() - widget->height()) / 2;
    widget->move(x, y);
}

// Helper function to show styled message box
void showStyledMessageBox(const QString& title, const QString& text, QMessageBox::Icon icon) {
    QMessageBox msgBox;
    msgBox.setWindowTitle(title);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setIcon(icon);
    
    // Set text directly without HTML wrapper for proper centering
    msgBox.setText(text);
    msgBox.setTextFormat(Qt::PlainText);
    
    QString buttonColor;
    QString buttonHoverColor;
    QString buttonPressedColor;
    switch(icon) {
        case QMessageBox::Information:
            buttonColor = "#1976d2";
            buttonHoverColor = "#2196f3";
            buttonPressedColor = "#1565c0";
            break;
        case QMessageBox::Warning:
            buttonColor = "#f57c00";
            buttonHoverColor = "#ff9800";
            buttonPressedColor = "#e65100";
            break;
        case QMessageBox::Critical:
            buttonColor = "#e53935";
            buttonHoverColor = "#f44336";
            buttonPressedColor = "#c62828";
            break;
        default:
            buttonColor = "#1976d2";
            buttonHoverColor = "#2196f3";
            buttonPressedColor = "#1565c0";
    }
    
    msgBox.setStyleSheet(
        "QMessageBox { "
        "    background-color: #f5f5f5; "
        "    font-family: 'Segoe UI', Arial; "
        "    min-width: 450px; "
        "} "
        "QLabel { "
        "    font-size: 15pt; "
        "    color: #333333; "
        "    padding: 25px 30px; "
        "    qproperty-alignment: AlignCenter; "
        "    min-height: 70px; "
        "} "
        "QPushButton { "
        "    font-size: 14pt; "
        "    padding: 12px 40px; "
        "    border-radius: 6px; "
        "    min-width: 110px; "
        "    min-height: 45px; "
        "    background-color: " + buttonColor + "; "
        "    color: white; "
        "    border: none; "
        "    margin: 10px; "
        "} "
        "QPushButton:hover { "
        "    background-color: " + buttonHoverColor + "; "
        "} "
        "QPushButton:pressed { "
        "    background-color: " + buttonPressedColor + "; "
        "} "
        "QDialogButtonBox { "
        "    qproperty-centerButtons: true; "
        "}"
    );
    
    // Center the message box on screen
    centerWidget(&msgBox);
    msgBox.exec();
}

void connectToServer() {
    // Tạo socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        std::cerr << "Lỗi tạo socket" << std::endl;
        exit(1);
    }

    // Cấu hình địa chỉ server
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    // Kết nối tới server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Lỗi kết nối tới server" << std::endl;
        exit(1);
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
            cerr << "Lỗi khi gửi dữ liệu!" << endl;
            break;
        }

        // Cập nhật offset để gửi phần tiếp theo
        offset += bytesSent;
    }

    // Nhận phản hồi từ server với timeout
    char buffer[BUFFER_SIZE];
    string response;
    fd_set readfds;
    struct timeval timeout;
    
    while (true) {
        FD_ZERO(&readfds);
        FD_SET(clientSocket, &readfds);
        
        // Set timeout 5 giây
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        
        int selectResult = select(clientSocket + 1, &readfds, NULL, NULL, &timeout);
        
        if (selectResult < 0) {
            cerr << "Lỗi select!" << endl;
            break;
        } else if (selectResult == 0) {
            // Timeout - không nhận được dữ liệu trong 5 giây
            cerr << "Timeout waiting for response from server" << endl;
            // Nếu đã nhận được một phần response (status code), return nó
            if (!response.empty()) {
                cout << "DEBUG: Returning partial response due to timeout: " << response << endl;
                break;
            }
            return "100"; // Timeout error
        }
        
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
    
    // Đóng cửa sổ hiện tại
    QWidget* currentWidget = QApplication::activeWindow();
    if (currentWidget) {
        currentWidget->close();  // Đóng cửa sổ hiện tại
    }
    
    // Hiển thị lại menu người dùng
    handleUserMenu();
}


// Teacher
void handleDeclareTimeSlot() {
    TimeslotDialog dialog(user_id);  // Bỏ this vì không có parent widget
    if (dialog.exec() == QDialog::Accepted) {
        Timeslot ts = dialog.getTimeslot();
        string request = "DECLARE_TIME_SLOT|" + ts.toStringDeclare() + "|<END>";
        cout << request << endl;
        string response = sendRequestToServer(request);
        string status = response.substr(0, response.find('|'));
        vector<string> tokens = splitString(response, '|');
        
        if (status == "0") {
            // Thành công
            showStyledMessageBox("Thành công", QString::fromStdString(tokens[1]), QMessageBox::Information);
        } else if (status == "14") {
            // Conflict với time slot khác
            showStyledMessageBox("Lỗi - Thời gian bị trùng", QString::fromStdString(tokens[1]), QMessageBox::Warning);
        } else if (status == "6") {
            // Thời gian không hợp lệ
            showStyledMessageBox("Lỗi - Thời gian không hợp lệ", QString::fromStdString(tokens[1]), QMessageBox::Warning);
        } else if (status == "13") {
            // Format request không hợp lệ
            showStyledMessageBox("Lỗi", QString::fromStdString(tokens[1]), QMessageBox::Warning);
        } else {
            // Các lỗi khác
            QString errorMsg = tokens.size() > 1 ? QString::fromStdString(tokens[1]) : "Có lỗi xảy ra";
            showStyledMessageBox("Lỗi", errorMsg, QMessageBox::Critical);
        }
    }
}

void handleUpdateTimeslot(const Timeslot &timeslot) {
    // Hiển thị hộp thoại cập nhật thời gian
    UpdateTimeslotDialog dialog(timeslot);
    if (dialog.exec() == QDialog::Accepted) {
        // Lấy thông tin đã cập nhật từ hộp thoại
        Timeslot tsUpdate = dialog.getUpdatedTimeslot();
        
        // Tạo chuỗi yêu cầu gửi đến server
        string request = "EDIT_SLOT|" + to_string(user_id) + "|" + tsUpdate.toStringUpdate() + "|<END>";
        
        // Gửi yêu cầu đến server và nhận phản hồi
        string response = sendRequestToServer(request);
        
        // Kiểm tra trạng thái phản hồi
        string status = response.substr(0, response.find('|'));
        vector<string> tokens = splitString(response, '|');
        
        if (status == "0") {
            // Thành công
            QString message = tokens.size() > 1 ? QString::fromStdString(tokens[1]) : "Cập nhật thời gian rảnh thành công";
            showStyledMessageBox("Thành công", message, QMessageBox::Information);
        } else {
            // Lỗi
            QString errorMsg = tokens.size() > 1 ? QString::fromStdString(tokens[1]) : "Không thể cập nhật thời gian rảnh";
            showStyledMessageBox("Lỗi", errorMsg, QMessageBox::Warning);
        }
    }
}


void handleViewTimeslots() {
    try {
        string request = "VIEW_FREE_TIME_SLOTS|" + to_string(user_id) + "|<END>";
        string response = sendRequestToServer(request);
        
        string status = response.substr(0, response.find('|'));
        
        if (status == "0") {
            map<string, vector<Timeslot>> timeslots = clientController.viewTimeslots(response);
            
            ViewTimeslotsDialog dialog(timeslots);  // Khởi tạo đối tượng dialog với timeslots
            if (dialog.exec() == QDialog::Accepted) {
                Timeslot ts = dialog.getSelectedTimeslot();
                if (ts.getId() == -1) {
                    return;
                }

                int choice = dialog.showTimeslot(ts);  // Gọi showTimeslot từ dialog
                if (choice == 0) {
                    handleViewTimeslots();  // Nếu không muốn chỉnh sửa, quay lại xem
                } else {
                    handleUpdateTimeslot(ts);  // Nếu muốn chỉnh sửa, xử lý cập nhật
                    handleViewTimeslots();  // Xem lại danh sách sau khi cập nhật
                }
            }

        } else if (status == "8") {
            vector<string> tokens = splitString(response, '|');
            QString message = tokens.size() > 1 ? QString::fromStdString(tokens[1]) : "Có lỗi xảy ra";
            showStyledMessageBox("Thông báo", message, QMessageBox::Information);
        } else if (status == "9") {
            vector<string> tokens = splitString(response, '|');
            QString message = tokens.size() > 1 ? QString::fromStdString(tokens[1]) : "Có lỗi xảy ra";
            showStyledMessageBox("Lỗi", message, QMessageBox::Warning);
        }
    } catch (const std::exception& e) {
        cout << "ERROR in handleViewTimeslots: " << e.what() << endl;
        showStyledMessageBox("Lỗi", QString("Exception in handleViewTimeslots: ") + e.what(), QMessageBox::Critical);
    }
}

void handleEditReport(const int &meeting_id, const string &report) {
    try {
        string request = "ENTER_MEETING_NOTE|" + to_string(meeting_id) + "|" + report + "|<END>";
        string response = sendRequestToServer(request);
        
        if (response.empty() || response == "100") {
            showStyledMessageBox("Lỗi", "Không nhận được phản hồi từ server", QMessageBox::Warning);
            return;
        }
        
        string status = response.substr(0, response.find('|'));
        
        if (status == "0") {
            showStyledMessageBox("Thành công", "Đã cập nhật biên bản cuộc họp", QMessageBox::Information);
        } else {
            vector<string> tokens = splitString(response, '|');
            QString errorMsg = tokens.size() > 1 ? QString::fromStdString(tokens[1]) : "Có lỗi xảy ra";
            showStyledMessageBox("Lỗi", errorMsg, QMessageBox::Warning);
        }
    } catch (const std::exception& e) {
        cout << "ERROR in handleEditReport: " << e.what() << endl;
        showStyledMessageBox("Lỗi", QString("Exception: ") + e.what(), QMessageBox::Critical);
    }
}

void handleUpdateStatus(const int &meeting_id, const string &mstatus) {
    string request = "UPDATE_MEETING_STATUS|" + to_string(meeting_id) + "|" + mstatus + "|<END>";
    string response = sendRequestToServer(request);
    string status = response.substr(0, response.find('|'));
    vector<string> tokens = splitString(response, '|');
    
    QString message = tokens.size() > 1 ? QString::fromStdString(tokens[1]) : "Cập nhật trạng thái cuộc hẹn";
    if (status == "0") {
        showStyledMessageBox("Thành công", message, QMessageBox::Information);
    } else {
        showStyledMessageBox("Lỗi", message, QMessageBox::Warning);
    }
}

void handleTeacherViewMeeting(const int &meeting_id) {
    string request = "VIEW_MEETING|" + to_string(meeting_id) + "|<END>";
    string response = sendRequestToServer(request);
    string status = response.substr(0, response.find('|'));
    if (status == "0") {
        pair<Meeting, vector<User>> meetingDetail = teacherController.getMeetingFromResponse(response);
        
        MeetingDetailDialog dialog(meetingDetail.first, meetingDetail.second);
        if (dialog.exec() == QDialog::Accepted) {
            int choice = dialog.getResult();
            if (choice == 0) {
                return;
            } else if (choice == 1) {
                string report = dialog.getReport().toStdString();
                handleEditReport(meeting_id, report);
                handleTeacherViewMeeting(meeting_id);
            } else if (choice == 2) {
                string newStatus = dialog.getStatus().toStdString();
                handleUpdateStatus(meeting_id, newStatus);
                handleTeacherViewMeeting(meeting_id);
            }
        }
    }
}
void handleTeacherViewMeetings() {
    try {
        
        // Yêu cầu người dùng nhập ngày cần xem
        bool ok;
        QInputDialog inputDialog;
        inputDialog.setWindowTitle("Xem lịch hẹn");
        inputDialog.setLabelText("Nhập ngày cần xem (YYYY-MM-DD):");
        inputDialog.setTextValue(QDate::currentDate().toString("yyyy-MM-dd"));
        inputDialog.setStyleSheet(
            "QInputDialog { "
            "    background-color: #f5f5f5; "
            "    font-family: 'Segoe UI', Arial; "
            "    min-width: 500px; "
            "} "
            "QLabel { "
            "    font-size: 15pt; "
            "    color: #333333; "
            "    padding: 15px; "
            "} "
            "QLineEdit { "
            "    font-size: 15pt; "
            "    padding: 12px; "
            "    border: 2px solid #f57c00; "
            "    border-radius: 8px; "
            "    background-color: white; "
            "    min-height: 40px; "
            "} "
            "QPushButton { "
            "    font-size: 14pt; "
            "    padding: 12px 28px; "
            "    border-radius: 8px; "
            "    min-width: 100px; "
            "    min-height: 45px; "
            "    background-color: #f57c00; "
            "    color: white; "
            "    border: none; "
            "    margin: 5px; "
            "} "
            "QPushButton:hover { "
            "    background-color: #ef6c00; "
            "}"
        );
        inputDialog.resize(550, 200);
        centerWidget(&inputDialog);
        ok = inputDialog.exec();
        QString dateStr = inputDialog.textValue();
        
        if (!ok || dateStr.isEmpty()) {
            return; // Người dùng hủy
        }
        
        Meeting selectedMeeting; 
        // Tạo yêu cầu với date
        string request = "VIEW_MEETINGS|" + to_string(user_id) + "|" + dateStr.toStdString() + "|<END>";
        
        // Gửi yêu cầu và nhận phản hồi
        string response = sendRequestToServer(request);
        
        // Kiểm tra trạng thái phản hồi
        string status = response.substr(0, response.find('|'));

        // Nếu trạng thái phản hồi là "0", xử lý cuộc họp
        if (status == "0") {
            // Parse response để lấy meetings
            vector<string> tokens = splitString(response, '|');
            
            if (tokens.size() <= 2 || tokens[1] == "No meetings found") {
                showStyledMessageBox("Thông báo", "Không có lịch hẹn nào trong ngày này", QMessageBox::Information);
                return;
            }
            
            // Tạo map meetings từ response (format: 0|meeting_id|start|end|type|student_ids|...)
            QMap<QString, QVector<Meeting>> qtMeetings;
            QVector<Meeting> meetingList;
            
            for (size_t i = 1; i + 4 < tokens.size(); i += 5) {
                try {
                    int meeting_id = stoi(tokens[i]);
                    string start = tokens[i + 1];
                    string end = tokens[i + 2];
                    string type = tokens[i + 3];
                    string date = dateStr.toStdString();
                    // tokens[i + 4] là student_ids, bỏ qua vì chưa cần
                    
                    // Sử dụng constructor: Meeting(teacher_id, status, type, start, end, date)
                    Meeting meeting(user_id, "pending", type, start, end, date);
                    meeting.setId(meeting_id);
                    meetingList.append(meeting);
                } catch (const std::exception& e) {
                    cout << "ERROR parsing meeting: " << e.what() << endl;
                    continue;
                }
            }
            
            if (!meetingList.isEmpty()) {
                qtMeetings[dateStr] = meetingList;
                
                // Hiển thị giao diện TeacherViewMeetingsDialog
                TeacherViewMeetingsDialog dialog(qtMeetings);
                if (dialog.exec() == QDialog::Accepted) {
                    // Nhận cuộc họp đã chọn
                    selectedMeeting = dialog.getSelectedMeeting();
                    
                    if (selectedMeeting.getId() == -1) {
                        return; // Nếu không có cuộc họp nào được chọn
                    }

                    // Xử lý cuộc họp chi tiết
                    handleTeacherViewMeeting(selectedMeeting.getId());
                    
                    // Tiếp tục xử lý các cuộc họp khác
                    handleTeacherViewMeetings();
                }
            } else {
                showStyledMessageBox("Thông báo", "Không có lịch hẹn nào trong ngày này", QMessageBox::Information);
            }
        } 
        // Nếu trạng thái là "16", hiển thị thông báo lỗi
        else if (status == "16") {
            vector<string> tokens = splitString(response, '|');
            QString errorMessage = QString::fromStdString(tokens[1]);
            QMessageBox::critical(nullptr, "Lỗi", errorMessage);
        }
        // Xử lý các error codes khác
        else {
            vector<string> tokens = splitString(response, '|');
            QString errorMessage;
            if (tokens.size() > 1 && !tokens[1].empty()) {
                errorMessage = QString::fromStdString(tokens[1]);
            } else {
                errorMessage = "Không có dữ liệu lịch hẹn hoặc có lỗi xảy ra (Mã lỗi: " + QString::fromStdString(status) + ")";
            }
            showStyledMessageBox("Thông báo", errorMessage, QMessageBox::Information);
        }
    } catch (const std::exception& e) {
        cout << "ERROR in handleTeacherViewMeetings: " << e.what() << endl;
        QMessageBox::critical(nullptr, "Lỗi", QString("Exception: ") + e.what());
    }
}

void handleTeacherViewHistoryMeeting(const int &meeting_id) {
    // Tạo yêu cầu để lấy thông tin lịch hẹn từ server
    string request = "VIEW_MEETING|" + to_string(meeting_id) + "|<END>";
    string response = sendRequestToServer(request);

    // Phân tích phản hồi từ server
    string status = response.substr(0, response.find('|'));
    if (status == "0") {
        // Lấy thông tin chi tiết lịch hẹn và danh sách người tham gia
        pair<Meeting, vector<User>> meetingDetail = teacherController.getMeetingFromResponse(response);

        // Tạo dialog và hiển thị thông tin lịch hẹn
        MeetingHistoryDialog dialog(meetingDetail.first, meetingDetail.second);
        if (dialog.exec() == QDialog::Accepted) {
            // Nếu nhấn "OK" trong dialog, kiểm tra xem văn bản báo cáo có thay đổi không
            QString newReport = dialog.getReport();
            if (newReport.toStdString() != meetingDetail.first.getReport()) {
                // Gửi báo cáo đã chỉnh sửa lên server
                handleEditReport(meeting_id, newReport.toStdString());

                // Hiển thị lại giao diện với thông tin đã cập nhật
                handleTeacherViewHistoryMeeting(meeting_id);
            }
        }
    } else {
        // Xử lý trường hợp lỗi
        vector<string> tokens = splitString(response, '|');
        QString errorMsg = tokens.size() > 1 ? QString::fromStdString(tokens[1]) : "Không thể lấy thông tin cuộc hẹn";
        showStyledMessageBox("Lỗi", errorMsg, QMessageBox::Warning);
    }
}

// Modified handleTeacherViewHistory function
void handleTeacherViewHistory() {
    try {
        // Bước 1: Lấy danh sách sinh viên đã từng hẹn
        string fetchRequest = "FETCH_ALL_STUDENT|" + to_string(user_id) + "|<END>";
        string fetchResponse = sendRequestToServer(fetchRequest);
        
        string fetchStatus = fetchResponse.substr(0, fetchResponse.find('|'));
        
        if (fetchStatus != "0") {
            vector<string> tokens = splitString(fetchResponse, '|');
            QString errorMsg = tokens.size() > 1 ? QString::fromStdString(tokens[1]) : "Không có sinh viên nào";
            showStyledMessageBox("Thông báo", errorMsg, QMessageBox::Information);
            return;
        }
        
        // Parse danh sách sinh viên
        vector<User> students = teacherController.seeStudentList(fetchResponse);
        if (students.empty()) {
            showStyledMessageBox("Thông báo", "Không có sinh viên nào đã hẹn với bạn", QMessageBox::Information);
            return;
        }
        
        // Bước 2: Hiển thị dialog để chọn sinh viên
        QStringList studentNames;
        for (const auto& student : students) {
            studentNames << QString::fromStdString(student.getFirstName() + " " + student.getLastName() + " (ID: " + to_string(student.getId()) + ")");
        }
        
        QInputDialog inputDialog;
        inputDialog.setWindowTitle("Chọn sinh viên");
        inputDialog.setLabelText("Chọn sinh viên để xem lịch sử:");
        inputDialog.setComboBoxItems(studentNames);
        inputDialog.setComboBoxEditable(false);
        inputDialog.setStyleSheet(
            "QInputDialog { "
            "    background-color: #f5f5f5; "
            "    font-family: 'Segoe UI', Arial; "
            "    min-width: 550px; "
            "} "
            "QLabel { "
            "    font-size: 15pt; "
            "    color: #333333; "
            "    padding: 15px; "
            "} "
            "QComboBox { "
            "    font-size: 14pt; "
            "    padding: 10px; "
            "    border: 2px solid #f57c00; "
            "    border-radius: 8px; "
            "    background-color: white; "
            "    min-height: 40px; "
            "} "
            "QPushButton { "
            "    font-size: 14pt; "
            "    padding: 12px 28px; "
            "    border-radius: 8px; "
            "    min-width: 100px; "
            "    min-height: 45px; "
            "    background-color: #f57c00; "
            "    color: white; "
            "    border: none; "
            "    margin: 5px; "
            "} "
            "QPushButton:hover { "
            "    background-color: #ef6c00; "
            "}"
        );
        inputDialog.resize(600, 220);
        centerWidget(&inputDialog);
        bool ok = inputDialog.exec();
        QString selectedStudent = inputDialog.textValue();
        if (!ok || selectedStudent.isEmpty()) {
            return; // User hủy
        }
        
        // Lấy student_id từ lựa chọn
        int selectedIndex = studentNames.indexOf(selectedStudent);
        int student_id = students[selectedIndex].getId();
        
        // Bước 3: Gọi VIEW_HISTORY với teacher_id và student_id
        string request = "VIEW_HISTORY|" + to_string(user_id) + "|" + to_string(student_id) + "|<END>";
        string response = sendRequestToServer(request);
        
        string status = response.substr(0, response.find('|'));
        
        if (status == "0") {
            // Parse meetings history (format phức tạp: week -> day -> meetings)
            map<string, map<string, vector<pair<Meeting, vector<User>>>>> meetingsWeekly =
                teacherController.getMeetingsInWeeksFromResponse(response);
            
            // Flatten structure: chuyển từ nested map thành vector<Meeting> với thông tin ngày
            vector<pair<string, Meeting>> allMeetings; // pair<date, meeting>
            for (const auto& weekPair : meetingsWeekly) {
                for (const auto& dayPair : weekPair.second) {
                    string date = dayPair.first;
                    for (const auto& meetingPair : dayPair.second) {
                        allMeetings.push_back(make_pair(date, meetingPair.first));
                    }
                }
            }
            
            if (allMeetings.empty()) {
                showStyledMessageBox("Thông báo", "Không có lịch sử cuộc họp", QMessageBox::Information);
                return;
            }
            
            // Tạo dialog hiển thị danh sách meetings
            QDialog* listDialog = new QDialog();
            listDialog->setWindowTitle("Danh sách lịch hẹn");
            listDialog->setFixedSize(937, 750);
            listDialog->setStyleSheet(
                "QDialog { background-color: #f5f5f5; font-family: 'Segoe UI', Arial; }"
                "QLabel { color: #333333; font-size: 17pt; padding: 8px; }"
                "QListWidget { background-color: white; border: 2px solid #f57c00; border-radius: 8px; font-size: 14pt; padding: 10px; }"
                "QListWidget::item { padding: 12px; margin: 4px; border-bottom: 1px solid #e0e0e0; }"
                "QListWidget::item:selected { background-color: #f57c00; color: white; border-radius: 6px; }"
                "QPushButton { font-size: 14pt; padding: 12px 25px; border-radius: 8px; min-height: 45px; }"
            );
            centerWidget(listDialog);
            
            QVBoxLayout* layout = new QVBoxLayout(listDialog);
            QLabel* titleLabel = new QLabel("Chọn một cuộc hẹn để xem chi tiết:");
            layout->addWidget(titleLabel);
            
            QListWidget* meetingList = new QListWidget();
            layout->addWidget(meetingList);
            
            // Thêm các meetings vào list
            map<int, int> indexToMeetingId; // map từ index trong list đến meeting ID
            int index = 0;
            for (const auto& dateMeetingPair : allMeetings) {
                string date = dateMeetingPair.first;
                Meeting meeting = dateMeetingPair.second;
                
                QString itemText = QString("Ngày: %1 | Từ: %2 - Đến: %3 | %4 | %5")
                    .arg(QString::fromStdString(date))
                    .arg(QString::fromStdString(meeting.getStart()))
                    .arg(QString::fromStdString(meeting.getEnd()))
                    .arg(QString::fromStdString(meeting.getType()))
                    .arg(QString::fromStdString(meeting.getStatus()));
                
                meetingList->addItem(itemText);
                indexToMeetingId[index] = meeting.getId();
                index++;
            }
            
            // Thêm nút OK và Cancel
            QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
            layout->addWidget(buttonBox);
            
            QObject::connect(buttonBox, &QDialogButtonBox::accepted, listDialog, &QDialog::accept);
            QObject::connect(buttonBox, &QDialogButtonBox::rejected, listDialog, &QDialog::reject);
            
            // Xử lý double click
            int selectedMeetingId = -1;
            QObject::connect(meetingList, &QListWidget::itemDoubleClicked, [&](QListWidgetItem* item) {
                int row = meetingList->row(item);
                if (indexToMeetingId.find(row) != indexToMeetingId.end()) {
                    selectedMeetingId = indexToMeetingId[row];
                    listDialog->accept();
                }
            });
            
            // Hiển thị dialog
            if (listDialog->exec() == QDialog::Accepted) {
                // Nếu chưa chọn bằng double click, lấy item hiện tại
                if (selectedMeetingId == -1) {
                    QListWidgetItem* currentItem = meetingList->currentItem();
                    if (currentItem) {
                        int row = meetingList->row(currentItem);
                        if (indexToMeetingId.find(row) != indexToMeetingId.end()) {
                            selectedMeetingId = indexToMeetingId[row];
                        }
                    }
                }
                
                if (selectedMeetingId != -1) {
                    handleTeacherViewHistoryMeeting(selectedMeetingId);
                    handleTeacherViewHistory();
                }
            }
            
            delete listDialog;
        } else if (status == "18") {
            vector<string> tokens = splitString(response, '|');
            showStyledMessageBox("Thông báo", QString::fromStdString(tokens[1]), QMessageBox::Information);
        } else {
            vector<string> tokens = splitString(response, '|');
            QString errorMsg = tokens.size() > 1 ? QString::fromStdString(tokens[1]) : "Có lỗi xảy ra";
            showStyledMessageBox("Lỗi", errorMsg, QMessageBox::Warning);
        }
    } catch (const std::exception& e) {
        cout << "ERROR in handleTeacherViewHistory: " << e.what() << endl;
        showStyledMessageBox("Lỗi", QString("Exception: ") + e.what(), QMessageBox::Critical);
    }
}

void handleTracherViewMeetingsInWeeks() {
    // TODO: Chức năng này chưa được server hỗ trợ (VIEW_MEETINGS_IN_WEEKS protocol không tồn tại)
    showStyledMessageBox("Thông báo", 
        "Chức năng xem lịch hẹn theo tuần đang được phát triển.", QMessageBox::Information);
    return;
    
    /* DISABLED - Server chưa hỗ trợ
    string request = "VIEW_MEETINGS_IN_WEEKS|" + to_string(user_id) + "|<END>";
    string response = sendRequestToServer(request);
    string status = response.substr(0, response.find('|'));
    
    if (status == "0") {
        map<string, map<string, vector<pair<Meeting, vector<User>>>>> meetings =
            teacherController.getMeetingsInWeeksFromResponse(response);
            
        ViewWeeklyMeetingsDialog dialog(meetings);
        if (dialog.exec() == QDialog::Accepted) {
            Meeting meeting = dialog.getSelectedMeeting();
            if (meeting.getId() == -1) {
                return;
            }
            // Detail Meeting
            handleTeacherViewMeeting(meeting.getId());
            handleTracherViewMeetingsInWeeks();
        }
    } else if (status == "16") {
        vector<string> tokens = splitString(response, '|');
        showStyledMessageBox("Lỗi", QString::fromStdString(tokens[1]), QMessageBox::Warning);
    }
    */
}


// Global pointer để lưu teacher menu
TeacherMenuWidget* g_teacherMenuWidget = nullptr;

void resetTeacherMenu() {
    if (g_teacherMenuWidget != nullptr) {
        g_teacherMenuWidget->close();
        delete g_teacherMenuWidget;
        g_teacherMenuWidget = nullptr;
    }
}

void handleTeacherMenu() {
    // Nếu menu đã tồn tại, chỉ cần hiện lại
    if (g_teacherMenuWidget != nullptr) {
        g_teacherMenuWidget->show();
        g_teacherMenuWidget->raise();
        g_teacherMenuWidget->activateWindow();
        return;
    }
    
    // Tạo menu lần đầu tiên
    try {
        g_teacherMenuWidget = new TeacherMenuWidget();
    } catch (const std::exception& e) {
        QMessageBox::critical(nullptr, "Lỗi", QString("Cannot create menu: ") + e.what());
        return;
    }

    // Kết nối tín hiệu từ các nút
    QObject::connect(g_teacherMenuWidget, &TeacherMenuWidget::actionSelected, [](int action) {
        switch(action) {
            case 0: // Đăng xuất
                resetTeacherMenu();
                logout();
                break;
            case 1: // Khai báo thời gian rảnh
                if (g_teacherMenuWidget) {
                    g_teacherMenuWidget->hide();
                    handleDeclareTimeSlot();
                    g_teacherMenuWidget->show();
                }
                break;
            case 2: // Kiểm tra thời gian rảnh
                if (g_teacherMenuWidget) {
                    g_teacherMenuWidget->hide();
                    handleViewTimeslots();
                    g_teacherMenuWidget->show();
                }
                break;
            case 3: // Xem lịch hẹn với sinh viên
                if (g_teacherMenuWidget) {
                    g_teacherMenuWidget->hide();
                    handleTeacherViewMeetings();
                    g_teacherMenuWidget->show();
                }
                break;
            case 4: // Xem lịch sử cuộc hẹn
                if (g_teacherMenuWidget) {
                    g_teacherMenuWidget->hide();
                    handleTeacherViewHistory();
                    g_teacherMenuWidget->show();
                }
                break;
            case 5: // Xem lịch hẹn theo tuần
                if (g_teacherMenuWidget) {
                    g_teacherMenuWidget->hide();
                    handleTracherViewMeetingsInWeeks();
                    g_teacherMenuWidget->show();
                }
                break;
            default:
                break;
        }
    });

    g_teacherMenuWidget->show();
}


// Student
void handleBookMeeting(const Meeting &meeting) {
    string request = "BOOK_MEETING|" + to_string(user_id) + "|" + to_string(meeting.getTimeslotId()) + "|" +
                     meeting.getType() + "|<END>";
    string response = sendRequestToServer(request);
    string status = response.substr(0, response.find('|'));
    
    vector<string> tokens = splitString(response, '|');
    QString message = QString::fromStdString(tokens[1]); // Nội dung thông báo
    
    if (status == "0") {
        showStyledMessageBox("Thành công", message, QMessageBox::Information);
    } else if (status == "9") {
        showStyledMessageBox("Lỗi", message, QMessageBox::Warning);
    } else if (status == "7") {
        showStyledMessageBox("Lỗi", message, QMessageBox::Critical);
    } else if (status == "16") {
        showStyledMessageBox("Lỗi", message, QMessageBox::Warning);
    } else if (status == "11") {
        showStyledMessageBox("Lỗi", message, QMessageBox::Warning);
    } else if (status == "17") {
        showStyledMessageBox("Lỗi", message, QMessageBox::Warning);
    } else if (status == "12") {
        showStyledMessageBox("Lỗi", message, QMessageBox::Warning);
    } else if (status == "18") {
        showStyledMessageBox("Lỗi", message, QMessageBox::Warning);
    }
}


void handleViewTimeslotsOfTeacher(const int &teacher_id, const string &teacherName) {
    string request = "VIEW_FREE_TIME_SLOTS|" + to_string(teacher_id) + "|<END>";
    string response = sendRequestToServer(request);
    string status = response.substr(0, response.find('|'));
    
    if (status == "0") {
        map<string, vector<Timeslot>> timeslots = studentController.viewTimeslots(response);
        
        // Flatten structure: chuyển từ map thành vector<Timeslot> với thông tin ngày
        vector<pair<string, Timeslot>> allTimeslots; // pair<date, timeslot>
        for (const auto& datePair : timeslots) {
            string date = datePair.first;
            for (const auto& ts : datePair.second) {
                allTimeslots.push_back(make_pair(date, ts));
            }
        }
        
        if (allTimeslots.empty()) {
            showStyledMessageBox("Thông báo", 
                QString("Giáo viên %1 chưa có thời gian rảnh nào").arg(QString::fromStdString(teacherName)), QMessageBox::Information);
            return;
        }
        
        // Tạo dialog hiển thị danh sách timeslots
        QDialog* listDialog = new QDialog();
        listDialog->setWindowTitle(QString("Thời gian rảnh - %1").arg(QString::fromStdString(teacherName)));
        listDialog->setFixedSize(937, 750);
        listDialog->setStyleSheet(
            "QDialog { background-color: #f5f5f5; font-family: 'Segoe UI', Arial; }"
            "QLabel { color: #333333; font-size: 17pt; padding: 8px; }"
            "QListWidget { background-color: white; border: 2px solid #8e24aa; border-radius: 8px; font-size: 14pt; padding: 10px; }"
            "QListWidget::item { padding: 12px; margin: 4px; border-bottom: 1px solid #e0e0e0; }"
            "QListWidget::item:selected { background-color: #8e24aa; color: white; border-radius: 6px; }"
            "QPushButton { font-size: 14pt; padding: 12px 25px; border-radius: 8px; min-height: 45px; }"
        );
        centerWidget(listDialog);
        
        QVBoxLayout* layout = new QVBoxLayout(listDialog);
        QLabel* titleLabel = new QLabel(QString("Chọn một khung giờ để đặt lịch hẹn với %1:")
            .arg(QString::fromStdString(teacherName)));
        layout->addWidget(titleLabel);
        
        QListWidget* timeslotList = new QListWidget();
        layout->addWidget(timeslotList);
        
        // Thêm các timeslots vào list
        map<int, Timeslot> indexToTimeslot; // map từ index trong list đến Timeslot
        int index = 0;
        for (const auto& dateTimeslotPair : allTimeslots) {
            string date = dateTimeslotPair.first;
            Timeslot ts = dateTimeslotPair.second;
            
            QString itemText = QString("Ngày: %1 | Từ: %2 - Đến: %3 | Loại: %4 | Trạng thái: %5")
                .arg(QString::fromStdString(date))
                .arg(QString::fromStdString(ts.getStart()))
                .arg(QString::fromStdString(ts.getEnd()))
                .arg(QString::fromStdString(ts.getType()))
                .arg(QString::fromStdString(ts.getStatus()));
            
            timeslotList->addItem(itemText);
            indexToTimeslot[index] = ts;
            index++;
        }
        
        // Thêm nút OK và Cancel
        QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        layout->addWidget(buttonBox);
        
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, listDialog, &QDialog::accept);
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, listDialog, &QDialog::reject);
        
        // Xử lý double click
        Timeslot selectedTimeslot;
        selectedTimeslot.setId(-1);
        
        QObject::connect(timeslotList, &QListWidget::itemDoubleClicked, [&](QListWidgetItem* item) {
            int row = timeslotList->row(item);
            if (indexToTimeslot.find(row) != indexToTimeslot.end()) {
                selectedTimeslot = indexToTimeslot[row];
                listDialog->accept();
            }
        });
        
        // Hiển thị dialog
        if (listDialog->exec() == QDialog::Accepted) {
            // Nếu chưa chọn bằng double click, lấy item hiện tại
            if (selectedTimeslot.getId() == -1) {
                QListWidgetItem* currentItem = timeslotList->currentItem();
                if (currentItem) {
                    int row = timeslotList->row(currentItem);
                    if (indexToTimeslot.find(row) != indexToTimeslot.end()) {
                        selectedTimeslot = indexToTimeslot[row];
                    }
                }
            }
            
            if (selectedTimeslot.getId() != -1) {
                // Show meeting booking dialog
                MeetingBooking bookingDialog;
                Meeting meeting = bookingDialog.showBookMeeting(selectedTimeslot, teacherName);
                
                if (meeting.getId() != -1) {
                    handleBookMeeting(meeting);
                    handleViewTimeslotsOfTeacher(teacher_id, teacherName);
                }
            }
        }
        
        delete listDialog;
    } 
    else if (status == "4") {
        // Teacher không có thời gian rảnh
        vector<string> tokens = splitString(response, '|');
        showStyledMessageBox("Thông báo", 
            QString("Giáo viên %1 hiện chưa có thời gian rảnh nào.\n%2")
            .arg(QString::fromStdString(teacherName))
            .arg(QString::fromStdString(tokens[1])), QMessageBox::Information);
    }
    else if (status == "15") {
        // Teacher không tìm thấy
        vector<string> tokens = splitString(response, '|');
        showStyledMessageBox("Lỗi", QString::fromStdString(tokens[1]), QMessageBox::Warning);
    }
    else if (status == "8" || status == "9") {
        // Các lỗi khác
        vector<string> tokens = splitString(response, '|');
        showStyledMessageBox("Lỗi", QString::fromStdString(tokens[1]), QMessageBox::Warning);
    }
    else {
        // Lỗi không xác định
        QMessageBox::critical(nullptr, "Lỗi", 
            QString("Có lỗi xảy ra khi lấy thông tin thời gian rảnh (Mã lỗi: %1)").arg(QString::fromStdString(status)));
    }
}

void handleStudentMenu();

// In client1.cpp
void handleViewAllTeacher() {
    string request = "FETCH_ALL_TEACHER|<END>";
    string response = sendRequestToServer(request);
    string status = response.substr(0, response.find("|"));
    
    if (status == "0") {
        vector<User> teachers = studentController.getAllTeacher(response);
        
        // Create and show the Qt dialog
        TeacherListViewer viewer;
        auto [teacherName, teacherId] = viewer.showTeacherList(teachers);
        
        if (teacherId == -1) {
            return;
        }
        
        handleViewTimeslotsOfTeacher(teacherId, teacherName.toStdString());
        handleViewAllTeacher();
    } 
    else if (status == "8") {
        vector<string> tokens = splitString(response, '|');
        showStyledMessageBox("Lỗi", QString::fromStdString(tokens[1]), QMessageBox::Warning);
    }
}


void handleCancelMeeting(const int &meeting_id) {
    string request = "CANCEL_APPOINTMENT|" + to_string(user_id) + "|" + to_string(meeting_id) + "|<END>";
    string response = sendRequestToServer(request);
    string status = response.substr(0, response.find('|'));
    vector<string> tokens = splitString(response, '|');
    
    // Kiểm tra status code để hiển thị icon phù hợp
    if (status == "0") {
        // Thành công
        showStyledMessageBox("Thành công", QString::fromStdString(tokens[1]), QMessageBox::Information);
    } else if (status == "19") {
        // Không thể hủy do trạng thái
        showStyledMessageBox("Không thể hủy do trạng thái không hợp lệ", QString::fromStdString(tokens[1]), QMessageBox::Warning);
    } else {
        // Các lỗi khác
        showStyledMessageBox("Lỗi", QString::fromStdString(tokens[1]), QMessageBox::Critical);
    }
}

void handleStudentViewMeeting(const int &meeting_id) {
    string request = "VIEW_MEETING_STUDENT|" + to_string(meeting_id) + "|<END>";
    string response = sendRequestToServer(request);
    string status = response.substr(0, response.find('|'));
    
    if (status == "0") {
        pair<Meeting, User> meetingDetail = studentController.getMeetingFromResponse(response);
        MeetingDialog dialog;
        int choice = dialog.showMeeting(meetingDetail.first, meetingDetail.second);
        
        if (choice == 1) {
            handleCancelMeeting(meeting_id);
        }
    }
}

void handleStudentViewMeetings() {
    string request = "VIEW_MEETINGS_STUDENT|" + to_string(user_id) + "|<END>";
    string response = sendRequestToServer(request);
    string status = response.substr(0, response.find("|"));
    
    if (status == "0") {
        map<string, map<string, vector<pair<Meeting, User>>>> meetings =
            studentController.getMeetingsInWeeksFromResponse(response);
            
        // Create and show the calendar viewer
        MeetingCalendarViewer viewer;
        Meeting meeting = viewer.showMeetingsInWeeks(meetings);
        
        if (meeting.getId() == -1) {
            return;
        }
        
        // Detail Meeting
        handleStudentViewMeeting(meeting.getId());
        handleStudentViewMeetings();
    }
    else if (status == "8") {
        // Không có lịch hẹn nào
        vector<string> tokens = splitString(response, '|');
        showStyledMessageBox("Thông báo", 
            "Bạn chưa có lịch hẹn nào trong tuần này.", QMessageBox::Information);
    }
    else if (status == "17") {
        // Student không tìm thấy
        vector<string> tokens = splitString(response, '|');
        showStyledMessageBox("Lỗi", QString::fromStdString(tokens[1]), QMessageBox::Warning);
    }
    else if (status == "16") {
        // Các lỗi khác
        vector<string> tokens = splitString(response, '|');
        showStyledMessageBox("Lỗi", QString::fromStdString(tokens[1]), QMessageBox::Warning);
    }
    else {
        // Lỗi không xác định
        QMessageBox::critical(nullptr, "Lỗi", 
            QString("Có lỗi xảy ra khi lấy lịch hẹn (Mã lỗi: %1)").arg(QString::fromStdString(status)));
    }
}

void handleStudentMenu() {
    StudentMenu menu;
    while (true) {
        int choice = menu.showMenu();
        switch (choice) {
            case 0:
                logout();
                return;
            case 1:
                handleViewAllTeacher();
                break;
            case 2:
                handleStudentViewMeetings();
                break;
        }
    }
}

void handleLogin() {
    LoginWidget* loginWidget = new LoginWidget();
    
    // Kết nối signals
    QObject::connect(loginWidget, &LoginWidget::loginClicked, [loginWidget]() {
        try {
            map<string, string> info = loginWidget->getLoginInfo();
            string loginCommand = "LOGIN|" + info["username"] + "|" + info["password"] + "|<END>";
            string response = sendRequestToServer(loginCommand);
            vector<string> result = splitString(response, '|');
            
            if (result[0] == "0") {
                user_id = stoi(result[2]);
                role = result[3];
                showStyledMessageBox("Thành công", 
                    QString::fromStdString(result[1]), QMessageBox::Information);
                loginWidget->close();
                if (role == "teacher" && user_id != 0) {
                    handleTeacherMenu();
                } 
                else if (role == "student" && user_id != 0) {
                    handleStudentMenu();
                }
            }
            else if (result[0] == "1" || result[0] == "2") {
                showStyledMessageBox("Lỗi", 
                    QString::fromStdString(result[1]), QMessageBox::Warning);
            } 
            else {
                loginWidget->close();
                logout();
            }
        } catch (const std::exception& e) {
            cout << "ERROR in handleLogin: " << e.what() << endl;
            QMessageBox::critical(loginWidget, "Lỗi", QString("Exception: ") + e.what());
        }
    });
    
    QObject::connect(loginWidget, &LoginWidget::backClicked, [loginWidget]() {
        loginWidget->close();
        handleUserMenu();
    });
    
    loginWidget->show();
    
    // Event loop để đợi cửa sổ đóng
    QEventLoop loop;
    QObject::connect(loginWidget, &LoginWidget::destroyed, &loop, &QEventLoop::quit);
    loop.exec();
}

void handleRegister() {
    RegisterWidget* registerWidget = new RegisterWidget();
    
    // Kết nối signal đăng ký
    QObject::connect(registerWidget, &RegisterWidget::registerClicked, [registerWidget]() {
        map<string, string> info = registerWidget->getRegisterInfo();
        
        string registerCommand = "REGISTER|" + info["username"] + "|" + info["password"] + "|" + 
                               info["role"] + "|" + info["first_name"] + "|" + info["last_name"] + "|<END>";
        
        string response = sendRequestToServer(registerCommand);
        vector<string> result = splitString(response, '|');
        
        if (result[0] == "0") {
            user_id = stoi(result[2]);
            role = result[3];
            cout << user_id << role << result[1] << endl;
            showStyledMessageBox("Thành công", 
                QString::fromStdString(result[1]), QMessageBox::Information);
                
            registerWidget->close();
            
            if (role == "teacher" && user_id != 0) {
                handleTeacherMenu();
            } else if (role == "student" && user_id != 0) {
                handleStudentMenu();
            }
        } 
        else if (result[0] == "3") {
            showStyledMessageBox("Lỗi", 
                QString::fromStdString(result[1]), QMessageBox::Warning);
        }
    });
    
    // Kết nối signal quay lại
    QObject::connect(registerWidget, &RegisterWidget::backClicked, [registerWidget]() {
        registerWidget->close();
        handleUserMenu();
    });
    
    registerWidget->show();
    
    // Event loop để đợi cửa sổ đóng
    QEventLoop loop;
    QObject::connect(registerWidget, &RegisterWidget::destroyed, &loop, &QEventLoop::quit);
    loop.exec();
}
void handleUserMenu() {
    MainMenu* menu = new MainMenu();
    
    // Kết nối signals
    QObject::connect(menu, &MainMenu::loginSelected, [menu]() {
        menu->close();  // Đóng cửa sổ
        handleLogin();  // Xử lý đăng nhập
    });

    
    QObject::connect(menu, &MainMenu::registerSelected, [menu]() {
        menu->close();
        handleRegister();
    });
    
    menu->show();
    
    // Event loop để đợi cửa sổ đóng
    QEventLoop loop;
    QObject::connect(menu, &MainMenu::destroyed, &loop, &QEventLoop::quit);
    loop.exec();
    
    // Lấy lựa chọn và xử lý tiếp
    int choice = menu->getChoice();
    if (choice != 0) {  // Nếu đã chọn một option
        handleUserMenu();  // Quay lại menu chính
    }
}

void closeConnection() { close(clientSocket); }

int main(int argc, char *argv[]) {
   QApplication app(argc, argv);  
    
    connectToServer();
    handleUserMenu();
    closeConnection();
    
    return app.exec();
}

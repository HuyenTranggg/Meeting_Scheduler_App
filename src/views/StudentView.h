#ifndef STUDENTVIEW_H
#define STUDENTVIEW_H

#include "../controllers/StudentController.h"
#include "../models/Timeslot.h"
#include <iostream>
#include <vector>
using namespace std;

class StudentView {
  public:
    StudentView() {}

    int showMenu() {
        int choice;
        while (true) {
            cout << "----------------Student Menu----------------" << endl;
            cout << "1. Xem và đặt lịch rảnh của giáo viên" << endl;
            cout << "2. Xem lịch gặp trong tuần" << endl;
            cout << "0. Đăng xuất" << endl;
            cout << "Bạn muốn thực hiện chức năng nào?" << endl;
            cin >> choice;
            cin.ignore();
            switch (choice) {
            case 0:
            case 1:
            case 2:
                return choice;
                break;
            default:
                cout << "Lựa chọn không hợp lệ, vui lòng thử lại!" << endl;
                break;
            }
        }
    }

    pair<string, int> showAllTeacher(const vector<User> &teachers) {
        pair<string, int> teacher;
        if (teachers.empty()) {
            cout << "Không có giáo viên nào để chọn." << endl;
            teacher.second = -1;
            return teacher;
        }

        cout << "------------Danh sách giáo viên------------" << endl;
        for (int i = 0; i < teachers.size(); i++) {
            cout << i + 1 << ". " << teachers[i].getFirstName() << " " << teachers[i].getLastName()
                 << " (ID: " << teachers[i].getId() << ")" << endl;
        }
        cout << "------------------------------------------" << endl;

        int choice;
        while (true) {
            cout << "Nhập số thứ tự của giáo viên bạn muốn xem: ";
            cin >> choice;
            cin.ignore();

            if (choice > 0 && choice <= teachers.size()) {
                teacher.first = teachers[choice - 1].getFirstName() + " " + teachers[choice - 1].getLastName();
                teacher.second = teachers[choice - 1].getId();
                return teacher;
            } else if (choice == 0) {
                teacher.second = -1;
                return teacher;
            }
            cout << "Lựa chọn không hợp lệ, vui lòng thử lại!" << endl;
        }
    }

    Timeslot showTimeslots(const map<string, vector<Timeslot>> &timeslots, const string &teacherName) {
        if (timeslots.empty()) {
            Timeslot ts;
            cout << "Giáo viên chưa khai báo thời gian rảnh" << endl;
            return ts;
        }

        map<int, Timeslot> editTimeslots;
        int index = 0, choice = 0;
        cout << "------------------Thời gian rảnh của " << teacherName << "-------------------" << endl;
        for (const auto &ts : timeslots) {
            cout << "--Ngày: " << ts.first << endl;
            vector<Timeslot> tss = ts.second;
            for (int i = 0; i < tss.size(); i++) {
                index++;
                editTimeslots[index] = tss[i];
                cout << index << ". Từ: " << tss[i].getStart() << " - Đến: " << tss[i].getEnd() << "( "
                     << tss[i].getType() << " )" << endl;
            }
        }

        cout << "-----------" << endl;
        while (true) {
            cout << "Bạn có muốn đăng ký thời gian nào? Nhập số dòng cần sửa: ";
            cin >> choice;
            cin.ignore();
            if (choice > 0 && choice <= editTimeslots.size()) {
                return editTimeslots[choice];
            } else if (choice == 0) {
                Timeslot ts;
                ts.setId(-1);
                return ts;
            }
        }
    }

    Meeting showBookMeeting(const Timeslot &timeslot, const string &teacherName) {
        Meeting meeting;
        string type = timeslot.getType();
        cout << "Đặt lịch hẹn với giáo viên " << teacherName << endl;
        cout << "Ngày: " << timeslot.getDate() << endl;
        cout << "Từ: " << timeslot.getStart() << " - Đến: " << timeslot.getEnd() << endl;
        if (type == "both") {
            int choice;
            while (true) {
                cout << "Bạn muốn hẹn theo phương thức nào? (1. Cá nhân, 2. Nhóm)" << endl;
                cin >> choice;
                cin.ignore();
                if (choice == 1) {
                    type = "personal";
                    break;
                } else if (choice == 2) {
                    type = "group";
                    break;
                }
                cout << "Lựa chọn không hợp lệ vui lòng thử lại!" << endl;
            }
        }
        cout << "Loại cuộc họp: " << type << endl;
        int opt;
        while (true) {
            cout << "Bạn có chắc chắn không? (1. Có, 2. Không)" << endl;
            cin >> opt;
            cin.ignore();
            if (opt == 1) {
                Meeting meet(type, timeslot.getId());
                return meet;
            } else if (opt == 2) {
                meeting.setId(-1);
                return meeting;
            }
            cout << "Lựa chọn không hợp lệ vui lòng thử lại!" << endl;
        }

        return meeting;
    }
};

#endif

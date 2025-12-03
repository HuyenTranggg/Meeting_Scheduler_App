#ifndef TEACHERVIEW_H
#define TEACHERVIEW_H
#include "../models/Meeting.h"
#include "../models/Timeslot.h"
#include "../models/User.h"
#include "../utils/Utils.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

class TeacherView {

  public:
    Utils utils;

    // Constructor
    TeacherView() {};

    int showMenu() {
        int choice;
        while (true) {
            cout << "----------------Teacher menu----------------" << endl;
            cout << "1. Khai bao thoi gian ranh" << endl;
            cout << "2. Kiem tra thoi gian ranh" << endl;
            cout << "3. Xem lich hen voi sinh vien theo ngay" << endl;
            cout << "4. Xem lich su cuoc hen" << endl;
            cout << "5. Xem lich hen voi sinh vien theo tuan" << endl;
            cout << "0. Dang xuat" << endl;
            cout << "Ban muon thuc hien chuc nang nao?" << endl;
            cin >> choice;
            cin.ignore();
            switch (choice) {
            case 0:
            case 1:
            case 2:
            case 3:
                return choice;
                break;
            default:
                cout << "Lua chon khong hop le, vui long thu lai!" << endl;
                break;
            }
        }
    }

    Timeslot showDeclareTimeSlots(const int &teacher_id) {
        string startH, startM, endH, endM, day, month, year, type;
        bool typechosen = false;
        cout << "------------------------Khai bao thoi gian ranh--------------------------" << endl;

        cout << "Nhập thoi gian bat dau:" << endl;
        while (!utils.checkHour(startH)) {
            cout << "- Gio: ";
            getline(cin, startH);
            if (!utils.checkHour(startH)) {
                cout << "Gio khong hop le!" << endl;
            }
        }
        while (!utils.checkMinute(startM)) {
            cout << "- Phut: ";
            getline(cin, startM);
            if (!utils.checkMinute(startM)) {
                cout << "Phut khong hop le!" << endl;
            }
        }

        cout << "Nhập thoi gian ket thuc:" << endl;
        while (true) {
            cout << "- Gio: ";
            getline(cin, endH);
            if (!utils.checkHour(endH)) {
                cout << "Gio khong hop le!" << endl;
            } else {
                break;
            }
        }
        while (true) {
            cout << "- Phut: ";
            getline(cin, endM);
            if (!utils.checkMinute(endM)) {
                cout << "Phut khong hop le!" << endl;
            } else {
                break;
            }
        }

        while (!utils.checkDay(day)) {
            cout << "Ngay: ";
            getline(cin, day);
            if (!utils.checkDay(day)) {
                cout << "Ngay khong hop le!" << endl;
            }
        }
        while (!utils.checkMonth(month)) {
            cout << "Thang: ";
            getline(cin, month);
            if (!utils.checkMonth(month)) {
                cout << "Thang khong hop le!" << endl;
            }
        }
        while (!utils.checkYear(year)) {
            cout << "Nam: ";
            getline(cin, year);
            if (!utils.checkYear(year)) {
                cout << "Nam khong hop le!" << endl;
            }
        }

        while (!typechosen) {
            int choice = 0;
            cout << "Loai cuoc hen: 1. Ca nhan; 2. Nhom; 3. Ca hai;" << endl;
            cin >> choice;
            cin.ignore();
            switch (choice) {
            case 1:
                type = "personal";
                typechosen = true;
                break;
            case 2:
                type = "group";
                typechosen = true;
                break;
            case 3:
                type = "both";
                typechosen = true;
                break;
            default:
                cout << "Lua chon khong hop le" << endl;
                break;
            }
        }

        cout << "--------------------------------------------" << endl;
        string start = startH + ":" + startM + ":00";
        string end = endH + ":" + endM + ":00";
        string date = year + "-" + month + "-" + day;
        Timeslot ts(start, end, date, type, teacher_id);
        return ts;
    }

    Timeslot showTimeslots(const map<string, vector<Timeslot>> &timeslots) {
        if (timeslots.empty()) {
            Timeslot ts;
            cout << "Ban chua khai bao thoi gian ranh" << endl;
            return ts;
        }

        map<int, Timeslot> editTimeslots;
        int index = 0, choice = 0;
        cout << "------------------Thoi gian ranh cua ban-------------------" << endl;
        for (const auto &ts : timeslots) {
            cout << "--Ngay: " << ts.first << endl;
            vector<Timeslot> tss = ts.second;
            for (int i = 0; i < tss.size(); i++) {
                index++;
                editTimeslots[index] = tss[i];
                cout << index << ". Tu: " << tss[i].getStart() << " - Den: " << tss[i].getEnd() << "( "
                     << tss[i].getType() << " )" << endl;
            }
        }

        cout << "-----------" << endl;
        while (true) {
            cout << "Ban co muon sua doi khong? Nhap so dong can sua: ";
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

    int showTimeslot(const Timeslot &timeslot) {
        int choice;
        cout << "-------------Thong tin thoi gian ranh-----------------" << endl;
        cout << "Ngay: " << timeslot.getDate() << endl;
        cout << "Tu: " << timeslot.getStart() << endl;
        cout << "Den: " << timeslot.getEnd() << endl;
        cout << "Loai: " << timeslot.getType() << endl;
        cout << "Trang thai: " << timeslot.getStatus() << endl;
        cout << "------------------" << endl;
        while (true) {
            cout << "Ban co muon chinh sua thoi gian khong (1 de dong y, 0 de quai lai)" << endl;
            cin >> choice;
            cin.ignore();
            if (choice == 0 || choice == 1) {
                return choice;
            }
        }
    }

    Timeslot showUpdateTimeslot(const Timeslot &timeslot) {
        int id = timeslot.getId();
        string type = timeslot.getType();
        string startH, startM, endH, endM;
        bool typechosen = false;
        Timeslot ts = timeslot;

        cout << "Nhập thoi gian bat dau:" << endl;
        while (!utils.checkHour(startH)) {
            cout << "- Gio: ";
            getline(cin, startH);
            if (!utils.checkHour(startH)) {
                cout << "Gio khong hop le!" << endl;
            }
        }
        while (!utils.checkMinute(startM)) {
            cout << "- Phut: ";
            getline(cin, startM);
            if (!utils.checkMinute(startM)) {
                cout << "Phut khong hop le!" << endl;
            }
        }

        cout << "Nhập thoi gian ket thuc:" << endl;
        while (true) {
            cout << "- Gio: ";
            getline(cin, endH);
            if (!utils.checkHour(endH) || stoi(endH) < stoi(startH)) {
                cout << "Gio khong hop le!" << endl;
            } else {
                break;
            }
        }
        while (true) {
            cout << "- Phut: ";
            getline(cin, endM);
            if (!utils.checkMinute(endM) || (stoi(startH) == stoi(endH) && stoi(endM) <= stoi(startM))) {
                cout << "Phut khong hop le!" << endl;
            } else {
                break;
            }
        }

        while (!typechosen) {
            int choice = -1;
            cout << "Loai cuoc hen: 1. Ca nhan; 2. Nhom; 3. Ca hai; 0. Khong doi" << endl;
            cin >> choice;
            cin.ignore();
            switch (choice) {
            case 0:
                typechosen = true;
                break;
            case 1:
                type = "personal";
                typechosen = true;
                break;
            case 2:
                type = "group";
                typechosen = true;
                break;
            case 3:
                type = "both";
                typechosen = true;
                break;
            default:
                cout << "Lua chon khong hop le" << endl;
                break;
            }
        }

        string start = startH + ":" + startM + ":00";
        string end = endH + ":" + endM + ":00";
        ts.setStart(start);
        ts.setEnd(end);
        ts.setType(type);
        return ts;
    }

    Meeting showMeetings(const map<string, vector<Meeting>> &meetings) {
        if (meetings.empty()) {
            Meeting meeting;
            cout << "Ban chua co cuoc hen nao" << endl;
            return meeting;
        }

        map<int, Meeting> editMeetings;
        int index = 0, choice = 0;
        cout << "------------------Lich hen cua ban-------------------" << endl;
        for (const auto &meeting : meetings) {
            cout << "--Ngay: " << meeting.first << endl;
            vector<Meeting> currentMeetings = meeting.second;
            for (int i = 0; i < currentMeetings.size(); i++) {
                index++;
                editMeetings[index] = currentMeetings[i];
                cout << index << ". Tu: " << currentMeetings[i].getStart() << " - Den: " << currentMeetings[i].getEnd()
                     << "( " << currentMeetings[i].getType() << " - " << currentMeetings[i].getStatus() << " )" << endl;
            }
        }

        cout << "-----------" << endl;
        while (true) {
            cout << "Ban co muon xem chi tiet hoac sua doi? Nhap so dong can sua: ";
            cin >> choice;
            cin.ignore();
            if (choice > 0 && choice <= editMeetings.size()) {
                return editMeetings[choice];
            } else if (choice == 0) {
                Meeting meeting;
                meeting.setId(-1);
                return meeting;
            }
        }
    }

    int showMeeting(const Meeting &meeting, const vector<User> &students) {
        int choice;
        cout << "-------------Thong tin lich hen-----------------" << endl;
        cout << "Ngay: " << meeting.getDate() << endl;
        cout << "Tu: " << meeting.getStart() << endl;
        cout << "Den: " << meeting.getEnd() << endl;
        cout << "Loai: " << meeting.getType() << endl;
        cout << "Trang thai: " << meeting.getStatus() << endl;
        cout << "Van ban cuoc hop: " << meeting.getReport() << endl;
        cout << "Nguoi hen: " << endl;
        for (const auto &student : students) {
            cout << " - " << student.getFirstName() << " " << student.getLastName() << endl;
        }
        cout << "------------------" << endl;
        while (true) {
            cout << "Ban co muon chinh sua gi khong(1 de sua van ban cuoc hop, 2 de sua trang thai, 0 de quai lai)"
                 << endl;
            cin >> choice;
            cin.ignore();
            if (choice == 0 || choice == 1 || choice == 2) {
                return choice;
            }
        }
    }

    Meeting showMeetingsByDate(const vector<pair<Meeting, vector<int>>> &meetings, const string &date) {
        if (meetings.empty()) {
            Meeting meeting;
            cout << "Khong co lich hen nao" << endl;
            return meeting;
        }

        map<int, Meeting> meetingMap;
        int index = 0;
        cout << "------------------Lich hen ngay " << date << "-------------------" << endl;
        
        for (const auto &meetingPair : meetings) {
            index++;
            const Meeting &meeting = meetingPair.first;
            const vector<int> &studentIds = meetingPair.second;
            
            meetingMap[index] = meeting;
            
            cout << index << ". Tu: " << meeting.getStart() << " - Den: " << meeting.getEnd()
                 << " (" << meeting.getType() << ")";
            
            if (!studentIds.empty()) {
                cout << " - Sinh vien IDs: ";
                for (size_t i = 0; i < studentIds.size(); i++) {
                    cout << studentIds[i];
                    if (i < studentIds.size() - 1) cout << ", ";
                }
            }
            cout << endl;
        }

        cout << "-----------" << endl;
        int choice = 0;
        while (true) {
            cout << "Chon so dong de xem chi tiet (0 de quay lai): ";
            cin >> choice;
            cin.ignore();
            
            if (choice > 0 && choice <= meetingMap.size()) {
                return meetingMap[choice];
            } else if (choice == 0) {
                Meeting meeting;
                meeting.setId(-1);
                return meeting;
            } else {
                cout << "Lua chon khong hop le!" << endl;
            }
        }
    }
};

#endif

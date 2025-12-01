#ifndef TEACHERVIEW_H
#define TEACHERVIEW_H
#include "../models/Meeting.h"
#include "../models/Timeslot.h"
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
            cout << "3. Xem lich hen voi sinh vien" << endl;
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
};

#endif

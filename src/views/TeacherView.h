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
            cout << "0. Dang xuat" << endl;
            cout << "Chọn 1 số tương ứng với lựa chọn trên:" << endl;
            cin >> choice;
            cin.ignore();
            return choice;
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
};
#endif

#ifndef USERVIEW_H
#define USERVIEW_H
#include <iostream>
#include <map>
#include <string>
using namespace std;

class UserView {

  public:
    // Constructor
    UserView() {};

    int showMenu() {
        while (true) {
            cout << "----------------Meeting Scheduler App--------------" << endl;
            cout << "1. Đăng nhập" << endl;
            cout << "2. Đăng ký" << endl;
            int choice;
            cout << "Chọn 1 số tương ứng với lựa chọn trên:" << endl;
            cin >> choice;
            cin.ignore();
            if (choice == 1 || choice == 2) {
                return choice;
            }
        }
    }

    map<string, string> showLogin() {
        map<string, string> info;
        cout << "-----------------Đăng nhập ------------------" << endl;
        cout << "Nhập tên đăng nhập: ";
        string username;
        getline(cin, username);
        info["username"] = username;

        cout << "Nhập mật khẩu: ";
        string password;
        getline(cin, password);
        info["password"] = password;

        cout << "--------------------------------------------" << endl;
        return info;
    }

    map<string, string> showRegisterA() {
        map<string, string> info;
        cout << "-----------------Đăng ký-------------------" << endl;

        // Nhập tên đăng ký
        cout << "Nhập tên đăng ký: ";
        string username;
        getline(cin, username);
        info["username"] = username;

        // Nhập mật khẩu
        cout << "Nhập mật khẩu: ";
        string password;
        getline(cin, password);
        info["password"] = password;

        // Nhập vai trò
        cout << "Nhập vai trò (student/teacher): ";
        string role;
        getline(cin, role);
        info["role"] = role;

        // Nhập first name
        cout << "Nhập họ: ";
        string firstName;
        getline(cin, firstName);
        info["first_name"] = firstName;

        // Nhập last name
        cout << "Nhập tên: ";
        string lastName;
        getline(cin, lastName);
        info["last_name"] = lastName;

        cout << "--------------------------------------------" << endl;

        return info;
    }
};

#endif

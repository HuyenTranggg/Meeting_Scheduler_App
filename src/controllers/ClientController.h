#ifndef CLIENTCONTROLLER_H
#define CLIENTCONTROLLER_H

#include "../models/Response.h"
#include "../models/User.h"
#include "../repository/TimeslotRepository.h"
#include "../repository/UserRepository.h"
#include "StudentController.h"
#include "TeacherController.h"

#include <cppconn/prepared_statement.h>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class ClientController {
  private:
    StudentController sc;
    TeacherController tc;
    UserRepository userRepository;
    TimeslotRepository timeslotRepo;

  public:
    ClientController() {}

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

    map<string, vector<Timeslot>> viewTimeslots(const string &message) {
        map<string, vector<Timeslot>> timeslots;
        vector<string> tokens = splitString(message, '|');
        string date = tokens[1];
        int i = 2;
        while (i < tokens.size()) {
            if (tokens[i - 1] == "}") {
                date = tokens[i];
                i++;
            } else if (tokens[i] == "}" || tokens[i] == "{") {
                i++;
            } else {
                // Parse timeslot data
                Timeslot ts;
                ts.setId(stoi(tokens[i]));
                ts.setStart(tokens[i + 1]);
                ts.setEnd(tokens[i + 2]);
                ts.setType(tokens[i + 3]);
                ts.setStatus(tokens[i + 4]);
                ts.setDate(date);  // Use current date
                timeslots[date].push_back(ts);
                i = i + 5;  // Move to next timeslot (5 fields per timeslot)
            }
        }
        return timeslots;
    }

    vector<User> parseTeachersFromResponse(const string &message) {
        vector<User> teachers;

        vector<string> tokens = splitString(message, '|');
        int i = 1;

        while (i < tokens.size()) {
            User user;
            user.setId(stoi(tokens[i]));
            user.setFirstName(tokens[i + 1]);
            user.setLastName(tokens[i + 2]);
            teachers.push_back(user);
            i = i + 3;
        }

        return teachers;
    }

    vector<Meeting> parseMeetingsFromResponse(const string &message) {
        vector<Meeting> meetings;

        vector<string> tokens = splitString(message, '|');
        int i = 1;

        while (i < tokens.size()) {
            Meeting meeting;
            meeting.setId(stoi(tokens[i]));
            meeting.setTeacherId(stoi(tokens[i + 1]));
            meeting.setStatus(tokens[i + 2]);
            meeting.setType(tokens[i + 3]);
            meeting.setReport(tokens[i + 4]);
            meeting.setStart(tokens[i + 5]);
            meeting.setEnd(tokens[i + 6]);
            meeting.setDate(tokens[i + 7]);
            meetings.push_back(meeting);
            i = i + 8;
        }

        return meetings;
    }

    void registerAccount() {
        bool choiceChoosen = false, usernameExist = true;
        int choice;
        string username, password, role, first_name, last_name;
        cout << "------------------------Dang ky--------------------------" << endl;
        while (!choiceChoosen) {
            cout << "1. Hoc sinh\n" << "2. Giao vien\n" << "Ban la ai (1 hoac 2): ";
            cin >> choice;
            cin.ignore();

            switch (choice) {
            case 1:
                role = "student";
                choiceChoosen = true;
                cout << "Ban la hoc sinh" << endl;
                break;
            case 2:
                role = "teacher";
                choiceChoosen = true;
                cout << "Ban la giao vien" << endl;
                break;
            default:
                cout << "Lua chon khong phu hop (1 hoac 2)" << endl;
                break;
            }
        }

        while (usernameExist) {
            cout << "Nhập tên đăng nhập: ";
            getline(cin, username);
            if (userRepository.checkUsernameExists(username)) {
                cout << "Ten dang nhap da ton tai" << endl;
            } else {
                usernameExist = false;
            }
        }

        cout << "Nhập mật khẩu: ";
        getline(cin, password);
        cout << "Nhập ho: ";
        getline(cin, first_name);
        cout << "Nhập ten: ";
        getline(cin, last_name);
        User user(username, password, role, first_name, last_name);
        userRepository.create(user);
        cout << "--------------------------------------------" << endl;
    }
};

#endif

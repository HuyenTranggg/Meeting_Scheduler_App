#ifndef STUDENTCONTROLLER_H
#define STUDENTCONTROLLER_H

#include<iostream>
#include "../models/Meeting.h"
#include "../models/Response.h"
#include "../models/Timeslot.h"

#include "../repository/TimeslotRepository.h"
#include "../repository/UserRepository.h"
#include "StudentController.h"

#include <cppconn/prepared_statement.h>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

class StudentController {
  private:
    UserRepository userRepository;
    TimeslotRepository timeslotRepo;
    Utils utils;

  public:
    StudentController() {}

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

    vector<User> getAllTeacher(const string &message) {
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
        
    map<string, vector<Timeslot>> viewTimeslots(const string &message) {
        map<string, vector<Timeslot>> timeslots;
        vector<string> tokens = splitString(message, '|');

        // Kiểm tra an toàn để tránh crash nếu message không hợp lệ
        if (tokens.size() < 2) {
            return timeslots; // Trả về map rỗng nếu không có đủ token
        }        

        string date = tokens[1];
        int i = 2;
        while (i < tokens.size()) {
            if (tokens[i - 1] == "}") {
                date = tokens[i];
                i++;
            } else if (tokens[i] == "}" || tokens[i] == "{") {
                i++;
            } else {
                if (i + 4 < tokens.size()) {
                    Timeslot ts;
                    ts.setId(safeStoi(tokens[i]));
                    ts.setStart(tokens[i + 1]);
                    ts.setEnd(tokens[i + 2]);
                    ts.setType(tokens[i + 3]);  
                    ts.setStatus(tokens[i + 4]); 

                    // Gán date cho timeslot từ biến date chung của ngày đó
                    ts.setDate(date);

                    timeslots[date].push_back(ts);
                    i = i + 5; 
                } else {
                    // Nếu không đủ token, thoát vòng lặp để tránh crash
                    break;
                }
            }
        }
        return timeslots;
    }

};

#endif

#ifndef TEACHERCONTROLLER_H
#define TEACHERCONTROLLER_H

#include "../models/Response.h"
#include "../models/Timeslot.h"
#include "../models/User.h"

#include "../repository/TimeslotRepository.h"
#include "../repository/UserRepository.h"
#include "TeacherController.h"

#include <cppconn/prepared_statement.h>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

class TeacherController {
  private:
    UserRepository userRepository;
    TimeslotRepository timeslotRepo;
    // Utils utils;

  public:
    TeacherController() {}

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

    map<string, vector<Timeslot>> getTimeslotsFromResponse(const string &message) {
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

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

    map<string, map<string, vector<pair<Meeting, User>>>> getMeetingsInWeeksFromResponse(const string &message) {
        map<string, map<string, vector<pair<Meeting, User>>>> meetings;
        vector<string> tokens = splitString(message, '|');
        
        if (tokens.size() < 4) return meetings; // Kiểm tra an toàn
        
        string week = "";
        string date = "";
        int i = 1; // Bỏ qua status code
        bool inWeek = false;
        
        while (i < tokens.size()) {
            string token = tokens[i];
            
            // Bỏ qua token rỗng
            if (token.empty()) {
                i++;
                continue;
            }
            
            // Kiểm tra dấu mở week
            if (token == "{") {
                inWeek = true;
                i++;
                continue;
            }
            
            // Kiểm tra dấu đóng week
            if (token == "}") {
                inWeek = false;
                i++;
                continue;
            }
            
            // Nếu chưa vào week và gặp string, đó là week name
            if (!inWeek && token != "{" && token != "}" && 
                !(token.length() == 10 && token[4] == '-' && token[7] == '-')) {
                week = token;
                i++;
                continue;
            }
            
            // Kiểm tra date (format YYYY-MM-DD)
            if (token.length() == 10 && token[4] == '-' && token[7] == '-') {
                date = token;
                i++;
                continue;
            }
            
            // Parse meeting: meeting_id|timeslot_id|status|type|start|end
            // Sau đó là teacher: teacher_id|teacher_name
            if (i + 7 < tokens.size() && inWeek) {
                // Kiểm tra xem có phải là meeting data không (token hiện tại phải là số)
                if (safeStoi(token, -1) > 0) {
                    Meeting meeting;
                    meeting.setId(safeStoi(token));
                    meeting.setTimeslotId(safeStoi(tokens[i + 1]));
                    meeting.setStatus(tokens[i + 2]);
                    meeting.setType(tokens[i + 3]);
                    meeting.setStart(tokens[i + 4]);
                    meeting.setEnd(tokens[i + 5]);
                    meeting.setDate(date);
                    
                    // Parse teacher: teacher_id|teacher_name
                    User teacher;
                    teacher.setId(safeStoi(tokens[i + 6]));
                    string fullName = tokens[i + 7];
                    size_t spacePos = fullName.find(' ');
                    if (spacePos != string::npos) {
                        teacher.setFirstName(fullName.substr(0, spacePos));
                        teacher.setLastName(fullName.substr(spacePos + 1));
                    } else {
                        teacher.setFirstName(fullName);
                        teacher.setLastName("");
                    }
                    
                    pair<Meeting, User> meetingWithTeacher;
                    meetingWithTeacher.first = meeting;
                    meetingWithTeacher.second = teacher;
                    meetings[week][date].push_back(meetingWithTeacher);
                    
                    i += 8; // Bỏ qua 6 fields của meeting + 2 fields của teacher
                } else {
                    i++;
                }
            } else {
                i++;
            }
        }
        
        return meetings;
    }

    pair<Meeting, User> getMeetingFromResponse(const string &message) {
        pair<Meeting, User> meetingDetail;
        User teacher;
        vector<string> tokens = splitString(message, '|');

        Meeting meeting;
        meeting.setId(stoi(tokens[1]));
        meeting.setTimeslotId(stoi(tokens[2]));
        meeting.setStatus(tokens[3]);
        meeting.setType(tokens[4]);
        meeting.setReport(tokens[5]);
        meeting.setStart(tokens[6]);
        meeting.setEnd(tokens[7]);
        meeting.setDate(tokens[8]);
        meetingDetail.first = meeting;
        // 9 = [
        teacher.setId(stoi(tokens[10]));
        teacher.setFirstName(tokens[11]);
        teacher.setLastName(tokens[12]);
        meetingDetail.second = teacher;
        return meetingDetail;
    }

};

#endif

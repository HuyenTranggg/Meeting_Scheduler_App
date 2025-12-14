#ifndef TEACHERCONTROLLER_H
#define TEACHERCONTROLLER_H

#include "../models/Meeting.h"
#include "../models/Response.h"
#include "../models/Timeslot.h"
#include "../models/User.h"

#include "../repository/TimeslotRepository.h"
#include "../repository/UserRepository.h"

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

    map<string, vector<Meeting>> getMeetingsFromResponse(const string &message) {
        map<string, vector<Meeting>> meetings;
        vector<string> tokens = splitString(message, '|');
        string date = tokens[1];
        int i = 2;
        while (i < tokens.size()) {
            if (tokens[i - 1] == "]") {
                date = tokens[i];
                i++;
            } else if (tokens[i] == "]" || tokens[i] == "[") {
                i++;
            } else {
                Meeting meeting;
                meeting.setId(stoi(tokens[i]));
                meeting.setTimeslotId(stoi(tokens[i + 1]));
                meeting.setStatus(tokens[i + 2]);
                meeting.setType(tokens[i + 3]);
                meeting.setReport(tokens[i + 4]);
                meeting.setStart(tokens[i + 5]);
                meeting.setEnd(tokens[i + 6]);
                meeting.setDate(tokens[i + 7]);
                meetings[date].push_back(meeting);
                i = i + 8;
            }
        }
        return meetings;
    }

    pair<Meeting, vector<User>> getMeetingFromResponse(const string &message) {
        pair<Meeting, vector<User>> meetingDetail;
        vector<User> students;
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
        int i = 10;
        while (i < tokens.size()) {
            if (tokens[i] == "]") {
                break;
            }
            User student;
            student.setId(stoi(tokens[i]));
            student.setFirstName(tokens[i + 1]);
            student.setLastName(tokens[i + 2]);
            students.push_back(student);
            i = i + 3;
        };
        meetingDetail.second = students;
        return meetingDetail;
    }

    vector<pair<Meeting, vector<int>>> getMeetingsByDateFromResponse(const string &response) {
        vector<pair<Meeting, vector<int>>> meetings;
        vector<string> tokens = splitString(response, '|');
        
        // Format: 0|meeting_id|start|end|type|student_id1,student_id2|meeting_id|...
        // Or: 0|No meetings found|
        
        if (tokens.size() < 2) return meetings;
        
        int i = 1; // Skip status
        while (i < tokens.size()) {
            if (tokens[i].empty() || tokens[i] == "<END>") break;
            
            // Check if token is a valid number (meeting_id should be numeric)
            try {
                Meeting meeting;
                meeting.setId(stoi(tokens[i]));     // meeting_id
                
                // Make sure we have enough tokens
                if (i + 4 >= tokens.size()) break;
                
                meeting.setStart(tokens[i + 1]);     // start
                meeting.setEnd(tokens[i + 2]);       // end
                meeting.setType(tokens[i + 3]);      // type
                
                // Parse student IDs (comma-separated)
                vector<int> studentIds;
                if (!tokens[i + 4].empty() && tokens[i + 4] != "<END>") {
                    vector<string> idTokens = splitString(tokens[i + 4], ',');
                    for (const auto &idStr : idTokens) {
                        if (!idStr.empty()) {
                            studentIds.push_back(stoi(idStr));
                        }
                    }
                }
                
                meetings.push_back(make_pair(meeting, studentIds));
                i += 5; // Move to next meeting
            } catch (const invalid_argument &e) {
                // Not a meeting_id (probably a message like "No meetings found")
                break;
            } catch (const out_of_range &e) {
                break;
            }
        }
        
        return meetings;
    }

    map<string, map<string, vector<pair<Meeting, vector<User>>>>>
    getMeetingsInWeeksFromResponse(const string &message) {
        map<string, map<string, vector<pair<Meeting, vector<User>>>>> meetings;
        vector<string> tokens = splitString(message, '|');
        string week = tokens[1];
        string date = tokens[3];
        int i = 2;
        while (i < tokens.size()) {
            if (tokens[i - 1] == "}" && tokens[i - 2] == "]") {
                week = tokens[i];
                i++;
            } else if ((tokens[i - 1] == "{" && tokens[i - 2] != "students") ||
                       (tokens[i - 1] == "]" && tokens[i - 2] == "}")) {
                date = tokens[i];
                i++;
            } else if (tokens[i] == "]" || tokens[i] == "[" || tokens[i] == "}" || tokens[i] == "{") {
                i++;
            } else {
                // [
                pair<Meeting, vector<User>> meetingWithUser;
                vector<User> students;
                Meeting meeting;
                meeting.setId(stoi(tokens[i]));
                meeting.setTimeslotId(stoi(tokens[i + 1]));
                meeting.setStatus(tokens[i + 2]);
                meeting.setType(tokens[i + 3]);
                meeting.setReport(tokens[i + 4]);
                meeting.setStart(tokens[i + 5]);
                meeting.setEnd(tokens[i + 6]);
                meeting.setDate(tokens[i + 7]);
                meetingWithUser.first = meeting;
                // i + 8 == students; i + 9 == "{"
                i = i + 10; //
                while (tokens[i] != "}") {
                    User student;
                    student.setId(stoi(tokens[i]));
                    student.setFirstName(tokens[i + 1]);
                    student.setLastName(tokens[i + 2]);
                    students.push_back(student);
                    i = i + 3;
                }
                meetingWithUser.second = students;
                meetings[week][date].push_back(meetingWithUser);
            }
        }
        return meetings;
    }

    vector<User> seeStudentList(const string &message) {
        vector<User> students;

        vector<string> tokens = splitString(message, '|');
        int i = 1;

        while (i < tokens.size()) {
            User user;
            user.setId(stoi(tokens[i]));
            user.setFirstName(tokens[i + 1]);
            user.setLastName(tokens[i + 2]);
            students.push_back(user);
            i = i + 3;
        }

        return students;
    }
};

#endif

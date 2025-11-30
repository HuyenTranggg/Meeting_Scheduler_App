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
    
    map<string, vector<Timeslot>> viewTimeslots(const string &message) {
        map<string, vector<Timeslot>> timeslots;
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
                // [
                Timeslot ts;
                ts.setId(stoi(tokens[i]));
                ts.setStart(tokens[i + 1]);
                ts.setEnd(tokens[i + 2]);
                ts.setDate(tokens[i + 3]);
                ts.setType(tokens[i + 4]);
                ts.setStatus(tokens[i + 5]);
                ts.setTeacherId(stoi(tokens[i + 6]));
                timeslots[date].push_back(ts);
                i = i + 7;
            }
        }
        return timeslots;
    }

};

#endif

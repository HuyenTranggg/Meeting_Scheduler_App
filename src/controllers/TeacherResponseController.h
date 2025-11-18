#ifndef TEACHERRESPONSECONTROLLER_H
#define TEACHERRESPONSECONTROLLER_H

#include "../models/Meeting.h"
#include "../models/Response.h"
#include "../models/Timeslot.h"
#include "../models/User.h"
#include "../repository/TimeslotRepository.h"
#include "../repository/UserRepository.h"

#include <cppconn/prepared_statement.h>
#include <iostream>
#include <map>

using namespace std;

class TeacherResponseController {
  private:
    TimeslotRepository timeslotRepo;

  public:
    TeacherResponseController() {}

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

    Response declareTimeslot(const string &message) {
        Response res;
        vector<string> tokens = splitString(message, '|');
        string start = tokens[1];
        string end = tokens[2];
        string date = tokens[3];
        string type = tokens[4];
        int teacher_id = stoi(tokens[5]);
        Timeslot ts(start, end, date, type, teacher_id);
        if (timeslotRepo.check(start, end, date, teacher_id)) {
            res.setStatus(14);
            res.setMessage("Time slot conflicts with another slot|");
        } 
        else if(timeslotRepo.checkStartEnd(start, end)) {
            res.setStatus(6);
            res.setMessage("Invalid time|");
        }
        else {
            timeslotRepo.create(ts);
            res.setStatus(0);
            res.setMessage("Successfully declared free time|");
        }

        return res;
    }
};
// Nếu thành công: 0 | Successfully declared free time | <END>
// Nếu thời gian không hợp lệ: 6 | Invalid time | <END>
// Trùng thời gian với slot khác: 14 | Time slot conflicts with another slot | <END>
#endif

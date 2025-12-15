#ifndef TEACHERRESPONSECONTROLLER_H
#define TEACHERRESPONSECONTROLLER_H

#include "../models/Meeting.h"
#include "../models/Response.h"
#include "../models/Timeslot.h"
#include "../models/User.h"
#include "../repository/AttendanceRepository.h"
#include "../repository/MeetingRepository.h"
#include "../repository/TimeslotRepository.h"
#include "../repository/UserRepository.h"

#include <cppconn/prepared_statement.h>
#include <iostream>
#include <map>
#include <sstream> // Thêm thư viện cần thiết
#include <vector>  // Thêm thư viện cần thiết

using namespace std;

class TeacherResponseController {
  private:
    UserRepository userRepository;
    TimeslotRepository timeslotRepo;
    MeetingRepository meetingRepo;
    AttendanceRepository attendanceRepo;
    Utils utils; // them utils

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
    // Teacher
    Response viewTimeslots(const int &teacher_id) {
        Response res;
        User user = userRepository.getUserById(teacher_id);
        if (user.getId() == 0) {
            res.setStatus(15);
            res.setMessage("Teacher not found|");
        } else {
            map<string, vector<Timeslot>> timeslots = timeslotRepo.getTimeslotsByTeacherId(teacher_id);
            if (timeslots.empty()) {
                res.setStatus(4);
                res.setMessage("Teacher has no available time|");
            } else {
                string message = "";
                bool first_date = true;
                for (const auto &ts : timeslots) {

                    if (!first_date) {
                        message += "|";
                    }
                    message += ts.first + "|{|"; 
                    vector<Timeslot> tss = ts.second;
                    for (int i = 0; i < tss.size(); i++) {
                        message += tss[i].toString();
                        if (i < tss.size() - 1) {
                            message += "|"; // Thêm | giữa các timeslot
                        }
                    }
                    message += "|}"; 
                    first_date = false;
                }
                res.setStatus(0);
                res.setMessage(message + "|");

            }
        }

        return res;
    }

    Response updateTimeslot(const string &message) {
        Response res;
        vector<string> tokens = splitString(message, '|');

        if (tokens.size() < 6) {
             res.setStatus(99); // Mã lỗi tự định nghĩa cho request sai
             res.setMessage("Yeu cau khong hop le|");
             return res;
        }
        
        int teacher_id = stoi(tokens[0]); // Bị thiếu
        int slot_id = stoi(tokens[1]);    // Sai index
        string start = tokens[2];         // Sai index
        string end = tokens[3];           // Sai index
        string date = tokens[4];          // Bị thiếu
        string type = tokens[5];          // Sai index

        Timeslot ts = timeslotRepo.getTimeslotById(slot_id);
        if (ts.getId() == 0) {

            res.setStatus(7);

            res.setMessage("Time slot not found|");

        } else if (utils.isTimeInvalid(start, end)) { 
            res.setStatus(6);
            res.setMessage("Invalid time|");
        } else if (timeslotRepo.check2(start, end, ts.getDate(), ts.getTeacherId(), slot_id)) {
            res.setStatus(14);
            res.setMessage("Time slot conflicts with another slot|");
        } else {
            timeslotRepo.updateTimeAndType(slot_id, start, end, type);
            res.setStatus(0);
            res.setMessage("Successfully updated free time|");
        }

        return res;
    }

    Response viewMeetingsByDate(const int &teacher_id, const string &date) {
        Response res;
        vector<Meeting> meetings = meetingRepo.getMeetingsByTeacherIdAndDate(teacher_id, date);
        
        if (meetings.empty()) {
            res.setStatus(0);
            res.setMessage("No meetings found|");
        } else {
            string message = "";
            for (size_t i = 0; i < meetings.size(); i++) {
                const Meeting &meeting = meetings[i];
                
                // Format: meeting_id|start|end|type|student_ids
                message += to_string(meeting.getId()) + "|";
                message += meeting.getStart() + "|";
                message += meeting.getEnd() + "|";
                message += meeting.getType() + "|";
                
                // Get student IDs for this meeting
                vector<User> students = attendanceRepo.getStudentsFromMeeting(meeting.getId());
                for (size_t j = 0; j < students.size(); j++) {
                    message += to_string(students[j].getId());
                    if (j < students.size() - 1) {
                        message += ",";
                    }
                }
                message += "|";
            }
            res.setStatus(0);
            res.setMessage(message);
        }
        
        return res;
    }

    Response viewMeeting(const int &meeting_id) {
        Response res;
        Meeting meeting = meetingRepo.getMeetingById(meeting_id);

        if (meeting.getId() == 0) {
            res.setStatus(12);
            res.setMessage("Không tìm thấy cuộc họp|");
            return res;
        }

        vector<User> students = attendanceRepo.getStudentsFromMeeting(meeting_id);

        string message = meeting.toString();
        message += "|[";
        for (const auto &student : students) {
            message += "|" + to_string(student.getId()) + "|" + student.getFirstName() + "|" + student.getLastName();
        }
        message += "|]|";
        res.setStatus(0);
        res.setMessage(message);

        return res;
    }

    Response viewHistory(const int &teacher_id, const int &student_id) {
        Response res;
        User user = userRepository.getUserById(teacher_id);
        if (user.getId() == 0) {
            res.setStatus(8);
            res.setMessage("Giáo viên không tồn tại|");
        } else {
            map<string, map<string, vector<Meeting>>> meetings =
                meetingRepo.getDoneMeetingsByTeacherIdAndStudentId(teacher_id, student_id);
            if (meetings.empty()) {
                res.setStatus(18);
                res.setMessage("Không có lịch sử cuộc họp|");
            } else {
                string message = "";
                for (const auto &week : meetings) {
                    const string weekName = week.first;
                    const map<string, vector<Meeting>> dailyMeetings = week.second;
                    message += weekName + "|{";
                    for (const auto &day : dailyMeetings) {
                        const string dayName = day.first;
                        vector<Meeting> mts = day.second;
                        message += "|" + dayName + "|[";
                        for (int i = 0; i < mts.size(); i++) {
                            message += "|" + mts[i].toString();
                            vector<User> students = attendanceRepo.getStudentsFromMeeting(mts[i].getId());
                            message += "|students|{";
                            for (const auto &student : students) {
                                message += "|" + to_string(student.getId()) + "|" + student.getFirstName() + "|" +
                                           student.getLastName();
                            }
                            message += "|}";
                        }
                        message += "|]";
                    }
                    message += "|}|";
                }
                res.setStatus(0);
                res.setMessage(message);
            }
        }

        return res;
    }

    Response updateReport(const string &message) {
        Response res;
        vector<string> tokens = splitString(message, '|');
        int meeting_id = stoi(tokens[1]);
        string report = tokens[2];
        Meeting meeting = meetingRepo.getMeetingById(meeting_id);
        if (meeting.getId() == 0) {
            res.setStatus(5);
            res.setMessage("Meeting does not exist|");
        } else {
            meetingRepo.updateReport(meeting_id, report);
            res.setStatus(0);
            res.setMessage("Success|");
        }
        return res;
    }

    Response updateStatus(const string &message) {
        Response res;
        vector<string> tokens = splitString(message, '|');
        int meeting_id = stoi(tokens[1]);
        string status = tokens[2];
        Meeting meeting = meetingRepo.getMeetingById(meeting_id);
        if (meeting.getId() == 0) {
            res.setStatus(12);
            res.setMessage("Khong tim thay cuoc hen|");
        } else {
            if (status == "canceled") {
                meetingRepo.deleteMeeting(meeting_id);
                timeslotRepo.updateStatus(meeting.getTimeslotId(), "free");
            } else {
                meetingRepo.updateStatus(meeting_id, status);
            }
            res.setStatus(0);
            res.setMessage("Success|");
        }
        return res;
    }

    Response getStudentList(const string &message) {
        Response res;
        vector<string> tokens = splitString(message, '|');
        int teacher_id = stoi(tokens[1]);
        vector<User> students = userRepository.getStudentsInHistory(teacher_id);

        if (students.empty()) {
            res.setStatus(17);
            res.setMessage("Không có sinh viên nào đã hẹn với bạn|");
        } else {
            string message = "";
            for (int i = 0; i < students.size(); i++) {
                message += students[i].toStringProfile() + "|";
            }
            res.setStatus(0);
            res.setMessage(message);
        }
        return res;
    }    
};
// Nếu thành công: 0 | Successfully declared free time | <END>
// Nếu thời gian không hợp lệ: 6 | Invalid time | <END>
// Trùng thời gian với slot khác: 14 | Time slot conflicts with another slot | <END>
#endif

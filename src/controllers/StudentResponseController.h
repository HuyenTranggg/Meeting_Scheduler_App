#ifndef STUDENTRESPONSECONTROLLER_H
#define STUDENTRESPONSECONTROLLER_H

#include "../models/Meeting.h"
#include "../models/Response.h"
#include "../models/User.h"
#include "../models/Attendance.h"
#include "../repository/TimeslotRepository.h"
#include "../repository/UserRepository.h"
#include "./TeacherResponseController.h"
#include "../repository/MeetingRepository.h"
#include "../repository/AttendanceRepository.h"
#include "../utils/Utils.h"
#include <iostream>
#include <vector>
using namespace std;

class StudentResponseController {
  private:
    UserRepository userRepository;
    TimeslotRepository timeslotRepository;
    TeacherResponseController trc;
    MeetingRepository meetingRepository;
    AttendanceRepository attendanceRepository;
    Utils utils;

  public:
    StudentResponseController() {}

    bool check(const string &start, const string &end, const string &date, const int &student_id) {
        vector<Meeting> studenMeetings = meetingRepository.getMeetingsInDateByStudentId(student_id, date);
        for (const Meeting& meeting : studenMeetings) {
            if (!(utils.checkTimeGreater(meeting.getStart(), end) || utils.checkTimeGreater(start, meeting.getEnd()))) {
                return true;
            }
        }
        return false;
    }

    Response getAllTeacher() {
        Response res;
        vector<User> teachers = userRepository.getAllTeachers();

        if (teachers.empty()) {
            res.setStatus(15);
            res.setMessage("No teacher was found|");
        } else {
            string message = "";
            for (int i = 0; i < teachers.size(); i++) {
                message += teachers[i].toStringProfile() + "|";
            }
            res.setStatus(0);
            res.setMessage(message);
        }

        return res;
    }
    
    Response bookMeeting(const string &request) {
        Response res;
        vector<string> tokens = trc.splitString(request, '|');
        int student_id = stoi(tokens[1]);
        int timeslot_id = stoi(tokens[2]);
        string type = tokens[3];

        User student = userRepository.getUserById(student_id);
        if (student.getId() == 0) {
            res.setStatus(9);
            res.setMessage("Student is not exist|");
            return res;
        }

        Timeslot timeslot = timeslotRepository.getTimeslotById(timeslot_id);
        if (timeslot.getId() == 0) {
            res.setStatus(7);
            res.setMessage("Timeslot not found|");
            return res;
        }
        
        // timeslot trong quá khứ 
        if (utils.isDateBeforeToday(timeslot.getDate()) ||
            (utils.isDateToday(timeslot.getDate()) && utils.checkTimeGreater(utils.getCurrentTime(), timeslot.getStart()))) {
            res.setStatus(18);
            res.setMessage("Cannot book a timeslot in the past|");
            return res;
        } 

        if(timeslot.getStatus() == "busy") {
            res.setStatus(17);
            res.setMessage("Timeslot is busy|");
            return res;
        }     

        // Kiểm tra xem có xung đột với thời gian của Timeslot đã hẹn không
        if (check(timeslot.getStart(), timeslot.getEnd(), timeslot.getDate(), student_id)) {
            res.setStatus(12);
            res.setMessage("You have a meeting at this time|");
            return res;
        }

        // Kiem tra xem meeting ton tai chua
        Meeting meeting = meetingRepository.getMeetingByTimeslotId(timeslot_id);
        if (meeting.getId() != 0 && meeting.getType() == "personal") {
            res.setStatus(17);
            res.setMessage("Timeslot is busy|");
            return res;
        } else if (meeting.getId() == 0) {
            // Tao meeting và lưu original_type của timeslot
            Meeting newMeeting(type, timeslot.getType(), timeslot_id);
            meetingRepository.create(newMeeting);
            meeting = meetingRepository.getMeetingByTimeslotId(timeslot_id);
        }

        // Update type cho timeslot neu no la both
        if (timeslot.getType() == "both") {
            timeslotRepository.updateType(timeslot_id, type);
        }

        // Update status cho timeslot neu no la personal
        if (type == "personal") {
            timeslotRepository.updateStatus(timeslot_id, "busy");
        }

        // Them attendance
        // Kiem tra attendance ton tai chua
        Attendance att = attendanceRepository.getAttendanceByMeetingIdAndStudentId(meeting.getId(), student_id);
        if (att.getId() == 0) {
            Attendance newAttendance(meeting.getId(), student_id);
            attendanceRepository.create(newAttendance);
        } else {
            res.setStatus(11);
            res.setMessage("You already registered for this meeting|");
            return res;
        }

        res.setStatus(0);
        res.setMessage("Meeting booked successfully|");

        return res;
    }

    Response viewMeetingsStudent(const string &request) {
        Response res;
        vector<string> tokens = trc.splitString(request, '|');
        int student_id = stoi(tokens[1]);
        User student = userRepository.getUserById(student_id);
        if (student.getId() == 0 || student.getRole() != "student") {
            res.setStatus(17);
            res.setMessage("Student not found|");
            return res;
        } else {
            map<string, map<string, vector<Meeting>>> meetings =
                meetingRepository.getMeetingsInWeeksByStudentId(student_id);
            if (meetings.empty()) {
                res.setStatus(8);
                res.setMessage("No meeting found|");
                return res;
            } else {
                string message = "";
                for (const auto &week : meetings) {
                    const string weekName = week.first;
                    const map<string, vector<Meeting>> dailyMeetings = week.second;
                    message += weekName + "|{";  // Bắt đầu week
                    
                    for (const auto &day : dailyMeetings) {
                        const string dayName = day.first;
                        vector<Meeting> mts = day.second;
                        message += "|" + dayName;  // Thêm date
                        
                        // Thêm từng meeting
                        for (int i = 0; i < mts.size(); i++) {
                            // Format meeting: meeting_id|timeslot_id|status|type|start|end
                            message += "|" + to_string(mts[i].getId()) + "|" + 
                                    to_string(mts[i].getTimeslotId()) + "|" + 
                                    mts[i].getStatus() + "|" + 
                                    mts[i].getType() + "|" + 
                                    mts[i].getStart() + "|" + 
                                    mts[i].getEnd();
                            
                            // Format teacher: teacher_id|teacher_name
                            User teacher = userRepository.getTeacherFromMeeting(mts[i].getId());
                            message += "|" + to_string(teacher.getId()) + "|" + 
                                    teacher.getFirstName() + " " + teacher.getLastName();
                        }
                    }
                    message += "|}";  // Kết thúc week
                    message += "|";  // Phân cách giữa các week
                }
                res.setStatus(0);
                res.setMessage(message);
            }
        }
        return res;
    }

    Response viewMeetingStudent(const string &request) {
        Response res;
        vector<string> tokens = trc.splitString(request, '|');
        int meeting_id = stoi(tokens[1]);
        
        // Kiểm tra meeting có tồn tại không
        Meeting meeting = meetingRepository.getMeetingById(meeting_id);
        if (meeting.getId() == 0) {
            res.setStatus(5);
            res.setMessage("Meeting does not exist|");
            return res;
        }
        
        User teacher = userRepository.getTeacherFromMeeting(meeting_id);
        string message = meeting.toString();
        message += "|[";
        message += "|" + to_string(teacher.getId()) + "|" + teacher.getFirstName() + "|" + teacher.getLastName();
        message += "|]|";
        res.setStatus(0);
        res.setMessage(message);

        return res;
    }

    Response cancelMeeting(const string &request) {
        Response res;
        vector<string> tokens = trc.splitString(request, '|');
        int student_id = stoi(tokens[1]);
        int meeting_id = stoi(tokens[2]);
        
        // Kiểm tra student_id
        User student = userRepository.getUserById(student_id);
        if (student.getId() == 0 || student.getRole() != "student") {
            res.setStatus(9);
            res.setMessage("Student does not exist|");
            return res;
        }
        
        Meeting meeting = meetingRepository.getMeetingById(meeting_id);
        if (meeting.getId() == 0) {
            res.setStatus(5);
            res.setMessage("Meeting does not exist|");
            return res;
        }
        
        // Kiểm tra trạng thái: chỉ cho phép hủy khi status là pending hoặc confirmed
        string status = meeting.getStatus();
        if (status != "pending" && status != "confirmed") {
            res.setStatus(19);
            if (status == "doing") {
                res.setMessage("Cannot cancel meeting in progress|");
            } else if (status == "completed") {
                res.setMessage("Cannot cancel completed meeting|");
            } else if (status == "canceled") {
                res.setMessage("Meeting has already been canceled|");
            } else {
                res.setMessage("Cannot cancel meeting in current status|");
            }
            return res;
        }
        
        // Thực hiện hủy lịch hẹn
        if (meeting.getType() == "group") {
            vector<Attendance> attendances = attendanceRepository.getAttendancesByMeetingId(meeting_id);
            if (attendances.size() == 1) {
                // Đây là attendance cuối cùng, xóa meeting và restore timeslot
                timeslotRepository.updateStatus(meeting.getTimeslotId(), "free");
                timeslotRepository.updateType(meeting.getTimeslotId(), meeting.getOriginalType());
                meetingRepository.deleteMeeting(meeting_id);
            }
        }
        if (meeting.getType() == "personal") {
            timeslotRepository.updateStatus(meeting.getTimeslotId(), "free");
            timeslotRepository.updateType(meeting.getTimeslotId(), meeting.getOriginalType());
            meetingRepository.deleteMeeting(meeting_id);
        }
        attendanceRepository.deleteAttendanceByMeetingIdAndStudentId(meeting_id, student_id);
        
        res.setStatus(0);
        res.setMessage("Meeting is cancelled|");
        return res;
    }

};

#endif

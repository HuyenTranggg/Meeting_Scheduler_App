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
        if (meeting.getId() != 0) {
            res.setStatus(16);
            res.setMessage("Meeting already exists|");
            return res;
        } else if (meeting.getId() == 0) {
            // Tao meeting
            Meeting newMeeting(type, timeslot_id);
            meetingRepository.create(newMeeting);
            meeting = meetingRepository.getMeetingByTimeslotId(timeslot_id);
        }

        // Update type cho timeslot neu no la both
        if (timeslot.getType() == "both") {
            timeslotRepository.updateType(timeslot_id, type);
        }

        timeslotRepository.updateStatus(timeslot_id, "busy");

        // Them attendance
        // Kiem tra attendance ton tai chua
        Attendance att = attendanceRepository.getAttendanceByMeetingIdAndStudentId(meeting.getId(), student_id);
        if (att.getId() == 0) {
            Attendance newAttendance(meeting.getId(), student_id);
            attendanceRepository.create(newAttendance);
        } else {
            res.setStatus(11);
            res.setMessage("Student already registered for this meeting|");
            return res;
        }

        res.setStatus(0);
        res.setMessage("Meeting booked successfully|");

        return res;
    }

};

#endif

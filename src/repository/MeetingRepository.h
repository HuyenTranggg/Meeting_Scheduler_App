#ifndef MEETINGREPOSITORY_H
#define MEETINGREPOSITORY_H

#include "../../data/Database.h"
#include "../models/Attendance.h"
#include "../models/Meeting.h"
#include "../models/Timeslot.h"
#include "../models/User.h"
#include "./TimeslotRepository.h"
#include "UserRepository.h"
#include <cppconn/prepared_statement.h>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

class MeetingRepository {
  private:
    Database db;
    UserRepository userRepo;
    TimeslotRepository timeslotRepo;

  public:
    MeetingRepository() {}

    void create(const Meeting &meeting) {
        if (db.connect()) {
            string query = "INSERT INTO meetings (timeslot_id, type ) VALUES (?, ?)";

            try {
                sql::PreparedStatement *pstmt = db.getConnection()->prepareStatement(query);
                pstmt->setInt(1, meeting.getTimeslotId());
                pstmt->setString(2, meeting.getType());
                pstmt->executeUpdate();
                delete pstmt;
            } catch (sql::SQLException &e) {
                cerr << "Lỗi thêm cuộc hẹn: " << e.what() << endl;
            }

        } else {
            cout << "Lỗi không thể truy cập cơ sở dữ liệu." << endl;
        }
    }

    Meeting getMeetingByTimeslotId(const int &timeslot_id) {
        Meeting meeting;
        if (db.connect()) {
            string query = "SELECT * FROM meetings WHERE timeslot_id  = ?";
            try {
                sql::PreparedStatement *pstmt = db.getConnection()->prepareStatement(query);
                pstmt->setInt(1, timeslot_id);
                sql::ResultSet *res = pstmt->executeQuery();

                if (res->next()) {
                    Timeslot timeslot = timeslotRepo.getTimeslotById(res->getInt("timeslot_id"));
                    meeting.setId(res->getInt("id"));
                    meeting.setTeacherId(timeslot.getTeacherId());
                    meeting.setStatus(res->getString("status"));
                    meeting.setType(res->getString("type"));
                    meeting.setReport(res->getString("report"));
                    meeting.setStart(timeslot.getStart());
                    meeting.setEnd(timeslot.getEnd());
                    meeting.setDate(timeslot.getDate());
                    meeting.setTimeslotId(res->getInt("timeslot_id"));
                }

                delete res;
                delete pstmt;
            } catch (sql::SQLException &e) {
                std::cerr << "Lỗi khi lấy dữ liệu từ timeslots: " << e.what() << std::endl;
            }

        } else {
            cout << "Lỗi không thể truy cập cơ sở dữ liệu." << endl;
        }
        return meeting;
    }

    // Lay lich hen chua hoan thanh
    map<string, vector<Meeting>> getWaitingMeetingsByTeacherId(const int &teacher_id) {
        map<string, vector<Meeting>> timeslots;

        if (db.connect()) {
            string query = R"(
                SELECT meetings.*
                FROM meetings
                INNER JOIN timeslots ON meetings.timeslot_id = timeslots.id
                INNER JOIN users ON timeslots.teacher_id = users.id
                WHERE users.id = ? 
                AND (meetings.status = 'pending' OR meetings.status = 'confirmed' OR meetings.status = 'doing')
            )";
            try {
                sql::PreparedStatement *pstmt = db.getConnection()->prepareStatement(query);
                pstmt->setInt(1, teacher_id);
                sql::ResultSet *res = pstmt->executeQuery();

                while (res->next()) {
                    Meeting meeting;
                    Timeslot timeslot = timeslotRepo.getTimeslotById(res->getInt("timeslot_id"));
                    meeting.setId(res->getInt("id"));
                    meeting.setTeacherId(timeslot.getTeacherId());
                    meeting.setStatus(res->getString("status"));
                    meeting.setType(res->getString("type"));
                    meeting.setReport(res->getString("report"));
                    meeting.setStart(timeslot.getStart());
                    meeting.setEnd(timeslot.getEnd());
                    meeting.setDate(timeslot.getDate());
                    meeting.setTimeslotId(res->getInt("timeslot_id"));
                    string date = meeting.getDate();
                    timeslots[date].push_back(meeting);
                }
                delete res;
                delete pstmt;
            } catch (sql::SQLException &e) {
                std::cerr << "Lỗi khi lấy dữ liệu từ meetings: " << e.what() << std::endl;
            }

        } else {
            cout << "Lỗi không thể truy cập cơ sở dữ liệu." << endl;
        }

        // Sắp xếp lại Timeslots trong mỗi ngày theo giờ tăng dần
        for (auto &entry : timeslots) {
            sort(entry.second.begin(), entry.second.end(),
                 [](const Meeting &a, const Meeting &b) { return a.getStart() < b.getStart(); });
        }

        return timeslots;
    }

    string getWeekName(const string &date) {
        struct tm tm = {};
        strptime(date.c_str(), "%Y-%m-%d", &tm); // Chuyển chuỗi ngày sang `tm`
        time_t time = mktime(&tm);               // Chuyển sang `time_t`

        // Lấy ngày đầu tuần
        int dayOfMonth = tm.tm_mday;
        int firstDayOfMonth = dayOfMonth - (tm.tm_wday == 0 ? 6 : (tm.tm_wday - 1));
        tm.tm_mday = firstDayOfMonth;
        mktime(&tm); // Ngày đầu tuần đầu tiên trong tháng

        // Tính số thứ tự tuần
        int weekNumber = 1 + (dayOfMonth - firstDayOfMonth) / 7;

        // Lấy tháng và ngày đầu/ cuối tuần
        char bufferStart[11], bufferEnd[11];
        strftime(bufferStart, sizeof(bufferStart), "%d/%m", &tm); // Ngày đầu tuần
        tm.tm_mday += 6;
        mktime(&tm); // Ngày cuối tuần
        strftime(bufferEnd, sizeof(bufferEnd), "%d/%m", &tm);

        // Trả về tên tuần
        char result[50];
        snprintf(result, sizeof(result), "Tuần cua tháng %d (%s - %s)", tm.tm_mon + 1, bufferStart, bufferEnd);
        return string(result);
    }

    // Lay lich hen chua hen theo tuan
    map<string, map<string, vector<Meeting>>> getWaitingMeetingsInWeeksByTeacherId(const int &teacher_id) {
        map<string, map<string, vector<Meeting>>> meetings;

        if (db.connect()) {
            string query = R"(
                SELECT meetings.*
                FROM meetings
                INNER JOIN timeslots ON meetings.timeslot_id = timeslots.id
                INNER JOIN users ON timeslots.teacher_id = users.id
                WHERE users.id = ? 
                AND (meetings.status = 'pending' OR meetings.status = 'confirmed' OR meetings.status = 'doing')
            )";
            try {
                sql::PreparedStatement *pstmt = db.getConnection()->prepareStatement(query);
                pstmt->setInt(1, teacher_id);
                sql::ResultSet *res = pstmt->executeQuery();

                while (res->next()) {
                    Meeting meeting;
                    Timeslot timeslot = timeslotRepo.getTimeslotById(res->getInt("timeslot_id"));
                    meeting.setId(res->getInt("id"));
                    meeting.setTeacherId(timeslot.getTeacherId());
                    meeting.setStatus(res->getString("status"));
                    meeting.setType(res->getString("type"));
                    meeting.setReport(res->getString("report"));
                    meeting.setStart(timeslot.getStart());
                    meeting.setEnd(timeslot.getEnd());
                    meeting.setDate(timeslot.getDate());
                    meeting.setTimeslotId(res->getInt("timeslot_id"));
                    string date = meeting.getDate();
                    string week = getWeekName(date);
                    meetings[week][date].push_back(meeting);
                }
                delete res;
                delete pstmt;
            } catch (sql::SQLException &e) {
                std::cerr << "Lỗi khi lấy dữ liệu từ meetings: " << e.what() << std::endl;
            }

        } else {
            cout << "Lỗi không thể truy cập cơ sở dữ liệu." << endl;
        }

        // Sắp xếp lại meetings trong mỗi ngày theo giờ tăng dần
        for (auto &weekEntry : meetings) {
            for (auto &dayEntry : weekEntry.second) {
                sort(dayEntry.second.begin(), dayEntry.second.end(),
                     [](const Meeting &a, const Meeting &b) { return a.getStart() < b.getStart(); });
            }
        }

        return meetings;
    }

    map<string, map<string, vector<Meeting>>> getDoneMeetingsByTeacherIdAndStudentId(const int &teacher_id,
                                                                                     const int &student_id) {
        map<string, map<string, vector<Meeting>>> meetings;

        if (db.connect()) {
            // string query = "SELECT * FROM meetings WHERE teacher_id = ? AND status = 'completed' ";
            string query = R"(
                SELECT meetings.*
                FROM meetings
                INNER JOIN timeslots ON meetings.timeslot_id = timeslots.id
                INNER JOIN attendances ON attendances.meeting_id = meetings.id
                WHERE timeslots.teacher_id = ? 
                AND attendances.student_id = ?
                AND meetings.status = 'completed'
            )";
            try {
                sql::PreparedStatement *pstmt = db.getConnection()->prepareStatement(query);
                pstmt->setInt(1, teacher_id);
                pstmt->setInt(2, student_id);
                sql::ResultSet *res = pstmt->executeQuery();

                while (res->next()) {
                    Meeting meeting;
                    Timeslot timeslot = timeslotRepo.getTimeslotById(res->getInt("timeslot_id"));
                    meeting.setId(res->getInt("id"));
                    meeting.setTeacherId(timeslot.getTeacherId());
                    meeting.setStatus(res->getString("status"));
                    meeting.setType(res->getString("type"));
                    meeting.setReport(res->getString("report"));
                    meeting.setStart(timeslot.getStart());
                    meeting.setEnd(timeslot.getEnd());
                    meeting.setDate(timeslot.getDate());
                    meeting.setTimeslotId(res->getInt("timeslot_id"));
                    string date = meeting.getDate();
                    string week = getWeekName(date);
                    meetings[week][date].push_back(meeting);
                }
                delete res;
                delete pstmt;
            } catch (sql::SQLException &e) {
                std::cerr << "Lỗi khi lấy dữ liệu từ meetings: " << e.what() << std::endl;
            }

        } else {
            cout << "Lỗi không thể truy cập cơ sở dữ liệu." << endl;
        }

        // Sắp xếp lại meetings trong mỗi ngày theo giờ tăng dần
        for (auto &weekEntry : meetings) {
            for (auto &dayEntry : weekEntry.second) {
                sort(dayEntry.second.begin(), dayEntry.second.end(),
                     [](const Meeting &a, const Meeting &b) { return a.getStart() < b.getStart(); });
            }
        }

        return meetings;
    }

    Meeting getMeetingById(const int &id) {
        Meeting meeting;
        if (db.connect()) {
            string query = "SELECT * FROM meetings WHERE id  = ?";
            try {
                sql::PreparedStatement *pstmt = db.getConnection()->prepareStatement(query);
                pstmt->setInt(1, id);
                sql::ResultSet *res = pstmt->executeQuery();

                if (res->next()) {
                    Timeslot timeslot = timeslotRepo.getTimeslotById(res->getInt("timeslot_id"));
                    meeting.setId(res->getInt("id"));
                    meeting.setTeacherId(timeslot.getTeacherId());
                    meeting.setStatus(res->getString("status"));
                    meeting.setType(res->getString("type"));
                    meeting.setReport(res->getString("report"));
                    meeting.setStart(timeslot.getStart());
                    meeting.setEnd(timeslot.getEnd());
                    meeting.setDate(timeslot.getDate());
                    meeting.setTimeslotId(res->getInt("timeslot_id"));
                }

                delete res;
                delete pstmt;
            } catch (sql::SQLException &e) {
                std::cerr << "Lỗi khi lấy dữ liệu từ timeslots: " << e.what() << std::endl;
            }

        } else {
            cout << "Lỗi không thể truy cập cơ sở dữ liệu." << endl;
        }
        return meeting;
    }

    void deleteMeeting(const int &id) {
        if (db.connect()) {
            string query = "DELETE FROM meetings WHERE id = ?";
            try {
                sql::PreparedStatement *pstmt = db.getConnection()->prepareStatement(query);
                pstmt->setInt(1, id);
                sql::ResultSet *res = pstmt->executeQuery();
            } catch (sql::SQLException &e) {
                std::cerr << "Lỗi khi xoa meeting: " << e.what() << std::endl;
            }

        } else {
            cout << "Lỗi không thể truy cập cơ sở dữ liệu." << endl;
        }
    }

    void updateReport(const int &id, const string &report) {
        if (db.connect()) {
            string query = "UPDATE meetings SET report = ? WHERE id = ?";
            try {
                sql::PreparedStatement *pstmt = db.getConnection()->prepareStatement(query);
                pstmt->setString(1, report);
                pstmt->setInt(2, id);

                int rowsUpdated = pstmt->executeUpdate();
                if (rowsUpdated > 0) {
                    cout << "Cập nhật thành công meeting với id: " << id << endl;
                } else {
                    cout << "Không tìm thấy meeting với id: " << id << endl;
                }
                delete pstmt;
            } catch (sql::SQLException &e) {
                cerr << "Lỗi khi cập nhật meeting: " << e.what() << endl;
            }

        } else {
            cout << "Lỗi không thể truy cập cơ sở dữ liệu." << endl;
        }
    }

    void updateStatus(const int &id, const string &status) {
        if (db.connect()) {
            string query = "UPDATE meetings SET status = ? WHERE id = ?";
            try {
                sql::PreparedStatement *pstmt = db.getConnection()->prepareStatement(query);
                pstmt->setString(1, status);
                pstmt->setInt(2, id);

                int rowsUpdated = pstmt->executeUpdate();
                if (rowsUpdated > 0) {
                    cout << "Cập nhật thành công meeting với id: " << id << endl;
                } else {
                    cout << "Không tìm thấy meeting với id: " << id << endl;
                }
                delete pstmt;
            } catch (sql::SQLException &e) {
                cerr << "Lỗi khi cập nhật meeting: " << e.what() << endl;
            }

        } else {
            cout << "Lỗi không thể truy cập cơ sở dữ liệu." << endl;
        }
    }

    map<string, map<string, vector<Meeting>>> getMeetingsInWeeksByStudentId(const int &student_id) {
        map<string, map<string, vector<Meeting>>> meetings;

        if (db.connect()) {
            string query = R"(
                SELECT meetings.*
                FROM meetings
                INNER JOIN attendances ON attendances.meeting_id = meetings.id
                INNER JOIN users ON attendances.student_id = users.id
                WHERE users.id = ? 
                AND (meetings.status = 'pending' OR meetings.status = 'confirmed' OR meetings.status = 'doing' OR meetings.status = 'completed')
            )";
            try {
                sql::PreparedStatement *pstmt = db.getConnection()->prepareStatement(query);
                pstmt->setInt(1, student_id);
                sql::ResultSet *res = pstmt->executeQuery();

                while (res->next()) {
                    Meeting meeting;
                    Timeslot timeslot = timeslotRepo.getTimeslotById(res->getInt("timeslot_id"));
                    meeting.setId(res->getInt("id"));
                    meeting.setTeacherId(timeslot.getTeacherId());
                    meeting.setStatus(res->getString("status"));
                    meeting.setType(res->getString("type"));
                    meeting.setReport(res->getString("report"));
                    meeting.setStart(timeslot.getStart());
                    meeting.setEnd(timeslot.getEnd());
                    meeting.setDate(timeslot.getDate());
                    meeting.setTimeslotId(res->getInt("timeslot_id"));
                    string date = meeting.getDate();
                    string week = getWeekName(date);
                    meetings[week][date].push_back(meeting);
                }
                delete res;
                delete pstmt;
            } catch (sql::SQLException &e) {
                std::cerr << "Lỗi khi lấy dữ liệu từ meetings: " << e.what() << std::endl;
            }

        } else {
            cout << "Lỗi không thể truy cập cơ sở dữ liệu." << endl;
        }

        // Sắp xếp lại meetings trong mỗi ngày theo giờ tăng dần
        for (auto &weekEntry : meetings) {
            for (auto &dayEntry : weekEntry.second) {
                sort(dayEntry.second.begin(), dayEntry.second.end(),
                     [](const Meeting &a, const Meeting &b) { return a.getStart() < b.getStart(); });
            }
        }

        return meetings;
    }

    vector<Meeting> getMeetingsInDateByStudentId(const int &student_id, const string &date) {
        vector<Meeting> meetings;

        if (db.connect()) {
            string query = R"(
                SELECT meetings.*
                FROM meetings
                INNER JOIN attendances ON attendances.meeting_id = meetings.id
                INNER JOIN timeslots ON meetings.timeslot_id = timeslots.id
                WHERE attendances.student_id = ? 
                AND timeslots.date = ?
            )";
            try {
                sql::PreparedStatement *pstmt = db.getConnection()->prepareStatement(query);
                pstmt->setInt(1, student_id);
                pstmt->setString(2, date);
                sql::ResultSet *res = pstmt->executeQuery();

                while (res->next()) {
                    Meeting meeting;
                    Timeslot timeslot = timeslotRepo.getTimeslotById(res->getInt("timeslot_id"));
                    meeting.setId(res->getInt("id"));
                    meeting.setTeacherId(timeslot.getTeacherId());
                    meeting.setStatus(res->getString("status"));
                    meeting.setType(res->getString("type"));
                    meeting.setReport(res->getString("report"));
                    meeting.setStart(timeslot.getStart());
                    meeting.setEnd(timeslot.getEnd());
                    meeting.setDate(timeslot.getDate());
                    meeting.setTimeslotId(res->getInt("timeslot_id"));
                    meetings.push_back(meeting);
                }
                delete res;
                delete pstmt;
            } catch (sql::SQLException &e) {
                std::cerr << "Lỗi khi lấy dữ liệu từ meetings: " << e.what() << std::endl;
            }

        } else {
            cout << "Lỗi không thể truy cập cơ sở dữ liệu." << endl;
        }

        // Sắp xếp lại meetings trong ngày theo giờ tăng dần
        sort(meetings.begin(), meetings.end(),
             [](const Meeting &a, const Meeting &b) { return a.getStart() < b.getStart(); });

        return meetings;
    }
};

#endif

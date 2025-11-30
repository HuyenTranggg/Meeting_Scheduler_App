#ifndef ATTENDANCE_H
#define ATTENDANCE_H
#include <iostream>
using namespace std;

class Attendance {
  protected:
    int id = 0;
    int meeting_id;
    int student_id;

  public:
    // Constructor
    Attendance() {};
    Attendance(const int &nmeeting_id, const int &nstudent_id) : meeting_id(nmeeting_id), student_id(nstudent_id) {}

    // Getter
    int getId() const { return id; }
    int getMeetingId() const { return meeting_id; }
    int getStudentId() const { return student_id; }

    // Setter
    void setId(const int &newId) { id = newId; }
    void setMeetingId(const int &newMeetingId) { meeting_id = newMeetingId; }
    void setStudentId(const int &newStudentId) { student_id = newStudentId; }

    void show() {
        cout << "Attendance, Id: " << id << " ,meeting_id: " << meeting_id << " ,student_id: " << student_id << endl;
    }
};

#endif

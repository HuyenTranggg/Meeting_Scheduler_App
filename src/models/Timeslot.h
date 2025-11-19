#ifndef TIMESLOT_H
#define TIMESLOT_H

#include <iostream>
using namespace std;

class Timeslot {
  private:
    int id = 0;
    string start;
    string end;
    string date;
    string type;
    string status;
    int teacher_id;

  public:
    // Constructor
    Timeslot() {};
    Timeslot(const string &nstart, const string &nend, const string &ndate, const string &ntype, const int &nteacher_id)
        : start(nstart), end(nend), date(ndate), type(ntype), teacher_id(nteacher_id) {}
    Timeslot(const string &nstart, const string &nend, const string &ndate, const string &ntype, const string &nstatus,
             const int &nteacher_id)
        : start(nstart), end(nend), date(ndate), type(ntype), status(nstatus), teacher_id(nteacher_id) {}

    // Getter
    int getId() const { return id; }
    string getStart() const { return start; }
    string getEnd() const { return end; }
    string getDate() const { return date; }
    string getType() const { return type; }
    string getStatus() const { return status; }
    int getTeacherId() const { return teacher_id; }

    // Setter
    void setId(const int &newId) { id = newId; }
    void setStart(const string &newStart) { start = newStart; }
    void setEnd(const string &newEnd) { end = newEnd; }
    void setDate(const string &newDate) { date = newDate; }
    void setType(const string &newType) { type = newType; }
    void setStatus(const string &newStatus) { status = newStatus; }
    void setTeacherId(const int &newTeacherId) { teacher_id = newTeacherId; }

    string toString() {
        return to_string(id) + "|" + start + "|" + end + "|" + date + "|" + type + "|" + status + "|" +
               to_string(teacher_id);
    }

    string toStringUpdate() { return to_string(id) + "|" + start + "|" + end + "|" + type; }

    string toStringDeclare() { return start + "|" + end + "|" + date + "|" + type + "|" + to_string(teacher_id); }

    string toStringBookMeeting() { return start + "|" + end + "|" + date + "|" + type + "|" + to_string(teacher_id) + "|" + to_string(id); }

    void show() {
        cout << "TimeSlot, Id: " << id << " ,start: " << start << " ,end: " << end << " ,date: " << date
             << " ,teacher_id: " << teacher_id << endl;
    }
};

#endif

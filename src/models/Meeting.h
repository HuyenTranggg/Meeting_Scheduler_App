#ifndef MEETING_H
#define MEETING_H
#include <iostream>
using namespace std;

class Meeting {
  protected:
    int id = 0;
    int teacher_id;
    string status;
    string type;
    string report;
    string start;
    string end;
    string date;
    int timeslot_id;

  public:
    // Constructor
    Meeting() {};
    Meeting(const int &nteacher_id, const string &nstatus, const string &ntype, const string &nstart,
            const string &nend, const string &ndate)
        : teacher_id(nteacher_id), status(nstatus), type(ntype), start(nstart), end(nend), date(ndate) {}
    Meeting(const int &nteacher_id, const string &nstatus, const string &ntype, const string &nreport,
            const string &nstart, const string &nend, const string &ndate)
        : teacher_id(nteacher_id), status(nstatus), type(ntype), report(nreport), start(nstart), end(nend),
          date(ndate) {}
    Meeting(const int &nteacher_id, const string &nstatus, const string &ntype, const string &nreport,
            const string &nstart, const string &nend, const string &ndate, const int &ntimeslot_id)
        : teacher_id(nteacher_id), status(nstatus), type(ntype), report(nreport), start(nstart), end(nend), date(ndate),
          timeslot_id(ntimeslot_id) {}
    Meeting(const string &ntype, const int &ntimeslot_id) : type(ntype), timeslot_id(ntimeslot_id) {}

    // Getter
    int getId() const { return id; }
    int getTeacherId() const { return teacher_id; }
    int getTimeslotId() const { return timeslot_id; }
    string getStatus() const { return status; }
    string getType() const { return type; }
    string getReport() const { return report; }
    string getStart() const { return start; }
    string getEnd() const { return end; }
    string getDate() const { return date; }

    // Setter
    void setId(const int &newId) { id = newId; }
    void setTeacherId(const int &newTeacherId) { teacher_id = newTeacherId; }
    void setTimeslotId(const int &newTimeslotId) { timeslot_id = newTimeslotId; }
    void setStatus(const string &newStatus) { status = newStatus; }
    void setType(const string &newType) { type = newType; }
    void setReport(const string &newReport) { report = newReport; }
    void setStart(const string &newStart) { start = newStart; }
    void setEnd(const string &newEnd) { end = newEnd; }
    void setDate(const string &newDate) { date = newDate; }

    string toString() {
        return to_string(id) + "|" + to_string(timeslot_id) + "|" + status + "|" + type + "|" + report + "|" + start +
               "|" + end + "|" + date;
    }

    void show() {
        cout << "Meeting, Id: " << id << " ,teacher_id: " << teacher_id << " ,status: " << status << " ,type: " << type
             << " ,start: " << start << " ,end: " << end << " ,date: " << date << endl;
    }
};

#endif

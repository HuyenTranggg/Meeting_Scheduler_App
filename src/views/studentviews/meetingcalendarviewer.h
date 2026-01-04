#ifndef MEETINGCALENDARVIEWER_H
#define MEETINGCALENDARVIEWER_H

#include <QDialog>
#include <QCalendarWidget>
#include <QListWidget>
#include <map>
#include <vector>
#include "../../models/Meeting.h"
#include "../../models/User.h"

namespace Ui {
class MeetingCalendarViewer;
}

class MeetingCalendarViewer : public QDialog {
    Q_OBJECT

public:
    explicit MeetingCalendarViewer(QWidget *parent = nullptr);
    ~MeetingCalendarViewer();
    Meeting showMeetingsInWeeks(const std::map<std::string, std::map<std::string, 
                              std::vector<std::pair<Meeting, User>>>>& meetings);

private slots:
    void on_calendarWidget_clicked(const QDate &date);
    void on_listWidget_doubleClicked(const QModelIndex &index);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_calendarWidget_selectionChanged();

private:
    Ui::MeetingCalendarViewer *ui;
    void highlightDatesWithMeetings();
    void updateMeetingList(const QString& dateStr);
    std::map<std::string, std::map<std::string, std::vector<std::pair<Meeting, User>>>> allMeetings;
    std::map<int, Meeting> currentMeetings;
    QString selectedDate;
    Meeting selectedMeeting;
    void setupCalendarFormat();
    QString getWeekNumber(const QDate& date);
};

#endif // MEETINGCALENDARVIEWER_H
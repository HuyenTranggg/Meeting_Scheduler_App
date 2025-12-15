// timeslotcalendar.h
#ifndef TIMESLOTCALENDAR_H
#define TIMESLOTCALENDAR_H

#include <QDialog>
#include <QCalendarWidget>
#include <QListWidget>
#include <map>
#include <vector>
#include "../../models/Timeslot.h"

namespace Ui {
class TimeslotCalendar;
}

class TimeslotCalendar : public QDialog {
    Q_OBJECT

public:
    explicit TimeslotCalendar(QWidget *parent = nullptr);
    ~TimeslotCalendar();
    Timeslot showTimeslots(const std::map<std::string, std::vector<Timeslot>>& timeslots, 
                          const std::string& teacherName);

private slots:
    void on_calendarWidget_clicked(const QDate &date);
    void on_listWidget_doubleClicked(const QModelIndex &index);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::TimeslotCalendar *ui;
    void highlightDatesWithTimeslots();
    void updateTimeslotList(const QString& dateStr);
    std::map<std::string, std::vector<Timeslot>> allTimeslots;
    std::map<int, Timeslot> currentTimeslots;
    QString selectedDate;
    Timeslot selectedTimeslot;
    void setupCalendarFormat();
};

#endif // TIMESLOTCALENDAR_H
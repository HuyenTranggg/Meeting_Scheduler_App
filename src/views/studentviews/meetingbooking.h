#ifndef MEETINGBOOKING_H
#define MEETINGBOOKING_H

#include <QDialog>
#include <QButtonGroup>
#include "../../models/Meeting.h"
#include "../../models/Timeslot.h"

namespace Ui {
class MeetingBooking;
}

class MeetingBooking : public QDialog {
    Q_OBJECT

public:
    explicit MeetingBooking(QWidget *parent = nullptr);
    ~MeetingBooking();
    Meeting showBookMeeting(const Timeslot& timeslot, const std::string& teacherName);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void onTypeSelectionChanged();

private:
    Ui::MeetingBooking *ui;
    QButtonGroup* typeButtonGroup;
    Meeting meeting;
    Timeslot currentTimeslot;
    void updateMeetingType();
};

#endif // MEETINGBOOKING_H
#ifndef MEETINGDIALOG_H
#define MEETINGDIALOG_H

#include <QDialog>
#include <QString>
#include "../../models/Meeting.h"
#include "../../models/User.h"

namespace Ui {
class MeetingDialog;
}

class MeetingDialog : public QDialog {
    Q_OBJECT

public:
    explicit MeetingDialog(QWidget *parent = nullptr);
    ~MeetingDialog();
    int showMeeting(const Meeting &meeting, const User &teacher);

private slots:
    void on_cancelButton_clicked();
    void on_backButton_clicked();

private:
    Ui::MeetingDialog *ui;
    int choice = 0;
};

#endif // MEETINGDIALOG_H
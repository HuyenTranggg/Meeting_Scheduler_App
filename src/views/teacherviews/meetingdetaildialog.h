// meetingdetaildialog.h
#ifndef MEETINGDETAILDIALOG_H
#define MEETINGDETAILDIALOG_H

#include <QDialog>
#include <QDateTime>
#include "../models/Meeting.h"
#include "../models/User.h"
#include <vector>

namespace Ui {
class MeetingDetailDialog;
}

class MeetingDetailDialog : public QDialog {
    Q_OBJECT

public:
    explicit MeetingDetailDialog(const Meeting& meeting, const std::vector<User>& students, QWidget *parent = nullptr);
    ~MeetingDetailDialog();
    int getResult() const;
    QString getReport() const;
    QString getStatus() const;

private slots:
    void onEditReport();
    void onEditStatus();
    void onBack();

private:
    Ui::MeetingDetailDialog *ui;
    Meeting meeting;
    std::vector<User> students;
    int result;
    QString currentReport;
    QString currentStatus;
    
    void setupUi();
    void updateMeetingInfo();
};

#endif // MEETINGDETAILDIALOG_H
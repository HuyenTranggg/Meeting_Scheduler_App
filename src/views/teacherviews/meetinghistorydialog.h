#ifndef MEETINGHISTORYDIALOG_H
#define MEETINGHISTORYDIALOG_H

#include <QDialog>
#include <vector>
#include "../../models/Meeting.h"
#include "../../models/User.h"

namespace Ui {
class MeetingHistoryDialog;
}

class MeetingHistoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MeetingHistoryDialog(const Meeting& meeting, const std::vector<User>& students, QWidget *parent = nullptr);
    ~MeetingHistoryDialog();
    QString getReport() const;

private slots:
    void on_editButton_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::MeetingHistoryDialog *ui;
    Meeting meeting;
    QString originalReport;
};

#endif // MEETINGHISTORYDIALOG_H
#ifndef TEACHERVIEWMEETINGSDIALOG_H
#define TEACHERVIEWMEETINGSDIALOG_H

#include <QDialog>
#include <QDate>
#include <QMap>
#include <QVector>
#include <QListWidgetItem>
#include "../../models/Meeting.h"

namespace Ui {
class TeacherViewMeetingsDialog;
}

class TeacherViewMeetingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TeacherViewMeetingsDialog(const QMap<QString, QVector<Meeting>>& meetings, QWidget *parent = nullptr);
    ~TeacherViewMeetingsDialog();
    Meeting getSelectedMeeting() const;

private slots:
    void on_calendarWidget_clicked(const QDate &date);
    void on_meetingListWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::TeacherViewMeetingsDialog *ui;
    QMap<QString, QVector<Meeting>> meetings;
    Meeting selectedMeeting;
    void updateMeetingList(const QDate &date);
};

#endif 
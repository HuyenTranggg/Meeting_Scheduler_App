// viewweeklymeetingsdialog.h
#ifndef VIEWWEEKLYMEETINGSDIALOG_H
#define VIEWWEEKLYMEETINGSDIALOG_H

#include <QDialog>
#include <QCalendarWidget>
#include <QTreeWidget>
#include <map>
#include <vector>
#include <utility>
#include "../../models/Meeting.h"
#include "../../models/User.h"

namespace Ui {
class ViewWeeklyMeetingsDialog;
}

class ViewWeeklyMeetingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit ViewWeeklyMeetingsDialog(
        const std::map<std::string, std::map<std::string, std::vector<std::pair<Meeting, std::vector<User>>>>>& meetings,
        QWidget *parent = nullptr);
    ~ViewWeeklyMeetingsDialog();
    Meeting getSelectedMeeting() const { return selectedMeeting; }

private slots:
    void dateSelected(const QDate& date);
    void meetingItemDoubleClicked(QTreeWidgetItem* item, int column);
    void accept() override;
    void reject() override;

private:
    Ui::ViewWeeklyMeetingsDialog *ui;
    void setupCalendar();
    void highlightDatesWithMeetings();
    void populateTree(const QString& dateStr);
    QString formatUserList(const std::vector<User>& users);

    std::map<std::string, std::map<std::string, std::vector<std::pair<Meeting, std::vector<User>>>>> allMeetings;
    std::map<int, Meeting> editMeetings;
    Meeting selectedMeeting;
    int currentIndex;
};

#endif // VIEWWEEKLYMEETINGSDIALOG_H
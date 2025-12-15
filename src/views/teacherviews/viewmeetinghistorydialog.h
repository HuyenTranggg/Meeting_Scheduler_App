#ifndef VIEWMEETINGHISTORYDIALOG_H
#define VIEWMEETINGHISTORYDIALOG_H

#include <QDialog>
#include <QCalendarWidget>
#include <QListWidget>
#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QAbstractButton>
#include <QTextCharFormat>
#include <map>
#include <vector>
#include "../../models/Meeting.h"

namespace Ui {
class ViewMeetingHistoryDialog;
}

class ViewMeetingHistoryDialog : public QDialog {
    Q_OBJECT

public:
    explicit ViewMeetingHistoryDialog(QWidget *parent = nullptr);
    ~ViewMeetingHistoryDialog();
    Meeting showHistory(const std::map<std::string, std::vector<Meeting>> &meetings);

private slots:
    void onCalendarDateChanged();
    void onMeetingDoubleClicked(QListWidgetItem* item);
    void onButtonClicked(QAbstractButton* button);

private:
    Ui::ViewMeetingHistoryDialog *ui;
    std::map<std::string, std::vector<Meeting>> meetingsData;
    std::map<int, Meeting> meetingIndex;
    void updateMeetingList(const QString& selectedDate);
    Meeting selectedMeeting;
};

#endif // VIEWMEETINGHISTORYDIALOG_H
#include "meetingcalendarviewer.h"
#include "ui_meetingcalendarviewer.h"
#include <QTextCharFormat>
#include <QMessageBox>

MeetingCalendarViewer::MeetingCalendarViewer(QWidget *parent)
    : QDialog(parent), ui(new Ui::MeetingCalendarViewer) {
    ui->setupUi(this);
    setupCalendarFormat();
}

MeetingCalendarViewer::~MeetingCalendarViewer() {
    delete ui;
}

void MeetingCalendarViewer::setupCalendarFormat() {
    ui->calendarWidget->setGridVisible(true);
    ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::ISOWeekNumbers);
    ui->calendarWidget->setFirstDayOfWeek(Qt::Monday);
}

QString MeetingCalendarViewer::getWeekNumber(const QDate& date) {
    return QString("Tuần %1").arg(date.weekNumber());
}

Meeting MeetingCalendarViewer::showMeetingsInWeeks(
    const std::map<std::string, std::map<std::string, std::vector<std::pair<Meeting, User>>>>& meetings) {
    
    allMeetings = meetings;
    setWindowTitle("Lịch hẹn của bạn");

    if (meetings.empty()) {
        QMessageBox::information(this, "Thông báo", "Bạn chưa có cuộc hẹn nào");
        Meeting meeting;
        meeting.setId(-1);
        return meeting;
    }

    highlightDatesWithMeetings();
    
    // Update the meeting list for the current selected date
    updateMeetingList(ui->calendarWidget->selectedDate().toString("yyyy-MM-dd"));
    
    if (exec() == QDialog::Accepted && selectedMeeting.getId() != -1) {
        return selectedMeeting;
    }
    
    Meeting meeting;
    meeting.setId(-1);
    return meeting;
}

void MeetingCalendarViewer::highlightDatesWithMeetings() {
    QTextCharFormat highlightFormat;
    highlightFormat.setBackground(Qt::cyan);
    
    for (const auto& [week, dailyMeetings] : allMeetings) {
        for (const auto& [dateStr, meetings] : dailyMeetings) {
            QDate date = QDate::fromString(QString::fromStdString(dateStr), "yyyy-MM-dd");
            if (date.isValid() && !meetings.empty()) {
                ui->calendarWidget->setDateTextFormat(date, highlightFormat);
            }
        }
    }
}

void MeetingCalendarViewer::updateMeetingList(const QString& dateStr) {
    ui->listWidget->clear();
    currentMeetings.clear();
    
    ui->weekLabel->setText(getWeekNumber(QDate::fromString(dateStr, "yyyy-MM-dd")));
    
    std::string dateStdStr = dateStr.toStdString();
    for (const auto& [week, dailyMeetings] : allMeetings) {
        if (dailyMeetings.find(dateStdStr) != dailyMeetings.end()) {
            const auto& meetings = dailyMeetings.at(dateStdStr);
            int index = 1;
            for (const auto& [meeting, teacher] : meetings) {
                QString displayText = QString("%1 - %2 (%3 - %4)\nGiáo viên: %5 %6")
                    .arg(QString::fromStdString(meeting.getStart()))
                    .arg(QString::fromStdString(meeting.getEnd()))
                    .arg(QString::fromStdString(meeting.getType()))
                    .arg(QString::fromStdString(meeting.getStatus()))
                    .arg(QString::fromStdString(teacher.getFirstName()))
                    .arg(QString::fromStdString(teacher.getLastName()));
                    
                QListWidgetItem* item = new QListWidgetItem(displayText);
                item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
                ui->listWidget->addItem(item);
                currentMeetings[index++] = meeting;
            }
            break;
        }
    }
}

void MeetingCalendarViewer::on_calendarWidget_clicked(const QDate &date) {
    selectedDate = date.toString("yyyy-MM-dd");
    updateMeetingList(selectedDate);
}

void MeetingCalendarViewer::on_calendarWidget_selectionChanged() {
    QDate date = ui->calendarWidget->selectedDate();
    selectedDate = date.toString("yyyy-MM-dd");
    updateMeetingList(selectedDate);
}

void MeetingCalendarViewer::on_listWidget_doubleClicked(const QModelIndex &index) {
    int row = index.row() + 1;
    if (currentMeetings.find(row) != currentMeetings.end()) {
        selectedMeeting = currentMeetings[row];
        accept();
    }
}

void MeetingCalendarViewer::on_buttonBox_accepted() {
    int currentRow = ui->listWidget->currentRow();
    if (currentRow == -1) {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn một cuộc hẹn!");
        return;
    }
    selectedMeeting = currentMeetings[currentRow + 1];
    accept();
}

void MeetingCalendarViewer::on_buttonBox_rejected() {
    Meeting meeting;
    meeting.setId(-1);
    selectedMeeting = meeting;
    reject();
}

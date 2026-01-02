// viewweeklymeetingsdialog.cpp
#include "viewweeklymeetingsdialog.h"
#include "ui_viewweeklymeetingsdialog.h"
#include "../../utils/PopupUtils.h"
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QPushButton>
#include <QTextCharFormat>

ViewWeeklyMeetingsDialog::ViewWeeklyMeetingsDialog(
    const std::map<std::string, std::map<std::string, std::vector<std::pair<Meeting, std::vector<User>>>>>& meetings,
    QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewWeeklyMeetingsDialog),
    allMeetings(meetings),
    currentIndex(0)
{
    ui->setupUi(this);
    setupCalendar();
    highlightDatesWithMeetings();

    // Đặt text cho các nút
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Xem chi tiết");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Đóng");

    // Kết nối signal cho double click trên tree item
    connect(ui->meetingsTree, &QTreeWidget::itemDoubleClicked,
            this, &ViewWeeklyMeetingsDialog::meetingItemDoubleClicked);
}

ViewWeeklyMeetingsDialog::~ViewWeeklyMeetingsDialog()
{
    delete ui;
}

void ViewWeeklyMeetingsDialog::setupCalendar()
{
    // Thiết lập ngày tối thiểu là ngày hiện tại
    ui->calendarWidget->setMinimumDate(QDate::currentDate());
    
    // Kết nối sự kiện chọn ngày
    connect(ui->calendarWidget, &QCalendarWidget::clicked,
            this, &ViewWeeklyMeetingsDialog::dateSelected);

    // Thiết lập định dạng hiển thị tiếng Việt cho lịch
    QLocale vietnameseLocale(QLocale::Vietnamese);
    ui->calendarWidget->setLocale(vietnameseLocale);
}

void ViewWeeklyMeetingsDialog::highlightDatesWithMeetings()
{
    QTextCharFormat highlightFormat;
    highlightFormat.setBackground(Qt::lightGray);

    for (const auto& week : allMeetings) {
        for (const auto& day : week.second) {
            QDate date = QDate::fromString(QString::fromStdString(day.first), "yyyy-MM-dd");
            if (date.isValid() && !day.second.empty()) {
                ui->calendarWidget->setDateTextFormat(date, highlightFormat);
            }
        }
    }
}

void ViewWeeklyMeetingsDialog::populateTree(const QString& dateStr)
{
    ui->meetingsTree->clear();
    editMeetings.clear();
    currentIndex = 0;

    // Tìm các cuộc hẹn cho ngày được chọn
    bool found = false;
    for (const auto& week : allMeetings) {
        for (const auto& day : week.second) {
            if (QString::fromStdString(day.first) == dateStr) {
                found = true;
                for (const auto& meetingPair : day.second) {
                    currentIndex++;
                    const Meeting& meeting = meetingPair.first;
                    const std::vector<User>& users = meetingPair.second;
                    
                    QTreeWidgetItem* meetingItem = new QTreeWidgetItem(ui->meetingsTree);
                    QString meetingText = QString("%1. Từ %2 đến %3 (%4 - %5). Người tham gia: %6")
                        .arg(currentIndex)
                        .arg(QString::fromStdString(meeting.getStart()))
                        .arg(QString::fromStdString(meeting.getEnd()))
                        .arg(QString::fromStdString(meeting.getType()))
                        .arg(QString::fromStdString(meeting.getStatus()))
                        .arg(formatUserList(users));
                    
                    meetingItem->setText(0, meetingText);
                    meetingItem->setData(0, Qt::UserRole, currentIndex);
                    editMeetings[currentIndex] = meeting;
                }
                break;
            }
        }
        if (found) break;
    }

    if (!found || editMeetings.empty()) {
        PopupUtils::showInfo("Thông báo", "Không có cuộc hẹn nào trong ngày này", this);
    }
}

QString ViewWeeklyMeetingsDialog::formatUserList(const std::vector<User>& users)
{
    QStringList userNames;
    for (const User& user : users) {
        userNames << QString("%1 %2").arg(
            QString::fromStdString(user.getFirstName()),
            QString::fromStdString(user.getLastName())
        );
    }
    return userNames.join(", ");
}

void ViewWeeklyMeetingsDialog::dateSelected(const QDate& date)
{
    QString dateStr = date.toString("yyyy-MM-dd");
    populateTree(dateStr);
}

void ViewWeeklyMeetingsDialog::meetingItemDoubleClicked(QTreeWidgetItem* item, int column)
{
    bool ok;
    int index = item->data(0, Qt::UserRole).toInt(&ok);
    if (ok && editMeetings.find(index) != editMeetings.end()) {
        selectedMeeting = editMeetings[index];
        accept();
    }
}

void ViewWeeklyMeetingsDialog::accept()
{
    QTreeWidgetItem* selectedItem = ui->meetingsTree->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn một cuộc hẹn");
        return;
    }

    bool ok;
    int index = selectedItem->data(0, Qt::UserRole).toInt(&ok);
    if (ok && editMeetings.find(index) != editMeetings.end()) {
        selectedMeeting = editMeetings[index];
        QDialog::accept();
    }
}

void ViewWeeklyMeetingsDialog::reject()
{
    selectedMeeting.setId(-1);
    QDialog::reject();
}

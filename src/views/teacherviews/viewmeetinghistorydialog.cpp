#include "viewmeetinghistorydialog.h"
#include "ui_viewmeetinghistorydialog.h"
#include <QMessageBox>
#include <QDate>
ViewMeetingHistoryDialog::ViewMeetingHistoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewMeetingHistoryDialog)
{
    ui->setupUi(this);
    
    connect(ui->calendarWidget, &QCalendarWidget::selectionChanged,
            this, &ViewMeetingHistoryDialog::onCalendarDateChanged);
            
    connect(ui->meetingListWidget, &QListWidget::itemDoubleClicked,
            this, &ViewMeetingHistoryDialog::onMeetingDoubleClicked);
            
    connect(ui->buttonBox, &QDialogButtonBox::clicked,
            this, &ViewMeetingHistoryDialog::onButtonClicked);
            
    selectedMeeting.setId(-1);
}

ViewMeetingHistoryDialog::~ViewMeetingHistoryDialog()
{
    delete ui;
}


void ViewMeetingHistoryDialog::onCalendarDateChanged()
{
    QString selectedDate = ui->calendarWidget->selectedDate().toString("dd/MM/yyyy");
    updateMeetingList(selectedDate);
}

void ViewMeetingHistoryDialog::updateMeetingList(const QString& selectedDate)
{
    ui->meetingListWidget->clear();
    meetingIndex.clear();
    
    std::string dateStr = QDate::fromString(selectedDate, "dd/MM/yyyy").toString("yyyy-MM-dd").toStdString();
    if (meetingsData.find(dateStr) != meetingsData.end()) {
        const auto& meetings = meetingsData[dateStr];
        int index = 0; // Bắt đầu từ 0 để khớp với `row`

        for (const auto& meeting : meetings) {
            QString itemText = QString("Từ: %1 - Đến: %2 ( %3 - %4 )")
                .arg(QString::fromStdString(meeting.getStart()))
                .arg(QString::fromStdString(meeting.getEnd()))
                .arg(QString::fromStdString(meeting.getType()))
                .arg(QString::fromStdString(meeting.getStatus()));
                
            QListWidgetItem* item = new QListWidgetItem(itemText);
            ui->meetingListWidget->addItem(item);
            meetingIndex[index] = meeting; // Gán đúng chỉ số
            index++;
        }
    }
}



Meeting ViewMeetingHistoryDialog::showHistory(const std::map<std::string, std::vector<Meeting>> &meetings)
{
    if (meetings.empty()) {
        QMessageBox::information(this, "Thông báo", "Bạn chưa hoàn thành cuộc hẹn nào");
        return Meeting(); // Trả về một đối tượng `Meeting` mặc định với ID = -1
    }

    meetingsData = meetings;

    ui->calendarWidget->setSelectedDate(QDate::currentDate());
    for (const auto &meeting : meetings) {
        QDate date = QDate::fromString(QString::fromStdString(meeting.first), "yyyy-MM-dd");
        if (date.isValid()) {
            QTextCharFormat format;
            format.setBackground(Qt::green);
            ui->calendarWidget->setDateTextFormat(date, format);
        }
    }
    
    updateMeetingList(QDate::currentDate().toString("dd/MM/yyyy"));
    this->exec(); // Hiển thị dialog và chờ người dùng

    return selectedMeeting; // Trả về cuộc họp được chọn
}

void ViewMeetingHistoryDialog::onMeetingDoubleClicked(QListWidgetItem* item)
{
    int row = ui->meetingListWidget->row(item); // Không cần +1
    if (meetingIndex.find(row) != meetingIndex.end()) {
        selectedMeeting = meetingIndex[row];
        accept(); // Đóng dialog và trả về giá trị
    }
}


void ViewMeetingHistoryDialog::onButtonClicked(QAbstractButton* button)
{
    QDialogButtonBox::StandardButton standardButton = ui->buttonBox->standardButton(button);
    if (standardButton == QDialogButtonBox::Ok) {
        QListWidgetItem* currentItem = ui->meetingListWidget->currentItem();
        if (currentItem) {
            int row = ui->meetingListWidget->row(currentItem); // Không cần +1
            if (meetingIndex.find(row) != meetingIndex.end()) {
                selectedMeeting = meetingIndex[row];
                accept(); // Đóng dialog và trả về
            }
        }
    } else if (standardButton == QDialogButtonBox::Cancel) {
        selectedMeeting.setId(-1); // Đặt lại ID cho giá trị không hợp lệ
        reject(); // Đóng dialog mà không trả về
    }
}


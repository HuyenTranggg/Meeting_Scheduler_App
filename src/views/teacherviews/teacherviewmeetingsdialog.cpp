#include "teacherviewmeetingsdialog.h"
#include "ui_teacherviewmeetingsdialog.h"
#include "../../utils/PopupUtils.h"
#include <QMessageBox>
#include <QTextCharFormat>

TeacherViewMeetingsDialog::TeacherViewMeetingsDialog(const QMap<QString, QVector<Meeting>>& meetings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TeacherViewMeetingsDialog),
    meetings(meetings)
{
    ui->setupUi(this);
    setWindowTitle("Lịch họp của giáo viên");

    // Thiết lập calendar
    ui->calendarWidget->setGridVisible(true);
    ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);

    // Đánh dấu các ngày có họp
    for (auto it = meetings.begin(); it != meetings.end(); ++it) {
        QDate date = QDate::fromString(it.key(), "yyyy-MM-dd");  // Cập nhật format date
        if (date.isValid()) {
            // Đánh dấu ngày có cuộc họp bằng màu xanh nhạt
            QTextCharFormat format;
            format.setBackground(QColor(200, 255, 200));
            ui->calendarWidget->setDateTextFormat(date, format);
        }
    }

    // Hiển thị danh sách cuộc họp của ngày hiện tại
    updateMeetingList(QDate::currentDate());
}

TeacherViewMeetingsDialog::~TeacherViewMeetingsDialog()
{
    delete ui;
}

void TeacherViewMeetingsDialog::updateMeetingList(const QDate &date)
{
    ui->meetingListWidget->clear();
    QString dateStr = date.toString("yyyy-MM-dd");  // Cập nhật format date

    if (meetings.contains(dateStr)) {
        const QVector<Meeting>& dayMeetings = meetings[dateStr];
        for (int i = 0; i < dayMeetings.size(); i++) {
            const Meeting& meeting = dayMeetings[i];
            QString meetingText = QString("-----------------Lịch họp ngày %1-----------------\n")
                                    .arg(dateStr);
            meetingText += QString("%1. Từ: %2 - Đến: %3 ( %4 )")
                                    .arg(i + 1)
                                    .arg(QString::fromStdString(meeting.getStart()))
                                    .arg(QString::fromStdString(meeting.getEnd()))
                                    .arg(QString::fromStdString(meeting.getType()));
                                    // .arg(QString::fromStdString(meeting.getStatus()));

            QListWidgetItem* item = new QListWidgetItem(meetingText);
            
            // Màu nền theo trạng thái cuộc họp
            if (QString::fromStdString(meeting.getStatus()).toLower() == "pending") {
                item->setBackground(QColor(255, 255, 200));  // Màu vàng nhạt cho pending
            } else if (QString::fromStdString(meeting.getStatus()).toLower() == "confirmed") {
                item->setBackground(QColor(255, 182, 193));  // Màu xanh nhạt cho completed
            } else {
                item->setBackground(QColor(144, 238, 144));  // Màu xám cho các trạng thái khác
            }

            ui->meetingListWidget->addItem(item);
        }
    }
}

void TeacherViewMeetingsDialog::on_calendarWidget_clicked(const QDate &date)
{
    updateMeetingList(date);
}

void TeacherViewMeetingsDialog::on_meetingListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    int row = ui->meetingListWidget->row(item);
    QString dateStr = ui->calendarWidget->selectedDate().toString("yyyy-MM-dd");
    if (meetings.contains(dateStr) && row < meetings[dateStr].size()) {
        selectedMeeting = meetings[dateStr][row];
        accept();
    }
}

void TeacherViewMeetingsDialog::on_buttonBox_accepted()
{
    int currentRow = ui->meetingListWidget->currentRow();
    if (currentRow >= 0) {
        QString dateStr = ui->calendarWidget->selectedDate().toString("yyyy-MM-dd");
        if (meetings.contains(dateStr) && currentRow < meetings[dateStr].size()) {
            selectedMeeting = meetings[dateStr][currentRow];
            accept();
        }
    } else {
        PopupUtils::showWarning("Chú ý", "Vui lòng chọn một cuộc họp!", this);
    }
}

void TeacherViewMeetingsDialog::on_buttonBox_rejected()
{
    selectedMeeting.setId(-1);
    reject();
}

Meeting TeacherViewMeetingsDialog::getSelectedMeeting() const
{
    return selectedMeeting;
}

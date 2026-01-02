// meetingdetaildialog.cpp
#include "meetingdetaildialog.h"
#include "ui_meetingdetaildialog.h"
#include <QMessageBox>
#include <QInputDialog>

MeetingDetailDialog::MeetingDetailDialog(const Meeting& meeting, const std::vector<User>& students, QWidget *parent)
    : QDialog(parent), ui(new Ui::MeetingDetailDialog), meeting(meeting), students(students), result(0) {
    setupUi();
    updateMeetingInfo();
}

MeetingDetailDialog::~MeetingDetailDialog() {
    delete ui;
}

void MeetingDetailDialog::setupUi() {
    ui->setupUi(this);
    
    connect(ui->editReportButton, &QPushButton::clicked, this, &MeetingDetailDialog::onEditReport);
    connect(ui->editStatusButton, &QPushButton::clicked, this, &MeetingDetailDialog::onEditStatus);
    connect(ui->backButton, &QPushButton::clicked, this, &MeetingDetailDialog::onBack);
}

void MeetingDetailDialog::updateMeetingInfo() {
    ui->dateLabel->setText(QString::fromStdString(meeting.getDate()));
    ui->startTimeLabel->setText(QString::fromStdString(meeting.getStart()));
    ui->endTimeLabel->setText(QString::fromStdString(meeting.getEnd()));
    ui->typeLabel->setText(QString::fromStdString(meeting.getType()));
    ui->statusLabel->setText(QString::fromStdString(meeting.getStatus()));
    ui->reportTextEdit->setText(QString::fromStdString(meeting.getReport()));
    
    QString studentsText;
    for (const auto& student : students) {
        studentsText += "- " + QString::fromStdString(student.getFirstName() + " " + student.getLastName()) + "\n";
    }
    ui->studentsTextEdit->setPlainText(studentsText);
}

void MeetingDetailDialog::onEditReport() {
    bool ok;
    QString text = QInputDialog::getMultiLineText(this, "Chỉnh sửa văn bản cuộc họp",
                                                "Nhập nội dung:", ui->reportTextEdit->toPlainText(), &ok);
    if (ok && !text.isEmpty()) {
        currentReport = text;
        ui->reportTextEdit->setText(text);
        result = 1;
        accept();
    }
}

void MeetingDetailDialog::onEditStatus() {
    QStringList items;
    items << "pending" << "confirmed" << "doing" << "canceled" << "completed";
    
    bool ok;
    QString item = QInputDialog::getItem(this, "Cập nhật trạng thái",
                                       "Chọn trạng thái:", items, 0, false, &ok);
    if (ok && !item.isEmpty()) {
        currentStatus = item;
        ui->statusLabel->setText(item);
        result = 2;
        accept();
    }
}

void MeetingDetailDialog::onBack() {
    result = 0;
    accept();
}

int MeetingDetailDialog::getResult() const {
    return result;
}

QString MeetingDetailDialog::getReport() const {
    return currentReport;
}

QString MeetingDetailDialog::getStatus() const {
    return currentStatus;
}
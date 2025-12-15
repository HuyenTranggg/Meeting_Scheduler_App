#include "MeetingDialog.h"
#include "ui_MeetingDialog.h"

MeetingDialog::MeetingDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::MeetingDialog) {
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setModal(true);
}

MeetingDialog::~MeetingDialog() {
    delete ui;
}

int MeetingDialog::showMeeting(const Meeting &meeting, const User &teacher) {
    ui->dateLabel->setText(QString::fromStdString(meeting.getDate()));
    ui->startLabel->setText(QString::fromStdString(meeting.getStart()));
    ui->endLabel->setText(QString::fromStdString(meeting.getEnd()));
    ui->typeLabel->setText(QString::fromStdString(meeting.getType()));
    ui->statusLabel->setText(QString::fromStdString(meeting.getStatus()));
    ui->reportBrowser->setText(QString::fromStdString(meeting.getReport()));
    ui->teacherLabel->setText(QString::fromStdString(
        teacher.getFirstName() + " " + teacher.getLastName()));
    
    exec();
    return choice;
}

void MeetingDialog::on_cancelButton_clicked() {
    choice = 1;
    accept();
}

void MeetingDialog::on_backButton_clicked() {
    choice = 0;
    accept();
}
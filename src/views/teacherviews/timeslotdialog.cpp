// timeslotdialog.cpp
#include "timeslotdialog.h"
#include "ui_timeslotdialog.h"
#include "../../utils/PopupUtils.h"
#include <QMessageBox>

TimeslotDialog::TimeslotDialog(int teacherId, QWidget *parent)
    : QDialog(parent), ui(new Ui::TimeslotDialog), teacherId(teacherId) {
    ui->setupUi(this);
    
    // Set up date/time constraints
    QDateTime currentDate = QDateTime::currentDateTime();
    ui->dateEdit->setMinimumDate(currentDate.date());
    ui->startTimeEdit->setMinimumTime(QTime(7, 0));
    ui->endTimeEdit->setMinimumTime(QTime(7, 0));
    
    // Connect signals/slots
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &TimeslotDialog::onAccept);
    connect(ui->startTimeEdit, &QTimeEdit::timeChanged, this, &TimeslotDialog::validateTimes);
    connect(ui->endTimeEdit, &QTimeEdit::timeChanged, this, &TimeslotDialog::validateTimes);
}

TimeslotDialog::~TimeslotDialog() {
    delete ui;
}

void TimeslotDialog::validateTimes() {
    QTime startTime = ui->startTimeEdit->time();
    QTime endTime = ui->endTimeEdit->time();
    
    if (endTime <= startTime) {
        ui->endTimeEdit->setTime(startTime.addSecs(3600)); // Add 1 hour
    }
}

bool TimeslotDialog::validateInput() {
    QTime startTime = ui->startTimeEdit->time();
    QTime endTime = ui->endTimeEdit->time();
    QDate date = ui->dateEdit->date();
    
    if (endTime <= startTime) {
        PopupUtils::showWarning("Lỗi", "Thời gian kết thúc phải sau thời gian bắt đầu!", this);
        return false;
    }
    
    if (date < QDate::currentDate()) {
        PopupUtils::showWarning("Lỗi", "Ngày phải từ hôm nay trở đi!", this);
        return false;
    }
    
    return true;
}

void TimeslotDialog::onAccept() {
    if (!validateInput()) {
        return;
    }
    
    QString type;
    if (ui->radioPersonal->isChecked()) {
        type = "personal";
    } else if (ui->radioGroup->isChecked()) {
        type = "group";
    } else {
        type = "both";
    }
    
    QString startTime = ui->startTimeEdit->time().toString("HH:mm:00");
    QString endTime = ui->endTimeEdit->time().toString("HH:mm:00");
    QString date = ui->dateEdit->date().toString("yyyy-MM-dd");
    
    resultTimeslot = Timeslot(startTime.toStdString(), 
                             endTime.toStdString(), 
                             date.toStdString(), 
                             type.toStdString(), 
                             teacherId);
    
    accept();
}

Timeslot TimeslotDialog::getTimeslot() const {
    return resultTimeslot;
}

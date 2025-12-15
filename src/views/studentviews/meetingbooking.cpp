#include "meetingbooking.h"
#include "ui_meetingbooking.h"
#include <QMessageBox>

MeetingBooking::MeetingBooking(QWidget *parent)
    : QDialog(parent), ui(new Ui::MeetingBooking) {
    ui->setupUi(this);
    
    // Setup button group for meeting type
    typeButtonGroup = new QButtonGroup(this);
    typeButtonGroup->addButton(ui->personalRadio, 1);
    typeButtonGroup->addButton(ui->groupRadio, 2);
    
    connect(typeButtonGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
        this, &MeetingBooking::onTypeSelectionChanged);

            
    // Hide type selection by default
    ui->typeGroupBox->hide();
}

MeetingBooking::~MeetingBooking() {
    delete ui;
}

Meeting MeetingBooking::showBookMeeting(const Timeslot& timeslot, const std::string& teacherName) {
    currentTimeslot = timeslot;
    
    // Set window title and information
    setWindowTitle(QString("Đặt lịch hẹn với %1").arg(QString::fromStdString(teacherName)));
    
    // Update information display
    ui->dateLabel->setText(QString::fromStdString(timeslot.getDate()));
    ui->timeLabel->setText(QString("Từ %1 đến %2")
        .arg(QString::fromStdString(timeslot.getStart()))
        .arg(QString::fromStdString(timeslot.getEnd())));
    
    // Show/hide type selection based on timeslot type
    if (timeslot.getType() == "both") {
        ui->typeGroupBox->show();
        ui->personalRadio->setChecked(true);
        ui->typeLabel->setText("Cá nhân");
    } else {
        ui->typeGroupBox->hide();
        ui->typeLabel->setText(QString::fromStdString(timeslot.getType()));
    }
    
    if (exec() == QDialog::Accepted) {
        return meeting;
    } else {
        Meeting canceledMeeting;
        canceledMeeting.setId(-1);
        return canceledMeeting;
    }
}

void MeetingBooking::onTypeSelectionChanged() {
    if (ui->personalRadio->isChecked()) {
        ui->typeLabel->setText("Cá nhân");
    } else if (ui->groupRadio->isChecked()) {
        ui->typeLabel->setText("Nhóm");
    }
}

void MeetingBooking::updateMeetingType() {
    std::string type;
    if (currentTimeslot.getType() == "both") {
        type = ui->personalRadio->isChecked() ? "personal" : "group";
    } else {
        type = currentTimeslot.getType();
    }
    meeting = Meeting(type, currentTimeslot.getId());
}

void MeetingBooking::on_buttonBox_accepted() {
    updateMeetingType();
    accept();
}

void MeetingBooking::on_buttonBox_rejected() {
    meeting.setId(-1);
    reject();
}

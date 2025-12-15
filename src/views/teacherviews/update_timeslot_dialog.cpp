#include "update_timeslot_dialog.h"
#include "ui_update_timeslot_dialog.h"
#include <QMessageBox>
#include <QString>

UpdateTimeslotDialog::UpdateTimeslotDialog(const Timeslot &timeslot, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateTimeslotDialog),
    originalTimeslot(timeslot)
{
    ui->setupUi(this);
    setWindowTitle("Cập nhật thời gian rảnh");

    // Điền thông tin ban đầu vào giao diện
    QTime startTime = QTime::fromString(QString::fromStdString(timeslot.getStart()), "HH:mm:ss");
    QTime endTime = QTime::fromString(QString::fromStdString(timeslot.getEnd()), "HH:mm:ss");
    
    ui->startTimeEdit->setTime(startTime);
    ui->endTimeEdit->setTime(endTime);

    // Điền loại cuộc hẹn
    if (timeslot.getType() == "personal") {
        ui->radioPersonal->setChecked(true);
    } else if (timeslot.getType() == "group") {
        ui->radioGroup->setChecked(true);
    } else if (timeslot.getType() == "both") {
        ui->radioBoth->setChecked(true);
    }
}

UpdateTimeslotDialog::~UpdateTimeslotDialog()
{
    delete ui;
}

void UpdateTimeslotDialog::on_buttonBox_accepted()
{
    // Lấy giờ và phút từ QTimeEdit
    QTime startTime = ui->startTimeEdit->time();
    QTime endTime = ui->endTimeEdit->time();

    // Chuyển đổi QTime thành chuỗi
    std::string start = startTime.toString("HH:mm:ss").toStdString();
    std::string end = endTime.toString("HH:mm:ss").toStdString();

    // Cập nhật Timeslot
    updatedTimeslot = originalTimeslot;  // Sao chép bản gốc
    updatedTimeslot.setStart(start);
    updatedTimeslot.setEnd(end);
    updatedTimeslot.setType(getSelectedType());

    accept();  // Đóng hộp thoại và trả về QDialog::Accepted
}

Timeslot UpdateTimeslotDialog::getUpdatedTimeslot() const
{
    return updatedTimeslot;
}

std::string UpdateTimeslotDialog::getSelectedType()
{
    if (ui->radioPersonal->isChecked()) {
        return "personal";
    } else if (ui->radioGroup->isChecked()) {
        return "group";
    } else if (ui->radioBoth->isChecked()) {
        return "both";
    }
    return "";  // Nếu không có loại nào được chọn, trả về chuỗi rỗng
}

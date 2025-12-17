#include "meetinghistorydialog.h"
#include "ui_meetinghistorydialog.h"
#include "../../utils/PopupUtils.h"
#include <QMessageBox>

MeetingHistoryDialog::MeetingHistoryDialog(const Meeting& meeting, const std::vector<User>& students, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MeetingHistoryDialog),
    meeting(meeting)
{
    ui->setupUi(this);
    setWindowTitle("Thông tin lịch hẹn");

    // Hiển thị thông tin cuộc họp
    ui->dateLabel->setText(QString::fromStdString(meeting.getDate()));
    ui->startLabel->setText(QString::fromStdString(meeting.getStart()));
    ui->endLabel->setText(QString::fromStdString(meeting.getEnd()));
    ui->typeLabel->setText(QString::fromStdString(meeting.getType()));
    ui->statusLabel->setText(QString::fromStdString(meeting.getStatus()));

    // Lưu và hiển thị văn bản cuộc họp
    originalReport = QString::fromStdString(meeting.getReport());
    ui->reportTextEdit->setText(originalReport);

    // Hiển thị danh sách người hẹn
    for (const auto& student : students) {
        QString fullName = QString::fromStdString(student.getFirstName() + " " + student.getLastName());
        QListWidgetItem* item = new QListWidgetItem(fullName);
        ui->studentsListWidget->addItem(item);
    }
}

MeetingHistoryDialog::~MeetingHistoryDialog()
{
    delete ui;
}

void MeetingHistoryDialog::on_editButton_clicked()
{
    if (ui->reportTextEdit->isReadOnly()) {
        ui->reportTextEdit->setReadOnly(false);
        ui->editButton->setText("Lưu thay đổi");
        ui->buttonBox->setEnabled(false);
    } else {
        ui->reportTextEdit->setReadOnly(true);
        ui->editButton->setText("Chỉnh sửa văn bản");
        ui->buttonBox->setEnabled(true);
    }
}

void MeetingHistoryDialog::on_buttonBox_accepted()
{
    accept();
}

void MeetingHistoryDialog::on_buttonBox_rejected()
{
    if (ui->reportTextEdit->toPlainText() != originalReport) {
        bool reply = PopupUtils::showStyledQuestion("Xác nhận",
                                    "Bạn có muốn hủy các thay đổi không?", this);
        if (reply) {
            reject();
        }
    } else {
        reject();
    }
}

QString MeetingHistoryDialog::getReport() const
{
    return ui->reportTextEdit->toPlainText();
}

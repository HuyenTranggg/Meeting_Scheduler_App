
#include "teacherlistviewer.h"
#include "ui_teacherlistviewer.h"
#include <QMessageBox>

TeacherListViewer::TeacherListViewer(QWidget *parent)
    : QDialog(parent), ui(new Ui::TeacherListViewer), selectedTeacherId(-1) {
    ui->setupUi(this);
    setWindowTitle("Danh sách giáo viên");
}

TeacherListViewer::~TeacherListViewer() {
    delete ui;
}

std::pair<QString, int> TeacherListViewer::showTeacherList(const std::vector<User>& teachers) {
    currentTeachers = teachers;
    ui->listWidget->clear();

    if (teachers.empty()) {
        QMessageBox::information(this, "Thông báo", "Không có giáo viên nào để chọn.");
        return std::make_pair(QString(), -1);
    }

    for (const auto& teacher : teachers) {
        QString displayText = QString("%1 %2 (ID: %3)")
            .arg(QString::fromStdString(teacher.getFirstName()))
            .arg(QString::fromStdString(teacher.getLastName()))
            .arg(teacher.getId());
        ui->listWidget->addItem(displayText);
    }

    if (exec() == QDialog::Accepted && selectedTeacherId != -1) {
        return std::make_pair(selectedTeacherName, selectedTeacherId);
    }
    return std::make_pair(QString(), -1);
}

void TeacherListViewer::on_listWidget_currentRowChanged(int currentRow) {
    if (currentRow >= 0 && currentRow < static_cast<int>(currentTeachers.size())) {
        const User& teacher = currentTeachers[currentRow];
        selectedTeacherId = teacher.getId();
        selectedTeacherName = QString::fromStdString(
            teacher.getFirstName() + " " + teacher.getLastName());
    }
}

void TeacherListViewer::on_buttonBox_accepted() {
    if (ui->listWidget->currentRow() == -1) {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn một giáo viên!");
        return;
    }
    accept();
}

void TeacherListViewer::on_buttonBox_rejected() {
    selectedTeacherId = -1;
    selectedTeacherName = QString();
    reject();
}

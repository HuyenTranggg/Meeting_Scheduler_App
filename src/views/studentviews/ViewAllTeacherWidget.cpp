#include "ViewAllTeacherWidget.h"
#include "../../utils/PopupUtils.h"
#include <QHeaderView>
#include <QMessageBox>

ViewAllTeacherWidget::ViewAllTeacherWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViewAllTeacherWidget)
{
    ui->setupUi(this);
    setupTable();
}

ViewAllTeacherWidget::~ViewAllTeacherWidget()
{
    delete ui;
}

void ViewAllTeacherWidget::setupTable()
{
    ui->tableWidget->setColumnCount(3);
    QStringList headers;
    headers << "STT" << "Họ và tên" << "ID";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    
    // Set table properties
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void ViewAllTeacherWidget::setTeachers(const std::vector<User>& teachers)
{
    teachersList = teachers;
    populateTable();
}

void ViewAllTeacherWidget::populateTable()
{
    ui->tableWidget->setRowCount(0);
    
    if (teachersList.empty()) {
        PopupUtils::showInfo("Thông báo", "Không có giáo viên nào để chọn.", this);
        return;
    }

    for (size_t i = 0; i < teachersList.size(); i++) {
        ui->tableWidget->insertRow(i);
        
        // STT
        QTableWidgetItem *sttItem = new QTableWidgetItem(QString::number(i + 1));
        ui->tableWidget->setItem(i, 0, sttItem);
        
        // Họ và tên
        QString fullName = QString::fromStdString(teachersList[i].getFirstName() + " " + teachersList[i].getLastName());
        QTableWidgetItem *nameItem = new QTableWidgetItem(fullName);
        ui->tableWidget->setItem(i, 1, nameItem);
        
        // ID
        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(teachersList[i].getId()));
        ui->tableWidget->setItem(i, 2, idItem);
    }
}

void ViewAllTeacherWidget::on_tableWidget_cellClicked(int row, int column)
{
    if (row >= 0 && row < teachersList.size()) {
        QString teacherName = QString::fromStdString(
            teachersList[row].getFirstName() + " " + teachersList[row].getLastName());
        emit teacherSelected(teachersList[row].getId(), teacherName);
    }
}

void ViewAllTeacherWidget::on_btnBack_clicked()
{
    emit backClicked();
}

#include "viewtimeslotsdialog.h"
#include "ui_viewtimeslotsdialog.h"
#include <QListWidget>
#include <QDate>
#include <QPushButton>
#include <QTextCharFormat>

ViewTimeslotsDialog::ViewTimeslotsDialog(const std::map<std::string, std::vector<Timeslot>>& timeslots, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewTimeslotsDialog),
    allTimeslots(timeslots)
{
    ui->setupUi(this);
    setupCalendar();
    populateCalendar(timeslots);

    // Đặt text cho các nút
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Chọn");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Đóng");
}

ViewTimeslotsDialog::~ViewTimeslotsDialog()
{
    delete ui;
}

void ViewTimeslotsDialog::setupCalendar()
{
    // Thiết lập ngày tối thiểu là ngày hiện tại
    ui->calendarWidget->setMinimumDate(QDate::currentDate());
    
    // Kết nối sự kiện chọn ngày
    connect(ui->calendarWidget, &QCalendarWidget::clicked, this, &ViewTimeslotsDialog::dateSelected);

    // Thiết lập định dạng hiển thị tiếng Việt cho lịch
    QLocale vietnameseLocale(QLocale::Vietnamese);
    ui->calendarWidget->setLocale(vietnameseLocale);
}

void ViewTimeslotsDialog::populateCalendar(const std::map<std::string, std::vector<Timeslot>>& timeslots)
{
    if (timeslots.empty()) {
        QMessageBox::information(this, "Thông báo", "Bạn chưa khai báo thời gian rảnh");
        reject();
        return;
    }
    
    highlightDatesWithSlots(timeslots);
}

void ViewTimeslotsDialog::highlightDatesWithSlots(const std::map<std::string, std::vector<Timeslot>>& timeslots)
{
    QTextCharFormat format;
    format.setBackground(Qt::lightGray);
    
    for (const auto& ts : timeslots) {
        const std::string& dateStr = ts.first;
        QDate date = QDate::fromString(QString::fromStdString(dateStr), "yyyy-MM-dd");
        if (date.isValid()) {
            ui->calendarWidget->setDateTextFormat(date, format);
        }
    }
}

void ViewTimeslotsDialog::dateSelected(const QDate& date)
{
    QString dateStr = date.toString("yyyy-MM-dd");
    showTimeslotsForDate(dateStr);
}

void ViewTimeslotsDialog::showTimeslotsForDate(const QString& dateStr)
{
    ui->timeslotsList->clear();
    editTimeslots.clear();
    
    std::string dateStdStr = dateStr.toStdString();
    if (allTimeslots.find(dateStdStr) != allTimeslots.end()) {
        const std::vector<Timeslot>& tss = allTimeslots[dateStdStr];
        int index = 1;
        
        for (const Timeslot& slot : tss) {
            QString displayText = QString("Từ %1 đến %2 (%3)")
                .arg(QString::fromStdString(slot.getStart()))
                .arg(QString::fromStdString(slot.getEnd()))
                .arg(QString::fromStdString(slot.getType()));
            
            QListWidgetItem* item = new QListWidgetItem(displayText);
            ui->timeslotsList->addItem(item);
            editTimeslots[index] = slot;
            index++;
        }
    }
}

void ViewTimeslotsDialog::accept()
{
    QListWidgetItem* selectedItem = ui->timeslotsList->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn một khoảng thời gian");
        return;
    }
    
    int index = ui->timeslotsList->row(selectedItem) + 1;
    if (editTimeslots.find(index) != editTimeslots.end()) {
        selectedTimeslot = editTimeslots[index];
        QDialog::accept();
    }
}

void ViewTimeslotsDialog::reject()
{
    selectedTimeslot.setId(-1);
    QDialog::reject();
}

int ViewTimeslotsDialog::showTimeslot(const Timeslot& timeslot)
{
    QString message = "-------------Thông tin thời gian rảnh-----------------\n";
    message += "Ngày: " + QString::fromStdString(timeslot.getDate()) + "\n";
    message += "Từ: " + QString::fromStdString(timeslot.getStart()) + "\n";
    message += "Đến: " + QString::fromStdString(timeslot.getEnd()) + "\n";
    message += "Loại: " + QString::fromStdString(timeslot.getType()) + "\n";
    message += "Trạng thái: " + QString::fromStdString(timeslot.getStatus()) + "\n";
    message += "------------------";
    
    QMessageBox::information(this, "Thông tin thời gian rảnh", message);
    
    int choice = QMessageBox::question(this, "Chỉnh sửa",
        "Bạn có muốn chỉnh sửa thời gian này?",
        QMessageBox::Yes | QMessageBox::No);
    
    return (choice == QMessageBox::Yes) ? 1 : 0;
}
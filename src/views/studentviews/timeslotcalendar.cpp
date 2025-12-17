// timeslotcalendar.cpp
#include "timeslotcalendar.h"
#include "ui_timeslotcalendar.h"
#include "../../utils/PopupUtils.h"
#include <QTextCharFormat>
#include <QMessageBox>

TimeslotCalendar::TimeslotCalendar(QWidget *parent)
    : QDialog(parent), ui(new Ui::TimeslotCalendar) {
    ui->setupUi(this);
    setupCalendarFormat();
}

TimeslotCalendar::~TimeslotCalendar() {
    delete ui;
}

void TimeslotCalendar::setupCalendarFormat() {
    ui->calendarWidget->setGridVisible(true);
    ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    ui->calendarWidget->setFirstDayOfWeek(Qt::Monday);
}

Timeslot TimeslotCalendar::showTimeslots(
    const std::map<std::string, std::vector<Timeslot>>& timeslots,
    const std::string& teacherName) {
    
    allTimeslots = timeslots;
    setWindowTitle(QString("Lịch của %1").arg(QString::fromStdString(teacherName)));

    if (timeslots.empty()) {
        PopupUtils::showInfo("Thông báo", 
            "Giáo viên này chưa khai báo thời gian rảnh", this);
        Timeslot ts;
        ts.setId(-1);
        return ts;
    }

    highlightDatesWithTimeslots();
    
    if (exec() == QDialog::Accepted && selectedTimeslot.getId() != -1) {
        return selectedTimeslot;
    }
    
    Timeslot ts;
    ts.setId(-1);
    return ts;
}

void TimeslotCalendar::highlightDatesWithTimeslots() {
    QTextCharFormat highlightFormat;
    highlightFormat.setBackground(Qt::green);
    
    for (const auto& [dateStr, tsss] : allTimeslots) {
        QDate date = QDate::fromString(QString::fromStdString(dateStr), "yyyy-MM-dd");
        if (date.isValid() && !tsss.empty()) {
            ui->calendarWidget->setDateTextFormat(date, highlightFormat);
        }
    }
}

void TimeslotCalendar::updateTimeslotList(const QString& dateStr) {
    ui->listWidget->clear();
    currentTimeslots.clear();
    
    std::string dateStdStr = dateStr.toStdString();
    if (allTimeslots.find(dateStdStr) != allTimeslots.end()) {
        const auto& tss = allTimeslots[dateStdStr];
        int index = 1;
        for (const auto& slot : tss) {
            QString displayText = QString("Từ %1 đến %2 (%3)")
                .arg(QString::fromStdString(slot.getStart()))
                .arg(QString::fromStdString(slot.getEnd()))
                .arg(QString::fromStdString(slot.getType()));
            ui->listWidget->addItem(displayText);
            currentTimeslots[index++] = slot;
        }
    }
}

void TimeslotCalendar::on_calendarWidget_clicked(const QDate &date) {
    selectedDate = date.toString("yyyy-MM-dd");
    updateTimeslotList(selectedDate);
}

void TimeslotCalendar::on_listWidget_doubleClicked(const QModelIndex &index) {
    int row = index.row() + 1;
    if (currentTimeslots.find(row) != currentTimeslots.end()) {
        selectedTimeslot = currentTimeslots[row];
        accept();
    }
}

void TimeslotCalendar::on_buttonBox_accepted() {
    int currentRow = ui->listWidget->currentRow();
    if (currentRow == -1) {
        PopupUtils::showWarning("Cảnh báo", "Vui lòng chọn một khung thời gian!", this);
        return;
    }
    selectedTimeslot = currentTimeslots[currentRow + 1];
    accept();
}

void TimeslotCalendar::on_buttonBox_rejected() {
    Timeslot ts;
    ts.setId(-1);
    selectedTimeslot = ts;
    reject();
}

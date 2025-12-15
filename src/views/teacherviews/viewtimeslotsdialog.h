#ifndef VIEWTIMESLOTSDIALOG_H
#define VIEWTIMESLOTSDIALOG_H

#include <QDialog>
#include <QCalendarWidget>
#include <QVBoxLayout>
#include <QMessageBox>
#include <map>
#include <vector>
#include "../../models/Timeslot.h"

namespace Ui {
class ViewTimeslotsDialog;
}

class ViewTimeslotsDialog : public QDialog {
    Q_OBJECT

public:
    explicit ViewTimeslotsDialog(const std::map<std::string, std::vector<Timeslot>>& timeslots, QWidget *parent = nullptr);
    ~ViewTimeslotsDialog();
    Timeslot getSelectedTimeslot() const { return selectedTimeslot; }
    int showTimeslot(const Timeslot& timeslot);

private slots:
    void dateSelected(const QDate& date);
    void accept() override;
    void reject() override;

private:
    Ui::ViewTimeslotsDialog *ui;
    void setupCalendar();
    void populateCalendar(const std::map<std::string, std::vector<Timeslot>>& timeslots);
    void highlightDatesWithSlots(const std::map<std::string, std::vector<Timeslot>>& timeslots);
    void showTimeslotsForDate(const QString& dateStr);
    
    std::map<std::string, std::vector<Timeslot>> allTimeslots;
    std::map<int, Timeslot> editTimeslots;
    Timeslot selectedTimeslot;
};

#endif // VIEWTIMESLOTSDIALOG_H
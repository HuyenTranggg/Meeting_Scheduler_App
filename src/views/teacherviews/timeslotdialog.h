// timeslotdialog.h
#ifndef TIMESLOTDIALOG_H
#define TIMESLOTDIALOG_H

#include <QDialog>
#include <QDateTime>
#include "../../models/Timeslot.h"

namespace Ui {
class TimeslotDialog;
}

class TimeslotDialog : public QDialog {
    Q_OBJECT

public:
    explicit TimeslotDialog(int teacherId, QWidget *parent = nullptr);
    ~TimeslotDialog();
    Timeslot getTimeslot() const;

private slots:
    void onAccept();
    void validateTimes();

private:
    Ui::TimeslotDialog *ui;
    int teacherId;
    Timeslot resultTimeslot;
    bool validateInput();
};

#endif // TIMESLOTDIALOG_H


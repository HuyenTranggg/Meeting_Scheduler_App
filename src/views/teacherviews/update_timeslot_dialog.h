#ifndef UPDATETIMESLOTDIALOG_H
#define UPDATETIMESLOTDIALOG_H

#include <QDialog>
#include <QString>
#include <QTime>
#include "../../models/Timeslot.h"  // Đảm bảo rằng bạn đã có lớp Timeslot

namespace Ui {
class UpdateTimeslotDialog;
}

class UpdateTimeslotDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateTimeslotDialog(const Timeslot &timeslot, QWidget *parent = nullptr);
    ~UpdateTimeslotDialog();

    Timeslot getUpdatedTimeslot() const;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::UpdateTimeslotDialog *ui;
    Timeslot originalTimeslot;
    Timeslot updatedTimeslot;
    
    std::string getSelectedType();  // Hàm giúp lấy loại cuộc hẹn đã chọn
};

#endif // UPDATETIMESLOTDIALOG_H

#ifndef STUDENTMENU_H
#define STUDENTMENU_H

#include <QDialog>

namespace Ui {
class StudentMenu;
}

class StudentMenu : public QDialog {
    Q_OBJECT

public:
    explicit StudentMenu(QWidget *parent = nullptr);
    ~StudentMenu();
    int showMenu();

private slots:
    void on_scheduleButton_clicked();
    void on_viewMeetingsButton_clicked();
    void on_logoutButton_clicked();

private:
    Ui::StudentMenu *ui;
    int choice = -1;
};

#endif
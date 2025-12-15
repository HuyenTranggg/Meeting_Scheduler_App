#include "StudentMenu.h"
#include "ui_StudentMenu.h"

StudentMenu::StudentMenu(QWidget *parent)
    : QDialog(parent), ui(new Ui::StudentMenu) {
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setModal(true);
}

StudentMenu::~StudentMenu() {
    delete ui;
}

int StudentMenu::showMenu() {
    choice = -1;
    exec();
    return choice;
}

void StudentMenu::on_scheduleButton_clicked() {
    choice = 1;
    accept();
}

void StudentMenu::on_viewMeetingsButton_clicked() {
    choice = 2;
    accept();
}

void StudentMenu::on_logoutButton_clicked() {
    choice = 0;
    accept();
}
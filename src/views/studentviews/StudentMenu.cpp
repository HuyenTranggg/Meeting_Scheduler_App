#include "StudentMenu.h"
#include "ui_StudentMenu.h"
#include <QScreen>
#include <QGuiApplication>

StudentMenu::StudentMenu(QWidget *parent)
    : QDialog(parent), ui(new Ui::StudentMenu) {
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setModal(true);
    
    // Set fixed size and center window
    setFixedSize(937, 750);
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);
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

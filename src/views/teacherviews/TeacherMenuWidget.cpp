#include "TeacherMenuWidget.h"
#include "ui_TeacherMenuWidget.h"
#include <QMessageBox>
#include <QScreen>
#include <QGuiApplication>

TeacherMenuWidget::TeacherMenuWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TeacherMenuWidget)
{
    ui->setupUi(this);
    
    // Set fixed size and center window
    setFixedSize(937, 750);
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);
}

TeacherMenuWidget::~TeacherMenuWidget()
{
    delete ui;
}

void TeacherMenuWidget::on_declareTimeSlotButton_clicked()
{
    emit actionSelected(1);
}

void TeacherMenuWidget::on_checkAvailabilityButton_clicked()
{
    emit actionSelected(2);
}

void TeacherMenuWidget::on_viewMeetingsButton_clicked()
{
    emit actionSelected(3);
}

void TeacherMenuWidget::on_viewHistoryButton_clicked()
{
    emit actionSelected(4);
}

void TeacherMenuWidget::on_logoutButton_clicked()
{
    emit actionSelected(0);
}

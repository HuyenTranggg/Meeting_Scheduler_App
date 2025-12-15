#include "TeacherMenuWidget.h"
#include "ui_TeacherMenuWidget.h"
#include <QMessageBox>

TeacherMenuWidget::TeacherMenuWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TeacherMenuWidget)
{
    ui->setupUi(this);
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

void TeacherMenuWidget::on_viewWeeklyMeetingsButton_clicked()
{
    emit actionSelected(5);
}

void TeacherMenuWidget::on_logoutButton_clicked()
{
    emit actionSelected(0);
}

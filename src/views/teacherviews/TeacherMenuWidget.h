#ifndef TEACHERMENUWIDGET_H
#define TEACHERMENUWIDGET_H

#include <QWidget>

namespace Ui {
class TeacherMenuWidget;
}

class TeacherMenuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TeacherMenuWidget(QWidget *parent = nullptr);
    ~TeacherMenuWidget();

signals:
    void actionSelected(int action);

private slots:
    void on_declareTimeSlotButton_clicked();
    void on_checkAvailabilityButton_clicked();
    void on_viewMeetingsButton_clicked();
    void on_viewHistoryButton_clicked();
    void on_logoutButton_clicked();

private:
    Ui::TeacherMenuWidget *ui;
};

#endif // TEACHERMENUWIDGET_H

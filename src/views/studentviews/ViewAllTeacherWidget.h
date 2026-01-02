#ifndef VIEWALLTEACHERWIDGET_H
#define VIEWALLTEACHERWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <vector>
#include "ui_ViewAllTeacherWidget.h"
#include "../../models/User.h"

namespace Ui {
class ViewAllTeacherWidget;
}

class ViewAllTeacherWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ViewAllTeacherWidget(QWidget *parent = nullptr);
    ~ViewAllTeacherWidget();
    void setTeachers(const std::vector<User>& teachers);

signals:
    void teacherSelected(int teacherId, const QString& teacherName);
    void backClicked();

private slots:
    void on_tableWidget_cellClicked(int row, int column);
    void on_btnBack_clicked();

private:
    Ui::ViewAllTeacherWidget *ui;
    std::vector<User> teachersList;
    void setupTable();
    void populateTable();
};

#endif // VIEWALLTEACHERWIDGET_H
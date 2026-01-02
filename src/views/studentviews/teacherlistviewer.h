#ifndef TEACHERLISTVIEWER_H
#define TEACHERLISTVIEWER_H

#include <QDialog>
#include <QVector>
#include "../../models/User.h"

namespace Ui {
class TeacherListViewer;
}

class TeacherListViewer : public QDialog {
    Q_OBJECT

public:
    explicit TeacherListViewer(QWidget *parent = nullptr);
    ~TeacherListViewer();
    std::pair<QString, int> showTeacherList(const std::vector<User>& teachers);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_listWidget_currentRowChanged(int currentRow);

private:
    Ui::TeacherListViewer *ui;
    std::vector<User> currentTeachers;
    int selectedTeacherId;
    QString selectedTeacherName;
};

#endif // TEACHERLISTVIEWER_H
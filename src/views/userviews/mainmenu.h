#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>

namespace Ui {
class MainMenu;
}

class MainMenu : public QWidget {
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu();
    int getChoice() const { return userChoice; }

signals:
    void loginSelected();
    void registerSelected();

private slots:
    void onLoginClicked();
    void onRegisterClicked();

private:
    Ui::MainMenu *ui;
    int userChoice;
};

#endif // MAINMENU_H
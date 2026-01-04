#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <map>
#include <string>

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget {
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();
    std::map<std::string, std::string> getLoginInfo();
    bool isLoginSuccessful() const { return loginSuccess; }

signals:
    void loginClicked();
    void backClicked();

private slots:
    void onLoginButtonClicked();
    void onBackButtonClicked();
    void onEnterPressed();

private:
    Ui::LoginWidget *ui;
    bool loginSuccess;
    void setupConnections();
};

#endif // LOGINWIDGET_H
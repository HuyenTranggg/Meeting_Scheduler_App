#ifndef REGISTERWIDGET_H
#define REGISTERWIDGET_H

#include <QWidget>
#include <map>
#include <string>

namespace Ui {
class RegisterWidget;
}

class RegisterWidget : public QWidget {
    Q_OBJECT

public:
    explicit RegisterWidget(QWidget *parent = nullptr);
    ~RegisterWidget();
    std::map<std::string, std::string> getRegisterInfo();
    bool isRegistrationSuccessful() const { return registrationSuccess; }

signals:
    void registerClicked();
    void backClicked();

private slots:
    void onRegisterButtonClicked();
    void onBackButtonClicked();
    void validateForm();

private:
    Ui::RegisterWidget *ui;
    bool registrationSuccess;
    void setupConnections();
};

#endif // REGISTERWIDGET_H
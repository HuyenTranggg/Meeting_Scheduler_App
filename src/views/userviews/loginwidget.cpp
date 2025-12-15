#include "loginwidget.h"
#include "ui_loginwidget.h"
#include <QMessageBox>
#include <QPushButton>
#include <QKeyEvent>

LoginWidget::LoginWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWidget)
    , loginSuccess(false)
{
    ui->setupUi(this);
    setWindowTitle("Đăng nhập");
    setupConnections();
    
    // Set focus to username field
    ui->usernameLineEdit->setFocus();
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::setupConnections()
{
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginWidget::onLoginButtonClicked);
    connect(ui->backButton, &QPushButton::clicked, this, &LoginWidget::onBackButtonClicked);
    
    // Handle Enter key in input fields
    connect(ui->usernameLineEdit, &QLineEdit::returnPressed, this, &LoginWidget::onEnterPressed);
    connect(ui->passwordLineEdit, &QLineEdit::returnPressed, this, &LoginWidget::onEnterPressed);
}

void LoginWidget::onLoginButtonClicked()
{
    if (ui->usernameLineEdit->text().isEmpty() || ui->passwordLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập đầy đủ thông tin!");
        return;
    }
    
    loginSuccess = true;
    emit loginClicked();
}

void LoginWidget::onBackButtonClicked()
{
    loginSuccess = false;
    emit backClicked();
    close();
}

void LoginWidget::onEnterPressed()
{
    if (!ui->usernameLineEdit->text().isEmpty() && !ui->passwordLineEdit->text().isEmpty()) {
        onLoginButtonClicked();
    }
}

std::map<std::string, std::string> LoginWidget::getLoginInfo()
{
    std::map<std::string, std::string> info;
    info["username"] = ui->usernameLineEdit->text().toStdString();
    info["password"] = ui->passwordLineEdit->text().toStdString();
    return info;
}
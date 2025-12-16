#include "loginwidget.h"
#include "ui_loginwidget.h"
#include <QMessageBox>
#include <QPushButton>
#include <QKeyEvent>
#include <QScreen>
#include <QGuiApplication>

LoginWidget::LoginWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWidget)
    , loginSuccess(false)
{
    ui->setupUi(this);
    setWindowTitle("Đăng nhập");
    setupConnections();
    
    // Set fixed size and center window
    setFixedSize(937, 750);
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);
    
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

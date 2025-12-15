#include "registerwidget.h"
#include "ui_registerwidget.h"
#include <QMessageBox>
#include <QKeyEvent>
#include <QPushButton>

RegisterWidget::RegisterWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegisterWidget)
    , registrationSuccess(false)
{
    ui->setupUi(this);
    setWindowTitle("Đăng ký tài khoản");
    setupConnections();
    
    // Set default role
    ui->studentRadio->setChecked(true);
    
    // Set focus to first field
    ui->usernameLineEdit->setFocus();
}

RegisterWidget::~RegisterWidget()
{
    delete ui;
}

void RegisterWidget::setupConnections()
{
    connect(ui->registerButton, &QPushButton::clicked, this, &RegisterWidget::onRegisterButtonClicked);
    connect(ui->backButton, &QPushButton::clicked, this, &RegisterWidget::onBackButtonClicked);
    
    // Connect text changed signals for validation
    connect(ui->usernameLineEdit, &QLineEdit::textChanged, this, &RegisterWidget::validateForm);
    connect(ui->passwordLineEdit, &QLineEdit::textChanged, this, &RegisterWidget::validateForm);
    connect(ui->firstNameLineEdit, &QLineEdit::textChanged, this, &RegisterWidget::validateForm);
    connect(ui->lastNameLineEdit, &QLineEdit::textChanged, this, &RegisterWidget::validateForm);
}

void RegisterWidget::validateForm()
{
    bool isValid = !ui->usernameLineEdit->text().isEmpty() &&
                   !ui->passwordLineEdit->text().isEmpty() &&
                   !ui->firstNameLineEdit->text().isEmpty() &&
                   !ui->lastNameLineEdit->text().isEmpty();
    
    ui->registerButton->setEnabled(isValid);
}

void RegisterWidget::onRegisterButtonClicked()
{
    registrationSuccess = true;
    emit registerClicked();
}

void RegisterWidget::onBackButtonClicked()
{
    registrationSuccess = false;
    emit backClicked();
    close();
}

std::map<std::string, std::string> RegisterWidget::getRegisterInfo()
{
    std::map<std::string, std::string> info;
    info["username"] = ui->usernameLineEdit->text().toStdString();
    info["password"] = ui->passwordLineEdit->text().toStdString();
    info["role"] = ui->studentRadio->isChecked() ? "student" : "teacher";
    info["first_name"] = ui->firstNameLineEdit->text().toStdString();
    info["last_name"] = ui->lastNameLineEdit->text().toStdString();
    return info;
}
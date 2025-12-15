

#include "mainmenu.h"
#include "ui_mainmenu.h"
#include <QGraphicsDropShadowEffect>

MainMenu::MainMenu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainMenu)
    , userChoice(0)
{
    ui->setupUi(this);
    setWindowTitle("Ứng dụng Quản lý Cuộc họp");
    
    // Add shadows to buttons
    auto addShadow = [](QPushButton* button) {
        QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect;
        shadow->setBlurRadius(15);
        shadow->setColor(QColor(0, 0, 0, 80));
        shadow->setOffset(3, 3);
        button->setGraphicsEffect(shadow);
    };
    
    addShadow(ui->loginButton);
    addShadow(ui->registerButton);
    
    // Connect signals
    connect(ui->loginButton, &QPushButton::clicked, this, &MainMenu::onLoginClicked);
    connect(ui->registerButton, &QPushButton::clicked, this, &MainMenu::onRegisterClicked);
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::onLoginClicked()
{
    userChoice = 1;
    emit loginSelected();
    close();
}

void MainMenu::onRegisterClicked()
{
    userChoice = 2;
    emit registerSelected();
    close();
}
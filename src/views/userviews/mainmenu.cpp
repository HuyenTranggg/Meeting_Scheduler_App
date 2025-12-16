

#include "mainmenu.h"
#include "ui_mainmenu.h"
#include <QGraphicsDropShadowEffect>
#include <QScreen>
#include <QGuiApplication>

MainMenu::MainMenu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainMenu)
    , userChoice(0)
{
    ui->setupUi(this);
    setWindowTitle("Ứng dụng Quản lý Cuộc họp");
    
    // Set fixed size
    setFixedSize(937, 750);
    
    // Center window on screen
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);
    
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

#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H

#include "LoginView.h"
#include "RegisterView.h"

// Khai báo trước hàm showRegisterView
void showRegisterView();  

void showLoginView() {
    LoginView loginView(showRegisterView);  // Truyền callback cho nút "Đăng ký"
    loginView.showLoginWindow();
}

void showRegisterView() {
    RegisterView registerView(showLoginView);  // Truyền callback cho nút "Đăng nhập"
    registerView.showRegisterWindow();
}

#endif // VIEWMANAGER_H

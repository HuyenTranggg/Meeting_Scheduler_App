#ifndef REGISTER_VIEW_H
#define REGISTER_VIEW_H

#include <gtk/gtk.h>
#include <string>
#include <iostream>
#include <functional>

class RegisterView {
public:
    RegisterView(const std::function<void()> &onLoginCallback);
    void showRegisterWindow();

private:
    GtkWidget *window;
    GtkWidget *mainBox;
    GtkWidget *usernameEntry;
    GtkWidget *passwordEntry;
    GtkWidget *firstNameEntry;
    GtkWidget *lastNameEntry;
    std::function<void()> onLoginCallback;

    static void onRegisterButtonClicked(GtkWidget *widget, gpointer data);
    static void onLoginButtonClicked(GtkWidget *widget, gpointer data);
};

RegisterView::RegisterView(const std::function<void()> &onLoginCallback)
    : onLoginCallback(onLoginCallback) {
    gtk_init(nullptr, nullptr);

    // Tạo cửa sổ
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Đăng ký");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);

    // Tạo container chính
    mainBox = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), mainBox);

    // Tạo các trường nhập liệu
    usernameEntry = gtk_entry_new();
    passwordEntry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(passwordEntry), false);
    firstNameEntry = gtk_entry_new();
    lastNameEntry = gtk_entry_new();

    GtkWidget *registerButton = gtk_button_new_with_label("Đăng ký");
    GtkWidget *loginButton = gtk_button_new_with_label("Đăng nhập");

    gtk_fixed_put(GTK_FIXED(mainBox), gtk_label_new("Tên đăng nhập:"), 100, 50);
    gtk_fixed_put(GTK_FIXED(mainBox), usernameEntry, 200, 50);
    gtk_fixed_put(GTK_FIXED(mainBox), gtk_label_new("Mật khẩu:"), 100, 100);
    gtk_fixed_put(GTK_FIXED(mainBox), passwordEntry, 200, 100);
    gtk_fixed_put(GTK_FIXED(mainBox), gtk_label_new("Họ:"), 100, 150);
    gtk_fixed_put(GTK_FIXED(mainBox), firstNameEntry, 200, 150);
    gtk_fixed_put(GTK_FIXED(mainBox), gtk_label_new("Tên:"), 100, 200);
    gtk_fixed_put(GTK_FIXED(mainBox), lastNameEntry, 200, 200);
    gtk_fixed_put(GTK_FIXED(mainBox), registerButton, 200, 250);
    gtk_fixed_put(GTK_FIXED(mainBox), loginButton, 100, 250);

    g_signal_connect(registerButton, "clicked", G_CALLBACK(onRegisterButtonClicked), this);
    g_signal_connect(loginButton, "clicked", G_CALLBACK(onLoginButtonClicked), this);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), nullptr);
}

void RegisterView::showRegisterWindow() {
    gtk_widget_show_all(window);
    gtk_main();
}

void RegisterView::onRegisterButtonClicked(GtkWidget *widget, gpointer data) {
    RegisterView *view = static_cast<RegisterView *>(data);
    std::string username = gtk_entry_get_text(GTK_ENTRY(view->usernameEntry));
    std::string password = gtk_entry_get_text(GTK_ENTRY(view->passwordEntry));
    std::string firstName = gtk_entry_get_text(GTK_ENTRY(view->firstNameEntry));
    std::string lastName = gtk_entry_get_text(GTK_ENTRY(view->lastNameEntry));

    // Xử lý logic đăng ký ở đây
    std::cout << "Tên đăng nhập: " << username << ", Mật khẩu: " << password << ", Họ: " << firstName << ", Tên: " << lastName << std::endl;

    gtk_widget_destroy(view->window);
}

void RegisterView::onLoginButtonClicked(GtkWidget *widget, gpointer data) {
    RegisterView *view = static_cast<RegisterView *>(data);
    gtk_widget_destroy(view->window);  // Đóng màn hình đăng ký
    if (view->onLoginCallback) {
        view->onLoginCallback();  // Gọi lại màn hình đăng nhập
    }
}

#endif

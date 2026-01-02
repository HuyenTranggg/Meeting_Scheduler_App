#ifndef LOGIN_VIEW_H
#define LOGIN_VIEW_H

#include <gtk/gtk.h>
#include <string>
#include <functional>

class LoginView {
public:
    LoginView(const std::function<void()> &onRegisterCallback)
        : onRegisterCallback(onRegisterCallback) {

        // Khởi tạo GTK
        gtk_init(nullptr, nullptr);

        // Tạo cửa sổ chính
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), "Đăng nhập");
        gtk_window_set_default_size(GTK_WINDOW(window), 500, 300);

        // Tạo container chính
        mainBox = gtk_fixed_new();
        gtk_container_add(GTK_CONTAINER(window), mainBox);

        // Tạo các trường nhập liệu
        usernameEntry = gtk_entry_new();
        passwordEntry = gtk_entry_new();
        gtk_entry_set_visibility(GTK_ENTRY(passwordEntry), false);

        // Tạo các nút
        GtkWidget *loginButton = gtk_button_new_with_label("Đăng nhập");
        GtkWidget *registerButton = gtk_button_new_with_label("Đăng ký");

        // Đặt các phần tử vào container
        gtk_fixed_put(GTK_FIXED(mainBox), gtk_label_new("Tên đăng nhập:"), 100, 60);
        gtk_fixed_put(GTK_FIXED(mainBox), usernameEntry, 230, 50);
        gtk_fixed_put(GTK_FIXED(mainBox), gtk_label_new("Mật khẩu:"), 100, 110);
        gtk_fixed_put(GTK_FIXED(mainBox), passwordEntry, 230, 100);
        gtk_fixed_put(GTK_FIXED(mainBox), loginButton, 100, 150);
        gtk_fixed_put(GTK_FIXED(mainBox), registerButton, 300, 150);

        // Gắn kết các tín hiệu với các hàm xử lý
        g_signal_connect(loginButton, "clicked", G_CALLBACK(onLoginButtonClicked), this);
        g_signal_connect(registerButton, "clicked", G_CALLBACK(onRegisterButtonClicked), this);
        g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), nullptr);
    }

    void showLoginWindow() {
        gtk_widget_show_all(window);
        gtk_main();
    }

    std::string getUsername() {
        return username;
    }

    std::string getPassword() {
        return password;
    }

private:
    GtkWidget *window;
    GtkWidget *mainBox;
    GtkWidget *usernameEntry;
    GtkWidget *passwordEntry;
    std::string username;
    std::string password;
    std::function<void()> onRegisterCallback;  // Callback để chuyển sang màn hình đăng ký

    static void onLoginButtonClicked(GtkWidget *widget, gpointer data) {
        LoginView *view = static_cast<LoginView *>(data);
        view->username = gtk_entry_get_text(GTK_ENTRY(view->usernameEntry));
        view->password = gtk_entry_get_text(GTK_ENTRY(view->passwordEntry));
        gtk_widget_destroy(view->window);  // Đóng cửa sổ đăng nhập
    }

    static void onRegisterButtonClicked(GtkWidget *widget, gpointer data) {
        LoginView *view = static_cast<LoginView *>(data);
        gtk_widget_destroy(view->window);  // Đóng cửa sổ đăng nhập
        if (view->onRegisterCallback) {
            view->onRegisterCallback();  // Gọi callback chuyển sang màn hình đăng ký
        }
    }
};

#endif

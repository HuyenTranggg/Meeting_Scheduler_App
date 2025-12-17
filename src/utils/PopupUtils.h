#ifndef POPUPUTILS_H
#define POPUPUTILS_H

#include <QMessageBox>
#include <QString>
#include <QWidget>
#include <QScreen>
#include <QGuiApplication>

class PopupUtils {
public:
    // Helper function to center widget on screen
    static void centerWidget(QWidget* widget) {
        if (!widget) return;
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->geometry();
        int x = (screenGeometry.width() - widget->width()) / 2;
        int y = (screenGeometry.height() - widget->height()) / 2;
        widget->move(x, y);
    }

    // Show styled message box with proper text centering
    static void showStyledMessageBox(const QString& title, const QString& text, QMessageBox::Icon icon, QWidget* parent = nullptr) {
        QMessageBox msgBox(parent);
        msgBox.setWindowTitle(title);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(icon);
        
        // Set text directly without HTML div wrapper
        msgBox.setText(text);
        msgBox.setTextFormat(Qt::PlainText);
        
        QString buttonColor;
        QString buttonHoverColor;
        QString buttonPressedColor;
        switch(icon) {
            case QMessageBox::Information:
                buttonColor = "#1976d2";
                buttonHoverColor = "#2196f3";
                buttonPressedColor = "#1565c0";
                break;
            case QMessageBox::Warning:
                buttonColor = "#f57c00";
                buttonHoverColor = "#ff9800";
                buttonPressedColor = "#e65100";
                break;
            case QMessageBox::Critical:
                buttonColor = "#e53935";
                buttonHoverColor = "#f44336";
                buttonPressedColor = "#c62828";
                break;
            default:
                buttonColor = "#1976d2";
                buttonHoverColor = "#2196f3";
                buttonPressedColor = "#1565c0";
        }
        
        msgBox.setStyleSheet(
            "QMessageBox { "
            "    background-color: #f5f5f5; "
            "    font-family: 'Segoe UI', Arial; "
            "    min-width: 400px; "
            "    max-width: 500px; "
            "} "
            "QLabel { "
            "    font-size: 15pt; "
            "    color: #333333; "
            "    padding: 20px; "
            "    text-align: center; "
            "    qproperty-alignment: AlignCenter; "
            "    min-height: 60px; "
            "} "
            "QPushButton { "
            "    font-size: 14pt; "
            "    padding: 12px 35px; "
            "    border-radius: 6px; "
            "    min-width: 100px; "
            "    min-height: 45px; "
            "    background-color: " + buttonColor + "; "
            "    color: white; "
            "    border: none; "
            "    margin: 10px; "
            "} "
            "QPushButton:hover { "
            "    background-color: " + buttonHoverColor + "; "
            "} "
            "QPushButton:pressed { "
            "    background-color: " + buttonPressedColor + "; "
            "} "
            "QDialogButtonBox { "
            "    qproperty-centerButtons: true; "
            "}"
        );
        
        // Center the message box on screen
        centerWidget(&msgBox);
        msgBox.exec();
    }

    // Show styled question dialog (Yes/No)
    static bool showStyledQuestion(const QString& title, const QString& text, QWidget* parent = nullptr) {
        QMessageBox msgBox(parent);
        msgBox.setWindowTitle(title);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        msgBox.setIcon(QMessageBox::Question);
        
        // Set text directly without HTML
        msgBox.setText(text);
        msgBox.setTextFormat(Qt::PlainText);
        
        // Translate buttons to Vietnamese
        msgBox.setButtonText(QMessageBox::Yes, "Có");
        msgBox.setButtonText(QMessageBox::No, "Không");
        
        msgBox.setStyleSheet(
            "QMessageBox { "
            "    background-color: #f5f5f5; "
            "    font-family: 'Segoe UI', Arial; "
            "    min-width: 400px; "
            "    max-width: 500px; "
            "} "
            "QLabel { "
            "    font-size: 15pt; "
            "    color: #333333; "
            "    padding: 20px; "
            "    text-align: center; "
            "    qproperty-alignment: AlignCenter; "
            "    min-height: 60px; "
            "} "
            "QPushButton { "
            "    font-size: 14pt; "
            "    padding: 12px 35px; "
            "    border-radius: 6px; "
            "    min-width: 100px; "
            "    min-height: 45px; "
            "    background-color: #1976d2; "
            "    color: white; "
            "    border: none; "
            "    margin: 10px; "
            "} "
            "QPushButton:hover { "
            "    background-color: #2196f3; "
            "} "
            "QPushButton:pressed { "
            "    background-color: #1565c0; "
            "} "
            "QDialogButtonBox { "
            "    qproperty-centerButtons: true; "
            "}"
        );
        
        centerWidget(&msgBox);
        return msgBox.exec() == QMessageBox::Yes;
    }

    // Convenience methods
    static void showInfo(const QString& title, const QString& text, QWidget* parent = nullptr) {
        showStyledMessageBox(title, text, QMessageBox::Information, parent);
    }

    static void showWarning(const QString& title, const QString& text, QWidget* parent = nullptr) {
        showStyledMessageBox(title, text, QMessageBox::Warning, parent);
    }

    static void showError(const QString& title, const QString& text, QWidget* parent = nullptr) {
        showStyledMessageBox(title, text, QMessageBox::Critical, parent);
    }
};

#endif // POPUPUTILS_H

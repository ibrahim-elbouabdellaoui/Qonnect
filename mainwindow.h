#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "loginwindow.h"
#include "SignUp.h"
#include "interface_user.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QStackedWidget *stackedWidget;
    LoginWindow *loginWindow;
    SignUp *signupWidget;
    Interface_user *userInterface;

private slots:
    void showSignup();
    void showLogin();
    void showUserInterface(const QString &email);
    void handleLoginFailed(const QString &errorMessage);
};

#endif // MAINWINDOW_H

#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include<QLineEdit>
#include<QPushButton>
#include"animatedbutton.h"

class LoginWindow : public QWidget
{
    Q_OBJECT

    QLineEdit *NameInput;
    QLineEdit *PasswordInput ;
    QPushButton *LoginButton ;
    QPushButton *SignUpButton ;

public:
    explicit LoginWindow(QWidget *parent = nullptr);


signals:
    void loginSuccessful(const QString &email);
    void showSignupRequested();
    void loginFailed(QString) ;

};

#endif // LOGINWINDOW_H

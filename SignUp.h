#ifndef SIGNUP_H
#define SIGNUP_H

#include<QLabel>
#include<QLineEdit>
#include<QPushButton>
#include"animatedbutton.h"
#include<QGraphicsDropShadowEffect>


class SignUp:public QWidget
{
    Q_OBJECT

    QWidget *Profil_Step ;
    QWidget *Contact_Step ;
    QWidget *Securite_Step ;
    QWidget *Confirmation_Step ;

public:
    SignUp();


signals:
    void signupSuccessful(const QString &email);
    void showLoginRequested();
};

#endif // SIGNUP_H

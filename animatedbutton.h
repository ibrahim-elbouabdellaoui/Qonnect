#ifndef ANIMATEDBUTTON_H
#define ANIMATEDBUTTON_H
#include<QString>
#include<QWidget>
#include<QPushButton>
#include<QEvent>
#include<QPropertyAnimation>


class AnimatedButton:public QPushButton
{
    Q_OBJECT
public:
    AnimatedButton(QString,QWidget * =nullptr);

    void enterEvent(QEnterEvent *)override;

    void leaveEvent(QEvent *)override;

    void animateSize(QSize) ;
};

#endif // ANIMATEDBUTTON_H

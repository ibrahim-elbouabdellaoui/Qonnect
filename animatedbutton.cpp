#include "animatedbutton.h"

AnimatedButton::AnimatedButton(QString NameButton,QWidget *Parent):QPushButton(NameButton,Parent) {
    setMinimumSize(300,35) ;
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

}

void AnimatedButton::enterEvent(QEnterEvent *event){
    animateSize(QSize(310, 40));
    QPushButton::enterEvent(event);
}

void AnimatedButton::leaveEvent(QEvent *event){
    animateSize(QSize(300,40));
    QPushButton::leaveEvent(event) ;
}

void AnimatedButton::animateSize(QSize newSize){
    QPropertyAnimation *animation = new QPropertyAnimation(this, "size");
    animation->setDuration(250);
    animation->setStartValue(size());
    animation->setEndValue(newSize);
    animation->start(QAbstractAnimation::DeleteWhenStopped);

}



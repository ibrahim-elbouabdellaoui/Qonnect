#include "Qwidget_cliquable.h"

Qwidget_cliquable::Qwidget_cliquable(QWidget *parent):QWidget(parent){
    setCursor(Qt::PointingHandCursor) ;
}

void Qwidget_cliquable::mousePressEvent(QMouseEvent *Event){
    if(Event->button()==Qt::LeftButton) emit clicked() ;
    QWidget::mousePressEvent(Event) ;
}

void Qwidget_cliquable::setchecked(bool c){
    if(c==true) setStyleSheet("background-color : #D2B4DE ;") ;
    else setStyleSheet(" background-color : #D4EFDF ; ") ;
}

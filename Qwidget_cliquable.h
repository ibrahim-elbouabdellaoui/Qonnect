#ifndef QWIDGET_CLIQUABLE_H
#define QWIDGET_CLIQUABLE_H
#include<QWidget>
#include<QMouseEvent>

class Qwidget_cliquable:public QWidget
{
    Q_OBJECT

public :
    Qwidget_cliquable(QWidget * = nullptr) ;
    void setchecked(bool) ;

signals:
    void clicked() ;

protected :
    void mousePressEvent(QMouseEvent *) override;
};

#endif // QWIDGET_CLIQUABLE_H

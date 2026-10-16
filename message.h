#ifndef MESSAGE_H
#define MESSAGE_H
#include<QWidget>
#include<QDateTime>
#include<QLabel>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QTimer>
#include<QPushButton>
#include<QPropertyAnimation>
#include<QSequentialAnimationGroup>
#include<QGraphicsScene>
#include<QGraphicsPixmapItem>
#include<QGraphicsView>
#include"Qwidget_cliquable.h"

class Message:public QWidget
{
    Q_OBJECT

    QWidget *message=nullptr ;
    QLabel *content=nullptr ;
    QLabel *send_time=nullptr ;
    QLabel *icon_message_read=nullptr ;
    QLabel *icon_emojis=nullptr  ;
    QLabel *fleche=nullptr ;
    Qwidget_cliquable *fleche_iconEmoji=nullptr ;
    bool fleche_iconEmoji_cliked=false ;
    QWidget *Emojis_Popup=nullptr ; //widget des Emojis  (commente a cause de crash)
    QPushButton *Emoji_under_msg=nullptr ;
    QWidget *Emoji_PopUp ;
    bool emoji_Reacted=false ;

    QHBoxLayout *MainLayout ;

    int id_message ;

    QHBoxLayout *layout_send_time_Icone ;

    QLabel    *reactionLabel ;

    int id_sender ;


public:
    Message(int,QString,QString,bool,QString,int,bool=0);
protected:
    void enterEvent(QEnterEvent*) ;

   void leaveEvent(QEvent*) ;

    public slots :
    void afficher_Emoji(QString) ;

    //bool eventFilter(QObject *,QEvent *) ;
   signals :
   void emojiClicked(const QString &);


};

#endif // MESSAGE_H


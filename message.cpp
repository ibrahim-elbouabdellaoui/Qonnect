#include "message.h"
#include"interface_user.h"
#include"MyMemory.h"

Message::Message(int id_message,QString Content,QString Send_time,bool this_user_sender,QString Emoji,int id_sender,bool message_read):id_message(id_message),id_sender(id_sender){


    reactionLabel= new QLabel(this) ;
    reactionLabel->hide() ;
    reactionLabel->setFixedSize(28,20) ;
    reactionLabel->setAlignment(Qt::AlignCenter) ;


    content= new QLabel(Content,this) ;
    content->setWordWrap(true);  //activation retour a la ligne ;

    send_time = new QLabel(Send_time,this) ;
    send_time->setStyleSheet("color: #666666; font-size: 10px;");


    message = new QWidget(this) ;



    QVBoxLayout *VBoxLayout= new QVBoxLayout(this) ;
    QHBoxLayout *mainLayout= new QHBoxLayout() ;

    VBoxLayout->setSpacing(0) ;

    QVBoxLayout *Message_layout=new QVBoxLayout(message) ;


    QHBoxLayout *content_translate =  new QHBoxLayout ;



    Message_layout->addLayout(content_translate) ;

    layout_send_time_Icone=new QHBoxLayout ;

    layout_send_time_Icone->addWidget(send_time) ;

    if(this_user_sender && message_read){
        icon_message_read = new QLabel() ;
        QPixmap Pix(":/icons/check-mark.png") ;
        icon_message_read->setPixmap(Pix.scaled(23,23,Qt::KeepAspectRatio,Qt::SmoothTransformation)) ;

        layout_send_time_Icone->addWidget(icon_message_read) ;

    }

    if(Emoji != "none"){ // si il existe un emoji
        reactionLabel->setVisible(true) ;

        QPixmap emoji(Emoji) ;
        reactionLabel->setPixmap(emoji.scaled(17,17, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        layout_send_time_Icone->addWidget(reactionLabel) ;
    }

    Message_layout->addLayout(layout_send_time_Icone) ;

    message->setLayout(Message_layout) ;



    if(this_user_sender){
       content_translate->addWidget(content) ;
       message->setStyleSheet("background-color:#DCF8C6; border-radius:10px ;border-top-right-radius: 0px;") ;
       mainLayout->addWidget(message) ;

      reactionLabel->setStyleSheet("background-color : #C8E6C9; border : 1px solid #A5D6A7; border-radius : 9px;");
    }
   else{

        reactionLabel->setStyleSheet("background-color : #E3F2FD; border : 1px solid #BBDEFB; border-radius : 9px;");

       QPushButton *translate=new QPushButton(this) ;
       translate->setIcon(QIcon(":/icons/translate.png")) ;
       translate->setIconSize(QSize(15, 15));
       translate->setFixedSize(22,22) ;
       translate->setStyleSheet("QPushButton{ border-radius:11px ; background : transparent ;}"
                                "QPushButton:hover { background-color : #e0f0ff } "
                                "QPushButton:pressed { background-color : #cce0ff }") ;

       connect(translate,&QPushButton::clicked,this,[=](){

           QString text_original = content->text() ;

           MyMemory M ;
           content->setText(M.Traduire(content->text(),"fr","ar")) ;


           QTimer::singleShot(2000, this, [=]() {
               content->setText(text_original) ;
           }) ;

       });

        content_translate->addWidget(content) ;
        content_translate->addWidget(translate) ;


    fleche_iconEmoji = new Qwidget_cliquable(this) ;
    fleche_iconEmoji->setFixedHeight(28);
    fleche_iconEmoji->setStyleSheet("background:transparent ; border : 1px solid #AAB8C2 ; border-radius:  11px ;") ;


    fleche_iconEmoji->setVisible(false) ; //invisible au debut

    //icone de emoji :
    icon_emojis = new QLabel(fleche_iconEmoji) ;
    QPixmap Pix1(":/icons/happy-face.png") ;
    icon_emojis->setPixmap(Pix1.scaled(20,20,Qt::KeepAspectRatio, Qt::SmoothTransformation)) ;
    icon_emojis->setStyleSheet("border : none ;") ;

    QHBoxLayout *layout_fleche_iconEmojis= new QHBoxLayout(fleche_iconEmoji) ;

      QLabel *fleche= new QLabel(fleche_iconEmoji) ;
       QPixmap Pix(":/icons/arrow-down-sign-to-navigate.png") ;
      fleche->setPixmap(Pix.scaled(8,8,Qt::KeepAspectRatio, Qt::SmoothTransformation)) ;
       fleche->setStyleSheet("background : transparent;border: none ;") ;

    layout_fleche_iconEmojis->addWidget(fleche) ;
    layout_fleche_iconEmojis->addWidget(icon_emojis) ;
    layout_fleche_iconEmojis->setContentsMargins(5,4,4,4) ;
    fleche_iconEmoji->setLayout(layout_fleche_iconEmojis) ;

    //couleur de message :
        message->setStyleSheet("background-color:#FFFFFF; border-radius:10px ;border-top-left-radius: 0px;") ;
        mainLayout->addWidget(message) ;
        mainLayout->addWidget(fleche_iconEmoji) ;

    emoji_Reacted= false ; //aucune react en imoji

    //connecter le widget des imojis :
    fleche_iconEmoji_cliked = false ;
    connect(fleche_iconEmoji,&Qwidget_cliquable::clicked,this,[=](){
        fleche_iconEmoji_cliked = true ;

      //deplacer la fleche :
        QPropertyAnimation *descente= new QPropertyAnimation(fleche,"pos") ;
        descente->setDuration(200) ;
        descente->setStartValue(fleche->pos()) ;
        descente->setEndValue(fleche->pos()+QPoint(0,4)) ;

        QPropertyAnimation *remonte = new QPropertyAnimation(fleche, "pos");
        remonte->setDuration(200);
        remonte->setStartValue(fleche->pos() + QPoint(0,4));
        remonte->setEndValue(fleche->pos());

        QSequentialAnimationGroup *groupe = new QSequentialAnimationGroup;
        groupe->addAnimation(descente);
        groupe->addAnimation(remonte);
        groupe->start(QAbstractAnimation::DeleteWhenStopped);


         Emojis_Popup= new QWidget(this) ;  //popup signifie que cet widget apparait au dessus les autres et se ferme lorsque je clique ailleurs
         Emojis_Popup->setStyleSheet("background-color : #EFEFEF;border-radius : 19px ;") ;
         QGridLayout *Layout_Emojis = new QGridLayout(Emojis_Popup) ;
         QStringList Emojis_path={":/icons/Emojis/like.svg",
                                 ":/icons/Emojis/dislike.svg",
                                   ":/icons/Emojis/crying.svg",
                                   ":/icons/Emojis/surprized.svg",
                                   ":/icons/Emojis/laugh.svg",
                                   ":/icons/Emojis/heart.svg"} ;

        for(int i=0;i<Emojis_path.size();i++){
            QPushButton *Emoji = new QPushButton(Emojis_Popup) ;
            Emoji->setIcon(QIcon(Emojis_path[i])) ;
            Emoji->setIconSize(QSize(25,25)) ;
            Emoji->setFixedSize(38,38) ;
            Emoji->setStyleSheet("QPushButton{border : none ; border-radius:18px ; background:transparent ;}"
                                 "QPushButton:hover{ background-color : rgba(122, 122, 122, 0.45) ;}") ;

            Layout_Emojis->addWidget(Emoji,0,i) ;

            connect(Emoji,&QPushButton::clicked,this,[=](){

               emit emojiClicked(Emojis_path[i]);

                fleche_iconEmoji->setVisible(false) ;

                Emojis_Popup->deleteLater() ;
            });


        }

        Emojis_Popup->setLayout(Layout_Emojis) ;

        mainLayout->addWidget(Emojis_Popup) ;


    });
   }

    VBoxLayout->insertLayout(0,mainLayout) ;


    setLayout(VBoxLayout) ;

    connect(this, &Message::emojiClicked, this, &Message::afficher_Emoji);




}


void Message::enterEvent(QEnterEvent *Event){
    if(fleche_iconEmoji){
    fleche_iconEmoji->setVisible(true) ;
    }
    QWidget::enterEvent(Event);
}


void Message::leaveEvent(QEvent *Event){
    if(fleche_iconEmoji){
    fleche_iconEmoji->setVisible(false) ;
    }
    QWidget::leaveEvent(Event) ;
}

void Message::afficher_Emoji(QString path_Emojis){
    //inserer dans database
    Inserer_database inserer ;
    inserer.inserer_Emoji(id_message,path_Emojis) ;
    inserer.signal_msg_sent(id_sender) ;


    reactionLabel->setVisible(true) ;
    QPixmap Emoji(path_Emojis) ;
    reactionLabel->setPixmap(Emoji.scaled(15,15, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    layout_send_time_Icone->addWidget(reactionLabel) ;


}

#ifndef INTERFACE_USER_H
#define INTERFACE_USER_H
#include"zone_recherche.h"
#include"AnimatedButtonEffect.h"
#include"mainwindow.h"
#include <QWidget>
#include"QApplication"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QPropertyAnimation>
#include <QTimer>
#include <QVector>
#include<QFile>
#include<QStackedWidget>
#include<QTimer>
#include"QScrollBar"
#include "recuperer_database.h"
#include "inserer_database.h"
#include "Qwidget_cliquable.h"
#include "message.h"
#include "carte_top.h"
#include"profil.h"

class Interface_user : public QWidget
{
    Q_OBJECT
    int id_moi ;   //ID DE L'UTILISATEUR ACTUEL
    QTimer *timer_for_msg_sent ;
    QTimer *timer_for_msg_read ;
    QTimer *timer_for_update_profil ;
    QTimer *timer_for_invitation_sent ;
       bool animationInvitationStart ;
       AnimatedButtonEffect *animationInvitation ;

    QTimer *timer_for_new_friend; // pour l'animation de bordure
       bool animationAmiStart ;
       AnimatedButtonEffect *animationAmis ;

    QWidget *WelcomeMessage  ;


    int status_contact_selectionnee ;
    QTimer *timer_for_status ;  // timer pour verifier regulierement le status de contact selectionnee

   void afficherListeAmis()  ;
    void afficherListInvitation() ;

   QVBoxLayout *layoutInvitations ;
   void clean(QLayout *)  ;
   void cleanMessageContainer() ;
   QWidget *Container_messages ;

   recuperer_database *R ;
   Inserer_database *I ;

   QPushButton* rechercher ;
   QPushButton*   Amis ;
   QPushButton* invitation ;
   QPushButton* profil  ;
   QPushButton *logout ;

   QScrollArea *ScrollArea_Invitations ;
   QWidget *Container_Invitations ;

   public:
    explicit Interface_user(const QString& email, QWidget *parent = nullptr);

    int id_contact_selectionne;

    void actualiserContacts();
    void  chargerMessages(int) ;

    void afficher_message_vide(int) ;

    void switchToMessagesView(int) ;  //**


    QWidget *Container_input ;
    Carte_top* carteTop ;
     QStackedWidget * zone_stack ;

    QList<QVariantMap> Contacts_Cartes  ;

    QVector<Qwidget_cliquable *> Carte_contact;

    QScrollArea *ScrollArea_Contacts_Cartes ;

    QVBoxLayout *main_layout_Container_Contacts_Cartes ;


     void showMessageComponents(bool show);



  public slots:
   // void afficherEmojiAuDessusDuMessage(QWidget *message, const QString &emojiPath);


  private:
    void chargerPhotoProfil(QLabel *, const QString &) ;
      void scrollToBottom() ;
    QWidget* createInputWidget();
   int index_selectionee ;


    QWidget  *Container_Contacts_Cartes  ;

    QVBoxLayout *mainVerticalLayout;
    QHBoxLayout *main_layout;
    QVBoxLayout * Container_messages_layout ;

    QLineEdit *Zone_input_msg;
    QPushButton *Button_send;
    QPushButton *Button_msg_vocal;

    Profil *Mon_profil ;
    Zone_recherche *zone_recherche ;

    QScrollArea *ScrollArea_Container_Messages ;

    void Zone_CarteContact_vide() ;
    void Zone_Amis_vide() ;
    void Zone_Invitation_vide() ;

    QScrollArea *ScrollArea_Amis ;
    QWidget *Container_Amis ;
    QVBoxLayout *Layout_Amis ;


};

#endif // INTERFACE_USER_H

#ifndef INSERER_DATABASE_H
#define INSERER_DATABASE_H
#include"database.h"
#include<QString>
#include<QDateTime>
#include<QCryptographicHash>

class Inserer_database
{
    DataBase *db ;
public:
    Inserer_database() ;
    void Inserer_SignUp_Info(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString);
    void Inserer_message(int,int,QString)const ;

    bool Ajouter_ami(int,int) ;

    bool ValiderInvitation(int,int) ;
    bool RefuserInvitation(int,int) ;

    bool Update_nom(int,QString) ;
    bool Update_prenom(int,QString) ;
    bool Update_adresse(int,QString) ;
    bool Update_birthday(int,QString) ;
    bool Update_email(int,QString) ;
    bool Update_tele(int,QString) ;
    bool Update_photo(int,QString) ;

    bool Update_msg_read(int,int) ;

    bool Status_EnLigne(int) ;
    bool Status_HorsLigne(int) ;

    bool signal_msg_sent(int) ;  //signal pour messages recus ou des emojis recus
    bool signal_msg_read(int) ;
    bool signal_update_profil(int) ;
    bool signal_invitation_sent(int) ;
    bool signal_new_friend(int) ;

    bool no_signal_msg_sent(int) ;
    bool no_signal_msg_read(int) ;
    bool no_signal_update_profil(int) ;
    bool no_signal_invitation_sent(int) ;
    bool no_signal_new_friend(int) ;


    bool inserer_Emoji(int,QString) ;

};

#endif // INSERER_DATABASE_H

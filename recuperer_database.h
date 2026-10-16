#ifndef RECUPERER_DATABASE_H
#define RECUPERER_DATABASE_H
#include<QSqlDatabase>
#include"database.h"

class recuperer_database
{
    DataBase *db ;
public:
    recuperer_database();
    int get_id_byEmail(const QString&) ;
    QList<QVariantMap> Recuperer_Amis(int) ;
    QList<QVariantMap> Recuperer_messages(int,int)const ;
    QList<QVariantMap> Recuperer_Contacts_DernierMessage(int) ;
    QList<QVariantMap> Recuperer_users(const QString&,int) ;
    QVariantMap Recuperer_info(int) ;
    QList<QVariantMap> Recuperer_invitations(int) ;

    int Verifie_inscription(const QString &,const QString &) ;
    bool verifierConversationExiste(int,int) ;

    int nbr_msg_unread(int,int) ;

    int nbr_msg_unread(int) ; //pour tous les contacts (la somme )
    int nbr_invitation(int) ;

    bool msg_sent(int) ;
    bool msg_read(int) ;
    bool profil_updated(int) ;
    bool invitation_sent(int) ;
    bool new_friend(int) ;

};


#endif // RECUPERER_DATABASE_H

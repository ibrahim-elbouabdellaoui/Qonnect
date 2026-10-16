#include "recuperer_database.h"
#include<QDateTime>
#include<QCryptographicHash>

recuperer_database::recuperer_database() {
    db= new DataBase() ;
}

int recuperer_database::get_id_byEmail(const QString& email){
    int id ;
    QSqlQuery Query ;
    Query.prepare("SELECT id_utilisateur "
                  "FROM Utilisateur "
                  "WHERE email=? ");
    Query.addBindValue(email) ;
    if(Query.exec() && Query.next()){
        id=Query.value(0).toInt() ;
        return id ;
    }
    else{
        qDebug()<<"Erreur : get_id_byEmail  " ;
        return -1 ;
    }
}

QList<QVariantMap> recuperer_database::Recuperer_Amis(int id_utilisateur){


    QList<QVariantMap> Amis ;

    QString sql="SELECT U.id_utilisateur,U.nom,U.prenom,U.photo_profil,U.status,U.last_seen "
                  " FROM Ami A "
                  " JOIN Utilisateur U ON ( "
                           "(A.id_senderInvitation = ? AND U.id_utilisateur = A.id_ami) "
                            "OR"
                            "(A.id_ami = ? AND U.id_utilisateur = A.id_senderInvitation) "
                   ") " ;


    QSqlQuery Query ;
    Query.prepare(sql) ;
    Query.addBindValue(id_utilisateur) ;
    Query.addBindValue(id_utilisateur) ;


    if(Query.exec()){
        while(Query.next()){
            QVariantMap Ami ;
            Ami["id"]=Query.value(0).toInt() ;
            Ami["nom"]=Query.value(1).toString() ;
            Ami["prenom"]=Query.value(2).toString() ;
            Ami["photo_profil"]=Query.value(3).toString() ;
            Ami["status"]=Query.value(4).toInt() ;
            Ami["last_seen"]=Query.value(5).toString() ;


            Amis.append(Ami) ;

        }
    }
    else{
        qDebug()<<"Erreur : "<<Query.lastError().text() ;
    }

    return Amis ;
}

QList<QVariantMap> recuperer_database::Recuperer_Contacts_DernierMessage(int id_utilisateur){

    QList<QVariantMap> Contacts_DernierMessage ;
    QSqlQuery Query ;
    Query.prepare("SELECT U.id_utilisateur,U.nom,U.prenom,M.content,message_read,MAX(M.send_time) AS dernier_envoi,U.photo_profil "
                  "FROM Message M "
                  "JOIN Utilisateur U ON (U.id_utilisateur=M.sender_id OR U.id_utilisateur=M.receiver_id) "
                  "WHERE U.id_utilisateur != ? AND (M.sender_id=? OR receiver_id=?) "
                  "GROUP BY U.id_utilisateur "
                  "ORDER BY dernier_envoi DESC ") ;

    Query.addBindValue(id_utilisateur) ;
    Query.addBindValue(id_utilisateur) ;
    Query.addBindValue(id_utilisateur) ;

    if(Query.exec()){
        while (Query.next()) {
            QVariantMap contact_derniermessage ;
            contact_derniermessage["id_utilisateur"]=Query.value(0).toInt() ;
            contact_derniermessage["nom"]=Query.value(1).toString() ;
            contact_derniermessage["prenom"]=Query.value(2).toString() ;
            contact_derniermessage["content"]=Query.value(3).toString() ;
            contact_derniermessage["message_read"]=Query.value(4).toBool() ;
            contact_derniermessage["send_time"]=Query.value(5).toDateTime() ;
            contact_derniermessage["photo_profil"]=Query.value(6).toString() ;

            Contacts_DernierMessage.append(contact_derniermessage) ;
        }
            return Contacts_DernierMessage ;
    }
    else {
        qDebug()<<"Erreur : Recuperer_Contacts_DernierMessage " << Query.lastError().text();
    }

    return Contacts_DernierMessage;
}


QList<QVariantMap> recuperer_database::Recuperer_messages(int id_utilisateur,int id_contact) const{

    QList<QVariantMap> Messages ;

    QSqlQuery Query ;

    Query.prepare("SELECT M.id_message,M.content,M.send_time,M.message_read,M.sender_id,M.receiver_id,M.emoji "
                  "FROM Message M "
                  "JOIN Utilisateur U1 ON M.sender_id=U1.id_utilisateur "
                  "JOIN Utilisateur U2 ON M.receiver_id=U2.id_utilisateur "
                  "WHERE U1.id_utilisateur=? AND M.receiver_id=? "
                  "OR U2.id_utilisateur=? AND M.sender_id=? "
                  "ORDER BY M.send_time ");

    Query.addBindValue(id_utilisateur) ;
    Query.addBindValue(id_contact) ;
    Query.addBindValue(id_utilisateur);
    Query.addBindValue(id_contact) ;

    if(Query.exec()){
        while(Query.next()){
            QVariantMap Message ;
            if(Query.value(5).toInt()==id_contact) Message["this_user_sender"]=true  ;
            else Message["this_user_sender"]=false ;

            Message["id_message"]=Query.value(0).toInt() ;
            Message["id_sender"]=Query.value(4).toInt() ;
            Message["content"]=Query.value(1).toString() ;
            Message["send_time"]=Query.value(2).toDateTime() ;
            Message["message_read"]=Query.value(3).toBool() ;
            Message["emoji"]=Query.value(6).toString() ;

            Messages.append(Message) ;
        }
    }

    return Messages ;
}


QList<QVariantMap> recuperer_database::Recuperer_users(const QString& texte,int id_user){
    QList<QVariantMap> Users ;
    QSqlQuery Query ;
    Query.prepare("SELECT id_utilisateur,prenom,nom,genre,photo_profil "
                  "FROM Utilisateur "
                  "WHERE ( LOWER(nom) LIKE LOWER(?) OR LOWER(prenom) LIKE LOWER(?) "
                  "OR LOWER(prenom || ' ' || nom) LIKE LOWER(?) "
                  "OR LOWER(nom || ' ' || prenom) LIKE LOWER(?) ) "
                  "AND id_utilisateur != ? "
                  "ORDER BY prenom ") ;
    Query.addBindValue(texte+"%") ;
    Query.addBindValue(texte+"%") ;
    Query.addBindValue(texte+"%") ;
    Query.addBindValue(texte+"%") ;
    Query.addBindValue(id_user) ;


    if(Query.exec()){
        while (Query.next()) {
            QVariantMap User ;
            User["id_utilisateur"]= Query.value(0).toInt() ;
            User["prenom"]= Query.value(1).toString() ;
            User["nom"]= Query.value(2).toString() ;
            User["genre"]= Query.value(3).toString() ;
            User["photo_profil"]= Query.value(4).toString() ;
            Users.append(User) ;
        }

    }
    else qDebug()<<"Erreur : hhhhhhhhhhhhhhhh "<<Query.lastError().text() ;

    return Users ;
}


QVariantMap recuperer_database::Recuperer_info(int id_user){
    QVariantMap User ;
    QSqlQuery Query ;
    Query.prepare("SELECT prenom,nom,date_naissance,adresse,email,genre,numero_telephone,photo_profil,status,last_seen "
                  "FROM Utilisateur "
                  "WHERE id_utilisateur = ? ") ;
    Query.addBindValue(id_user) ;

    if(Query.exec() && Query.next()){
            User["prenom"]= Query.value(0).toString() ;
            User["nom"]= Query.value(1).toString() ;
            User["date_naissance"]= Query.value(2).toString() ;
            User["adresse"]= Query.value(3).toString() ;
            User["email"]= Query.value(4).toString() ;
            User["genre"]= Query.value(5).toString() ;
            User["numero_telephone"]= Query.value(6).toString() ;
            User["photo_profil"]= Query.value(7).toString() ;
            User["status"]= Query.value(8).toInt() ;
            User["last_seen"]= Query.value(9).toString() ;

            return User ;

        }


       else qDebug()<< "erreur recuperer info:" << Query.lastError().text() ;

    return User;
}

bool recuperer_database::verifierConversationExiste(int id_moi,int id_other){
    QSqlQuery Query ;
    Query.prepare("SELECT * FROM Message WHERE (sender_id=? AND receiver_id=?) OR  (sender_id=? AND receiver_id=? )") ;
    Query.addBindValue(id_moi)   ;
    Query.addBindValue(id_other)   ;
    Query.addBindValue(id_other)   ;
    Query.addBindValue(id_moi)   ;

    if(Query.exec()){
        if(Query.next()) return 1 ;
        else return 0 ;
    }

    else qDebug()<<"Erreur :" << Query.lastError().text() ;

    return 0;
}


int recuperer_database::Verifie_inscription(const QString & email,const QString & password){
    QSqlQuery Query ;
    Query.prepare("SELECT s.mot_passe FROM Securite s "
                  "JOIN Utilisateur U ON s.id_securite=U.id_utilisateur "
                  "WHERE U.email=?  ") ;
    Query.addBindValue(email) ;

    if(Query.exec()){
        if(!Query.next()) return -1 ; //email n'existe pas
        else {
           return password == Query.value(0).toString() ? 1 : 0 ;
           // return (QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex()) == Query.value(0).toString()) ;
        }
    }
    return 0;
}


bool recuperer_database::msg_sent(int id_user){
    QSqlQuery Query ;
    Query.prepare("SELECT signal_msg_sent FROM Utilisateur WHERE id_utilisateur=? ")     ;
    Query.addBindValue(id_user) ;

    if(Query.exec() && Query.next()){
        bool msg_sent = Query.value(0).toBool() ;
        return msg_sent ;
    }

    else{
        qDebug()<<"Erreur signal received " ;
    }

}


bool recuperer_database::msg_read(int id_user){
    QSqlQuery Query ;
    Query.prepare("SELECT signal_msg_read FROM Utilisateur WHERE id_utilisateur=? ")     ;
    Query.addBindValue(id_user) ;

    if(Query.exec() && Query.next()){
        bool msg_read = Query.value(0).toBool() ;
        return msg_read ;
    }

    else{
        qDebug()<<"Erreur signal received " ;
    }
}


int recuperer_database::nbr_msg_unread(int id_user,int id_contact){
    QSqlQuery  Query ;
    Query.prepare("SELECT COUNT(*) FROM Message WHERE receiver_id=? AND sender_id=? AND message_read= 0 ") ;
    Query.addBindValue(id_user) ;
    Query.addBindValue(id_contact) ;

    if(Query.exec() && Query.next()){
        return Query.value(0).toInt() ;
    }

    else return -1 ;
}

bool recuperer_database::profil_updated(int id_user){
    QSqlQuery Query ;
    Query.prepare("SELECT signal_update_profil FROM Utilisateur WHERE id_utilisateur=? ")     ;
    Query.addBindValue(id_user) ;

    if(Query.exec() && Query.next()){
        bool updated_profil = Query.value(0).toBool() ;
        return updated_profil ;
    }

    else{
        qDebug()<<"Erreur signal received " ;
    }
}


QList<QVariantMap> recuperer_database::Recuperer_invitations(int id){
    QList<QVariantMap> invitations ;

    QSqlQuery Query ;
    Query.prepare("SELECT id_invitation,id_sender,id_otherUser "
                  "FROM Invitation WHERE id_otherUser=? "
                  "ORDER BY date_add DESC ") ;
    Query.addBindValue(id) ;

    if(Query.exec()){
        while(Query.next()){
            QVariantMap invitation ;
            invitation["id_invitation"]= Query.value(0).toInt() ;
            invitation["id_sender"]= Query.value(1).toInt() ;
            invitation["id_otherUser"]= Query.value(2).toInt() ;

            invitations.append(invitation) ;

        }
    }

    return invitations ;
}


bool recuperer_database::invitation_sent(int id_user){
    QSqlQuery Query ;
    Query.prepare("SELECT signal_invitation_sent FROM Utilisateur WHERE id_utilisateur=? ")     ;
    Query.addBindValue(id_user) ;

    if(Query.exec() && Query.next()){
        bool invitation_sent = Query.value(0).toBool() ;
        return invitation_sent ;
    }

    else{
        qDebug()<<"Erreur signal received " ;
    }
}

int recuperer_database::nbr_msg_unread(int id_user){
    QSqlQuery Query ;

    Query.prepare("SELECT COUNT(*) FROM Message WHERE receiver_id=? AND message_read=0 ") ;
    Query.addBindValue(id_user) ;

    if(Query.exec() && Query.next()){
        return Query.value(0).toInt() ;
    }

    else {
        qDebug()<<"erreur calculer nbr_message_unread " ;
    }

}


int recuperer_database::nbr_invitation(int id_user){
    QSqlQuery Query ;

    Query.prepare("SELECT COUNT(*) FROM Invitation WHERE id_otherUser=? ") ;
    Query.addBindValue(id_user) ;

    if(Query.exec() && Query.next()){
        return Query.value(0).toInt() ;
    }

    else {
        qDebug()<<"erreur calculer nbr_invitation " ;
    }

}

bool recuperer_database::new_friend(int id_user){

    QSqlQuery Query ;
    Query.prepare("SELECT signal_new_friend FROM Utilisateur WHERE id_utilisateur=? ")     ;
    Query.addBindValue(id_user) ;

    if(Query.exec() && Query.next()){
        bool new_friend = Query.value(0).toBool() ;
        return new_friend ;
    }

    else{
        qDebug()<<"Erreur signal received " ;
    }
}

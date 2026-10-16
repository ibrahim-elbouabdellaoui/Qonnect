#include "inserer_database.h"
#include"database.h"
#include<QSqlDatabase>
#include<QSqlQuery>


Inserer_database::Inserer_database(){
    db =new DataBase() ;

}


void Inserer_database::Inserer_SignUp_Info(QString prenom,QString nom,QString date_naissance,QString adresse,QString genre,QString email,QString num_tele,QString mot_passe,QString question_securite,QString reponce_question_securite){

    // insersion a Utilisateur
    QSqlQuery Query1 ;
    Query1.prepare("INSERT INTO Utilisateur(prenom,nom,date_naissance,adresse,genre,email,numero_telephone)"
                  "VALUES (?,?,?,?,?,?,?)") ;

    Query1.addBindValue(prenom) ;
    Query1.addBindValue(nom) ;
    Query1.addBindValue(date_naissance) ;
    Query1.addBindValue(adresse) ;
    Query1.addBindValue(genre) ;
    Query1.addBindValue(email) ;
    Query1.addBindValue(num_tele) ;

    if(!Query1.exec()){
        qDebug()<<"erreur d'insersion de donnees : "<<Query1.lastError().text() ;
    }

    int id_utilisateur = Query1.lastInsertId().toInt() ;  // recuperer l'id pour l'utiliser dans table securite



    //hachage de reponce de qestion de securite :
    QByteArray Hash_question_securite= QCryptographicHash::hash(reponce_question_securite.toUtf8(),QCryptographicHash::Sha256) ;
    QString reponce_question_securite_hache=QString(Hash_question_securite.toHex()) ;

    // insersion a Securite
    QSqlQuery Query2 ;
    Query2.prepare("INSERT INTO Securite(id_securite,mot_passe,question_securite,reponce_question_securite)"
                   "VALUES (?,?,?,?)") ;

    Query2.addBindValue(id_utilisateur) ;
    Query2.addBindValue(mot_passe) ;
    Query2.addBindValue(question_securite) ;
    Query2.addBindValue(reponce_question_securite_hache) ;

    if(!Query2.exec()){
        qDebug()<<"erreur d'in" ;
    }


}

void Inserer_database::Inserer_message(int id_sender,int id_receiver,QString S)const{
    QSqlQuery Query ;
    Query.prepare("INSERT INTO Message(sender_id,receiver_id,content,send_time) "
                  "VALUES (?,?,?,?) ");
    Query.addBindValue(id_sender) ;
    Query.addBindValue(id_receiver) ;
    Query.addBindValue(S) ;
    Query.addBindValue(QDateTime::currentDateTime()) ;
    if(!Query.exec()){
        qDebug()<<"erreur d'insertion des messages !"<<Query.lastError().text() ;
    }

}

bool Inserer_database::Ajouter_ami(int id_user,int id_other){
    QSqlQuery Query ;
    Query.prepare("INSERT INTO Invitation(id_sender,id_otherUser,date_add) "
                  " VALUES (?,?,?) ") ;
    Query.addBindValue(id_user) ;
    Query.addBindValue(id_other) ;
    Query.addBindValue(QDateTime::currentDateTime());

    if(Query.exec()){
        return true ;
    }
    else{
        qDebug()<<"erreur d'ajouter ami " ;
        return false ;
    }
}

bool Inserer_database::Update_nom(int id_user,QString nom){
    QSqlQuery Query ;
    Query.prepare("UPDATE Utilisateur SET nom=? WHERE id_utilisateur=? ") ;
    Query.addBindValue(nom) ;
    Query.addBindValue(id_user) ;

    if(Query.exec()){
        return true ;
    }
    else{
        qDebug()<<"erreur d'update nom" ;
        return false ;
    }

}


bool Inserer_database::Update_prenom(int id_user,QString prenom){
    QSqlQuery Query ;
    Query.prepare("UPDATE Utilisateur SET prenom=? WHERE id_utilisateur=? ") ;
    Query.addBindValue(prenom) ;
    Query.addBindValue(id_user) ;

    if(Query.exec()){
        return true ;
    }
    else{
        qDebug()<<"erreur d'update prenom" ;
        return false ;
    }
}


bool Inserer_database::Update_adresse(int id_user,QString adresse){
    QSqlQuery Query ;
    Query.prepare("UPDATE Utilisateur SET adresse=? WHERE id_utilisateur=? ") ;
    Query.addBindValue(adresse) ;
    Query.addBindValue(id_user) ;

    if(Query.exec()){
        return true ;
    }
    else{
        qDebug()<<"erreur d'update adresse" ;
        return false ;
    }
}



bool Inserer_database::Update_birthday(int id_user,QString birthday){
    QSqlQuery Query ;
    Query.prepare("UPDATE Utilisateur SET date_naissance=? WHERE id_utilisateur=? ") ;
    Query.addBindValue(birthday) ;
    Query.addBindValue(id_user) ;

    if(Query.exec()){
        return true ;
    }
    else{
        qDebug()<<"erreur d'update date naissance" ;
        return false ;
    }
}


bool Inserer_database::Update_email(int id_user,QString email){
    QSqlQuery Query ;
    Query.prepare("UPDATE Utilisateur SET email=? WHERE id_utilisateur=? ") ;
    Query.addBindValue(email) ;
    Query.addBindValue(id_user) ;

    if(Query.exec()){
        return true ;
    }
    else{
        qDebug()<<"erreur d'update email" ;
        return false ;
    }
}


bool Inserer_database::Update_tele(int id_user,QString tele){
    QSqlQuery Query ;
    Query.prepare("UPDATE Utilisateur SET numero_telephone=? WHERE id_utilisateur=? ") ;
    Query.addBindValue(tele) ;
    Query.addBindValue(id_user) ;

    if(Query.exec()){
        return true ;
    }
    else{
        qDebug()<<"erreur d'update tele" ;
        return false ;
    }
}

bool Inserer_database::Update_photo(int id_user,QString path){
    QSqlQuery Query ;
    Query.prepare("UPDATE Utilisateur SET photo_profil=? WHERE id_utilisateur=? ") ;
    Query.addBindValue(path) ;
    Query.addBindValue(id_user) ;

    if(Query.exec()){
        return true ;
    }
    else{
        qDebug()<<"erreur d'update photo" ;
        return false ;
    }
}

bool Inserer_database::Status_EnLigne(int id_user){
    QSqlQuery Query ;
    Query.prepare("UPDATE Utilisateur SET status=1 WHERE id_utilisateur=? ") ;
    Query.addBindValue(id_user) ;


    if(Query.exec()){
        return true ;
    }

    else{
        qDebug()<<"erreur d'update status en ligne " ;
        return false ;
    }

}


bool Inserer_database::Status_HorsLigne(int id_user){

    QSqlQuery Query ;
    Query.prepare("UPDATE Utilisateur SET status=0,last_seen=? WHERE id_utilisateur=? ") ;
    Query.addBindValue(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")) ;
    Query.addBindValue(id_user) ;

    if(Query.exec()){
        return true ;
    }

    else{
        qDebug()<<"erreur d'update status en ligne " ;
        return false ;
    }

}


bool Inserer_database::signal_msg_sent(int id_user){
    QSqlQuery Query ;
    Query.prepare("UPDATE Utilisateur SET signal_msg_sent=1 WHERE id_utilisateur=? AND status=1 ") ;
    Query.addBindValue(id_user) ;


    if(Query.exec()){
        return true ;
    }

    else{
        qDebug()<<"erreur d'envoyer signal msg_sent " ;
        return false ;
    }
}


bool Inserer_database::no_signal_msg_sent(int id_user){
    QSqlQuery Query ;
    Query.prepare("UPDATE Utilisateur SET signal_msg_sent=0 WHERE id_utilisateur=? ") ;
    Query.addBindValue(id_user) ;


    if(Query.exec()){
        return true ;
    }

    else{
        qDebug()<<"erreur d'annuler le signal " ;
        return false ;
    }
}

bool Inserer_database::no_signal_msg_read(int id_user){
    QSqlQuery Query ;
    Query.prepare("UPDATE Utilisateur SET signal_msg_read=0 WHERE id_utilisateur=? ") ;
    Query.addBindValue(id_user) ;


    if(Query.exec()){
        return true ;
    }

    else{
        qDebug()<<"erreur d'annuler le signal " ;
        return false ;
    }
}

bool Inserer_database::Update_msg_read(int id_user,int id_contact){
    QSqlDatabase database = QSqlDatabase::database();

    QSqlQuery Query ;
    Query.prepare("UPDATE Message SET message_read=1 WHERE sender_id=? AND receiver_id=? AND message_read=0 ") ;

    Query.addBindValue(id_contact) ;
    Query.addBindValue(id_user) ;


    if(Query.exec()){
        return true ;
    }

    else{
        qDebug()<<"erreur de update message read  " ;
        return false ;
    }

}

bool Inserer_database::signal_msg_read(int id_user){
    QSqlQuery Query ;
    Query.prepare("UPDATE Utilisateur SET signal_msg_read=1 WHERE id_utilisateur=? AND status=1 ") ;
    Query.addBindValue(id_user) ;


    if(Query.exec()){
        return true ;
    }

    else{
        qDebug()<<"erreur d'envoyer signal msg_read " ;
        return false ;
    }
}


bool Inserer_database::signal_update_profil(int id_user){
    QSqlQuery Query ;
    Query.prepare("UPDATE Utilisateur SET signal_update_profil=1 WHERE id_utilisateur=? AND status=1 ") ;
    Query.addBindValue(id_user) ;


    if(Query.exec()){
        return true ;
    }

    else{
        qDebug()<<"erreur d'envoyer signal update profil " ;
        return false ;
    }
}

bool Inserer_database::no_signal_update_profil(int id_user){
    QSqlQuery Query ;
    Query.prepare("UPDATE Utilisateur SET signal_update_profil=0 WHERE id_utilisateur=? ") ;
    Query.addBindValue(id_user) ;


    if(Query.exec()){
        return true ;
    }

    else{
        qDebug()<<"erreur d'annuler le signal " ;
        return false ;
    }
}


bool Inserer_database::inserer_Emoji(int id_message,QString pathEmoji){
    QSqlQuery Query ;
    Query.prepare("UPDATE Message SET emoji=? WHERE id_message=? ") ;
    Query.addBindValue(pathEmoji) ;
    Query.addBindValue(id_message) ;



    if(Query.exec()){
        return true ;
    }

    else{
        qDebug()<<"erreur d'INSERER EMOJI" ;
        return false ;
    }
}

bool Inserer_database::ValiderInvitation(int id_sender,int id_other){

    QSqlQuery Query1,Query2 ;
    Query1.prepare("INSERT INTO Ami(id_senderInvitation,id_ami,date_add) "
                  "VALUES (?,?,?) ") ;

    Query1.addBindValue(id_sender) ;
    Query1.addBindValue(id_other) ;
    Query1.addBindValue(QDateTime::currentDateTime()) ;

    Query2.prepare("DELETE FROM Invitation WHERE id_sender=? AND id_otherUser=? ") ;
    Query2.addBindValue(id_sender) ;
    Query2.addBindValue(id_other) ;


    if(Query1.exec() && Query2.exec()){
        return true ;
    }

    else{
        qDebug()<<"erreur de valider invittaion" ;
        return false ;
    }
}

bool Inserer_database::RefuserInvitation(int id_sender,int id_other){

    QSqlQuery Query ;
    Query.prepare("DELETE FROM Invitation WHERE id_sender=? AND id_otherUser=? ") ;
    Query.addBindValue(id_sender) ;
    Query.addBindValue(id_other) ;

    if(Query.exec()){
        return true ;
    }

    else{
        qDebug()<<"erreur de refuser invittaion" ;
        return false ;
    }

}

bool Inserer_database::signal_invitation_sent(int id_user){
    QSqlQuery Query ;
    Query.prepare("UPDATE Utilisateur SET signal_invitation_sent=1 WHERE id_utilisateur=? ") ;
    Query.addBindValue(id_user) ;


    if(Query.exec()){
        return true ;
    }

    else{
        qDebug()<<"erreur d'envoyer signal invitation_sent " ;
        return false ;
    }
}

bool Inserer_database::no_signal_invitation_sent(int id_user){
    QSqlQuery Query ;
    Query.prepare("UPDATE Utilisateur SET signal_invitation_sent=0 WHERE id_utilisateur=? ") ;
    Query.addBindValue(id_user) ;


    if(Query.exec()){
        return true ;
    }

    else{
        qDebug()<<"erreur d'annuler signal invitation_sent " ;
        return false ;
    }
}

bool Inserer_database::signal_new_friend(int id_user){
    QSqlQuery Query ;
    Query.prepare("UPDATE Utilisateur SET signal_new_friend=1 WHERE id_utilisateur=? ") ;
    Query.addBindValue(id_user) ;


    if(Query.exec()){
        return true ;
    }

    else{
        qDebug()<<"erreur d'envoyer signal new friend " ;
        return false ;
    }
}

bool Inserer_database::no_signal_new_friend(int id_user){
    QSqlQuery Query ;
    Query.prepare("UPDATE Utilisateur SET signal_new_friend=0 WHERE id_utilisateur=? ") ;
    Query.addBindValue(id_user) ;


    if(Query.exec()){
        return true ;
    }

    else{
        qDebug()<<"erreur d'annuler signal new friend " ;
        return false ;
    }
}

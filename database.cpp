#include "database.h"
#include<QDebug>
#include<QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QFile>
#include <QCoreApplication>
#include <QDir>



DataBase::DataBase(){
    // Reuse the default connection: every repository object creates a
    // DataBase instance, and replacing this connection invalidates queries
    // still owned by another repository object.
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection)) {
        db = QSqlDatabase::database(QSqlDatabase::defaultConnection);
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE");
        const QString databasePath =
            QDir(QCoreApplication::applicationDirPath()).filePath("Chat_App.db");
        db.setDatabaseName(databasePath);
        qDebug() << "Base de données :" << databasePath;
    }

    if(!db.open()){
        qDebug()<<"Erreur d'ouverture : "<<db.lastError() ;
    }

    QString Create_Table_Utilisateur = "CREATE TABLE IF NOT EXISTS Utilisateur("
                           " id_utilisateur INTEGER PRIMARY KEY AUTOINCREMENT ,"
                           " prenom TEXT NOT NULL ,"
                           " nom TEXT NOT NULL ,"
                           " date_naissance TEXT ,"
                           " adresse TEXT ,"
                           " genre TEXT CHECK(LENGTH(genre) = 1) ,"
                           " email TEXT ,"
                           " numero_telephone TEXT ,"
                           " photo_profil TEXT ,"
                           " status INTEGER DEFAULT 0 ,"
                           " last_seen DATETIME ,"
                           " signal_msg_sent INTEGER DEFAULT 0 ,"
                           " signal_msg_read INTEGER DEFAULT 0 ,"
                           " signal_update_profil INTEGER DEFAULT 0 ,"
                           " signal_invitation_sent INTEGER DEFAULT 0 ,"
                           " signal_new_friend INTEGER DEFAULT 0 ) " ;
    QString Create_Table_Securite="CREATE TABLE IF NOT EXISTS Securite( "
                                 "id_securite INTEGER PRIMARY KEY ,"
                                 " mot_passe TEXT NOT NULL ,"
                                 " question_securite TEXT NOT NULL ,"
                                 " reponce_question_securite TEXT NOT NULL , "
                                 "  FOREIGN KEY(id_securite) REFERENCES Utilisateur(id_utilisateur))" ;

    QString Create_Table_Status="CREATE TABLE IF NOT EXISTS Status("
                                  "id_status INTEGER PRIMARY KEY ,"
                                  "status TEXT DEFAULT 'OFF' ,"
                                  "last_seen DATETIME ,"
                                  "FOREIGN KEY(id_status) REFERENCES Utilisateur(id_utilisateur) );" ;

    QString Create_Table_Messages="CREATE TABLE IF NOT EXISTS Message("
                                    "id_message INTEGER PRIMARY KEY AUTOINCREMENT ,"
                                    "sender_id INTEGER NOT NULL ,"
                                    "receiver_id INTEGER NOT NULL ,"
                                    "content TEXT NOT NULL ,"
                                    "send_time DATETIME ,"
                                    "message_read BOOLEAN DEFAULT 0 ,"
                                    "emoji TEXT ,"
                                    "FOREIGN KEY(sender_id) REFERENCES Utilisateur(id_utilisateur),"
                                    "FOREIGN KEY(receiver_id) REFERENCES Utilisateur(id_utilisateur) );";

    QString Create_Table_Amis="CREATE TABLE IF NOT EXISTS Ami("
                                    "id_senderInvitation INTEGER NOT NULL ,"
                                    "id_ami INTEGER NOT NULL ,"
                                    "date_add DATETIME ,  "
                                    "status INTEGER DEFAULT 0 , " // STATUS DE L'amitie si confirme status=1
                                    "PRIMARY KEY(id_senderInvitation,id_ami),"
                                    "FOREIGN KEY(id_senderInvitation) REFERENCES Utilisateur(id_utilisateur) ,"
                                    "FOREIGN KEY(id_ami) REFERENCES Utilisateur(id_utilisateur));" ;


    QString Create_Table_Invitation="CREATE TABLE IF NOT EXISTS Invitation("
                                    "id_invitation INTEGER PRIMARY KEY AUTOINCREMENT , "
                                    "id_sender INTEGER , "
                                    "id_otherUser INTEGER , "
                                    "date_add DATETIME , "
                                    "FOREIGN KEY(id_sender) REFERENCES Utilisateur(id_utilisateur) ,"
                                    "FOREIGN KEY(id_otherUser) REFERENCES Utilisateur(id_utilisateur)) ; " ;


    QSqlQuery Query ;

    Query.exec(Create_Table_Utilisateur) ;
    Query.exec(Create_Table_Securite) ;
    Query.exec(Create_Table_Status) ;
    Query.exec(Create_Table_Messages) ;
    Query.exec(Create_Table_Amis) ;
    Query.exec(Create_Table_Invitation) ;

    seedDatabase();

}



void DataBase::seedDatabase()
{
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return;
    }

    QSqlQuery query;
    if (!query.exec("SELECT COUNT(*) FROM Utilisateur")) {
        qDebug() << "Impossible de vérifier le seed :" << query.lastError().text();
        return;
    }

    // if (query.next() && query.value(0).toInt() > 0) {
        // const QList<QPair<QString, QString>> profileMigrations = {
        //     {"yassine@example.com", ":/icons/users/user1.jpg"},
        //     {"sara@example.com", ":/icons/users/user2.jpg"},
        //     {"amine@example.com", ":/icons/users/user3.jpg"},
        //     {"salma@example.com", ":/icons/users/user4.jpg"},
        //     {"omar@example.com", ":/icons/users/user5.jpg"},
        //     {"lina@example.com", ":/icons/users/user6.jpg"},
        //     {"mehdi@example.com", ":/icons/users/user7.jpg"}
        // };

        // query.prepare(
        //     "UPDATE Utilisateur SET photo_profil=? "
        //     "WHERE email=? AND (photo_profil IS NULL OR photo_profil='' "
        //     "OR photo_profil LIKE 'images/profiles/%')");
        // for (const auto &profile : profileMigrations) {
        //     query.bindValue(0, profile.second);
        //     query.bindValue(1, profile.first);
        //     if (!query.exec()) {
        //         qDebug() << "Migration des photos impossible :" << query.lastError().text();
        //         return;
        //     }
        // }

        // qDebug() << "Seed ignoré : la table Utilisateur contient déjà des données.";
        // return;
    // }
    if (query.next() && query.value(0).toInt() > 0) {
        qDebug() << "Seed ignoré : la table Utilisateur contient déjà des données.";
        return;
    }

    QFile seedFile(":/seed.sql");
    if (!seedFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Impossible de lire seed.sql :" << seedFile.errorString();
        return;
    }

    if (!db.transaction()) {
        qDebug() << "Impossible de démarrer la transaction du seed :" << db.lastError().text();
        return;
    }

    QStringList sqlLines;
    for (const QString &line : QString::fromUtf8(seedFile.readAll()).split('\n')) {
        if (!line.trimmed().startsWith("--")) {
            sqlLines.append(line);
        }
    }

    const QStringList statements = sqlLines.join('\n').split(';', Qt::SkipEmptyParts);
    for (const QString &statement : statements) {
        const QString sql = statement.trimmed();
        if (sql.isEmpty() || sql.compare("BEGIN TRANSACTION", Qt::CaseInsensitive) == 0 ||
            sql.compare("COMMIT", Qt::CaseInsensitive) == 0) {
            continue;
        }

        if (!query.exec(sql)) {
            qDebug() << "Erreur seed :" << query.lastError().text();
            db.rollback();
            return;
        }
    }

    if (!db.commit()) {
        qDebug() << "Impossible de valider le seed :" << db.lastError().text();
        return;
    }

    qDebug() << "Seed effectué avec succès !";
}




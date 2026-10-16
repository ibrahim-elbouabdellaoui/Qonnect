#include "verification_input.h"
#include<QRegularExpression>

bool Verification_Nom_prenom(QLineEdit *LineEdit){
    QString texte=LineEdit->text() ;
    if(texte.isEmpty()) return false ;
    if(texte.length()<2 || texte.length()>30) return false ;
    QRegularExpression RegularEx("^[A-Za-z\\-]+$") ;
    if(!RegularEx.match(texte).hasMatch()) return false ;

    return true ;
}

bool Verification_Adresse(QLineEdit *LineEdit){
    QString texte=LineEdit->text() ;
    if(texte.length()<5 || texte.length()>50) return false ;
    QRegularExpression RegularEx("^[A-Za-z0-9 ,.-]") ;
    if(!RegularEx.match(texte).hasMatch()) return false ;

    return true ;

}


bool Verification_NumTele(QLineEdit *LineEdit){
   QString texte=LineEdit->text() ;
   /* if(texte.length()!=10)    return false ;
    else return true ;
    if(texte[0]!='0') return false ;
    if(texte[1]!='6' && texte[0]!='7') return false ;

    return true ;*/

   QRegularExpression RegularEx("^0[6 7][0-9]{8}$");
   if(RegularEx.match(texte).hasMatch()) return true ;

   return false ;

}

bool Verification_Mot_De_Passe(QLineEdit *LineEdit){
    QString texte=LineEdit->text();
    if(texte.length()<8) return false ;
    int Password_Contient_Numero=0 ;
    int Password_Contient_Majuscul=0 ;
    int Password_Contient_Miniscul=0 ;
    int Password_Contient_Caractere=0 ;
    for(int i=0;i<texte.length();i++){
        if(texte[i]>='0' && texte[i]<='9') Password_Contient_Numero++ ;
        else if(texte[i]>='A' && texte[i]<='Z') Password_Contient_Majuscul++ ;
        else if(texte[i]>='a' && texte[i]<='z') Password_Contient_Miniscul++ ;
        else Password_Contient_Caractere++ ;
    }

    if(Password_Contient_Numero && Password_Contient_Miniscul && Password_Contient_Majuscul && Password_Contient_Caractere)
        return true ;
}


bool Verification_Correspondane_Input(QLineEdit *LineEDit1,QLineEdit *LineEdit2){
    QString texte1=LineEDit1->text() ;
    QString texte2=LineEdit2->text() ;
    if(texte1==texte2) return true ;
    return false ;

}

bool Verification_Age_Annee(QComboBox *ComboBox){
    if(ComboBox->currentText().toInt()<6)
        return false ;

    return true ;
}

bool Verification_AdresseEmail(QLineEdit *LineEdit){

    QString texte=LineEdit->text() ;
    qDebug()<<texte ;
    QRegularExpression RegularEx("^[a-zA-Z0-9_.-]+@[a-zA-Z0-9-.]+\\.[a-zA-Z0-9-.]+$") ;
    return RegularEx.match(texte).hasMatch() ;

}











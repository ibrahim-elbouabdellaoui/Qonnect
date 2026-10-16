#ifndef VERIFICATION_INPUT_H
#define VERIFICATION_INPUT_H
#include<QLineEdit>
#include<QComboBox>

bool Verification_Nom_prenom(QLineEdit *) ;
bool Verification_Adresse(QLineEdit *) ;
bool Verification_NumTele(QLineEdit *) ;
bool Verification_AdresseEmail(QLineEdit *) ;
bool Verification_Mot_De_Passe(QLineEdit *) ;
bool Verification_Correspondane_Input(QLineEdit *,QLineEdit *) ;
bool Verification_Age_Annee(QComboBox *) ;
#endif // VERIFICATION_INPUT_H

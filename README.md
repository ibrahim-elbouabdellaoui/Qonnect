# Qonnect

Qonnect est une application de messagerie de bureau développée en C++ avec Qt. Elle propose une interface graphique Qt Widgets, une authentification locale et une persistance des utilisateurs, contacts et messages dans une base SQLite.

## Fonctionnalités

- Création de compte en plusieurs étapes : profil, coordonnées, sécurité et confirmation.
- Authentification locale par adresse e-mail et mot de passe.
- Recherche d'utilisateurs et gestion des invitations d'amitié.
- Acceptation ou refus des invitations et affichage de la liste d'amis.
- Envoi et consultation de messages enregistrés localement.
- Affichage des conversations, des messages non lus et des indicateurs de lecture.
- Réactions avec des images d'emojis intégrées aux ressources Qt.
- Gestion du profil : informations personnelles et photo de profil.
- Statut en ligne/hors ligne et dernière activité.

Les classes `Server` et `ChatClient` contiennent également des composants TCP basés sur Qt Network pour des notifications. Le code de l'interface utilisateur inspecté utilise toutefois principalement la base SQLite et des signaux périodiques locaux ; ce dépôt ne présente donc pas la messagerie réseau temps réel comme une fonctionnalité garantie.

## Technologies

- C++17
- Qt 6 ou Qt 5
- Qt Widgets
- Qt SQL avec SQLite
- Qt Network
- CMake
- Qt Resource System (`resources.qrc`)

Le projet utilise `CMAKE_AUTOUIC`, `CMAKE_AUTOMOC` et `CMAKE_AUTORCC`. Le fichier `CMakeLists.txt` configure les modules Qt `Widgets`, `Sql` et `Network`.

## Prérequis

- Un compilateur C++ compatible C++17.
- CMake 3.16 ou une version plus récente.
- Qt 6 ou Qt 5 avec les modules suivants :
  - Qt Widgets
  - Qt SQL
  - Qt Network
- Le pilote SQLite de Qt, généralement fourni avec l'installation Qt.

## Compilation avec CMake

Depuis la racine du projet :

```bash
cmake -S . -B build
cmake --build build --config Release
```

Sous Windows avec une configuration Visual Studio ou multi-configuration, l'exécutable se trouve généralement dans un sous-dossier de `build`, par exemple `build/Release/`.

Pour une compilation en mode Debug :

```bash
cmake --build build --config Debug
```

Le dossier `build/` est généré localement et est exclu du contrôle de version par `.gitignore`.

## Compilation avec Qt Creator

1. Ouvrir `CMakeLists.txt` dans Qt Creator.
2. Sélectionner un kit configuré avec Qt 5 ou Qt 6, un compilateur C++17 et CMake.
3. Configurer le projet dans un dossier de build distinct du dossier source.
4. Construire puis lancer la cible `ProjetQT`.

## Base de données et données de démonstration

Au démarrage, `main.cpp` initialise la classe `DataBase`. Celle-ci crée les tables SQLite nécessaires et tente de charger `seed.sql` lorsque la table `Utilisateur` est vide.

La base locale `Chat_App.db` est créée dans le dossier de l'exécutable. Elle est ignorée par Git afin d'éviter de publier des données d'exécution ou de test.

Le fichier `seed.sql` est embarqué comme ressource Qt et reste nécessaire à la construction. Il contient des données de démonstration, notamment des noms, des adresses e-mail, des numéros de téléphone et des mots de passe de test. Avant de rendre le dépôt public, ces données doivent être vérifiées et, si nécessaire, remplacées par des valeurs neutres ou retirées selon l'objectif du dépôt. Aucune base SQLite locale ne doit être publiée.

Si une base existe déjà et contient des utilisateurs, le seed est ignoré. Pour repartir avec les données de démonstration, il faut supprimer uniquement la base locale `Chat_App.db` du dossier de l'exécutable, puis relancer l'application.

## Organisation principale

- `main.cpp` : initialisation de l'application et de la base.
- `mainwindow.*` : fenêtre principale et navigation entre connexion, inscription et interface utilisateur.
- `loginwindow.*` : écran de connexion.
- `SignUp.*` : parcours de création de compte.
- `database.*` : connexion SQLite, création du schéma et chargement du seed.
- `recuperer_database.*` : lecture des utilisateurs, messages, contacts et invitations.
- `inserer_database.*` : insertion et mise à jour des données.
- `interface_user.*` : interface principale après connexion.
- `message.*` : affichage des messages et réactions emoji.
- `profil.*` : affichage et modification du profil.
- `zone_recherche.*` : recherche d'utilisateurs.
- `carte_top.*` : affichage du contact actif et de son statut.
- `resources.qrc` : images, icônes, photos de profil, emojis et `seed.sql`.


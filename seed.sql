BEGIN TRANSACTION;

--------------------------------------------------
-- 1. UTILISATEURS
--------------------------------------------------

INSERT INTO Utilisateur
(prenom, nom, date_naissance, adresse, genre, email,
 numero_telephone, photo_profil, status, last_seen)
VALUES
(
    'Yassine',
    'Alaoui',
    '2002-05-12',
    'Rabat',
    'M',
    'yassine@example.com',
    '0600000001',
    ':/icons/users/user1.jpg',
    1,
    '2026-10-15 17:05:00'
),
(
    'Sara',
    'Bennani',
    '2003-08-21',
    'Casablanca',
    'F',
    'sara@example.com',
    '0600000002',
    ':/icons/users/user2.jpg',
    1,
    '2026-10-15 17:04:00'
),
(
    'Amine',
    'Idrissi',
    '2001-11-03',
    'Fès',
    'M',
    'amine@example.com',
    '0600000003',
    ':/icons/users/user3.jpg',
    1,
    '2026-10-15 16:58:00'
),
(
    'Salma',
    'Tazi',
    '2004-02-18',
    'Marrakech',
    'F',
    'salma@example.com',
    '0600000004',
    ':/icons/users/user4.jpg',
    0,
    '2026-10-15 14:35:00'
),
(
    'Omar',
    'Fassi',
    '2002-07-09',
    'Tanger',
    'M',
    'omar@example.com',
    '0600000005',
    ':/icons/users/user5.jpg',
    1,
    '2026-10-15 17:00:00'
),
(
    'Lina',
    'Amrani',
    '2003-04-27',
    'Rabat',
    'F',
    'lina@example.com',
    '0600000006',
    ':/icons/users/user6.jpg',
    0,
    '2026-10-15 12:45:00'
),
(
    'Mehdi',
    'Karim',
    '2001-09-15',
    'Casablanca',
    'M',
    'mehdi@example.com',
    '0600000007',
    ':/icons/users/user7.jpg',
    1,
    '2026-10-15 17:02:00'
);


--------------------------------------------------
-- 2. SECURITE
--------------------------------------------------

INSERT INTO Securite
(id_securite, mot_passe, question_securite, reponce_question_securite)

SELECT id_utilisateur,
       'DemoPass1!',
       'Quelle est votre ville natale ?',
       'Rabat'
FROM Utilisateur
WHERE email = 'yassine@example.com';


INSERT INTO Securite
(id_securite, mot_passe, question_securite, reponce_question_securite)

SELECT id_utilisateur,
       'DemoPass2!',
       'Quelle est votre ville natale ?',
       'Casablanca'
FROM Utilisateur
WHERE email = 'sara@example.com';


INSERT INTO Securite
(id_securite, mot_passe, question_securite, reponce_question_securite)

SELECT id_utilisateur,
       'DemoPass3!',
       'Quelle est votre ville natale ?',
       'Fès'
FROM Utilisateur
WHERE email = 'amine@example.com';


INSERT INTO Securite
(id_securite, mot_passe, question_securite, reponce_question_securite)

SELECT id_utilisateur,
       'DemoPass4!',
       'Quelle est votre ville natale ?',
       'Marrakech'
FROM Utilisateur
WHERE email = 'salma@example.com';


INSERT INTO Securite
(id_securite, mot_passe, question_securite, reponce_question_securite)

SELECT id_utilisateur,
       'DemoPass5!',
       'Quelle est votre ville natale ?',
       'Tanger'
FROM Utilisateur
WHERE email = 'omar@example.com';


INSERT INTO Securite
(id_securite, mot_passe, question_securite, reponce_question_securite)

SELECT id_utilisateur,
       'DemoPass6!',
       'Quelle est votre ville natale ?',
       'Rabat'
FROM Utilisateur
WHERE email = 'lina@example.com';


INSERT INTO Securite
(id_securite, mot_passe, question_securite, reponce_question_securite)

SELECT id_utilisateur,
       'DemoPass7!',
       'Quelle est votre ville natale ?',
       'Casablanca'
FROM Utilisateur
WHERE email = 'mehdi@example.com';


--------------------------------------------------
-- 3. STATUS
--------------------------------------------------

INSERT INTO Status
(id_status, status, last_seen)

SELECT id_utilisateur,
       CASE
           WHEN status = 1 THEN 'ON'
           ELSE 'OFF'
       END,
       last_seen
FROM Utilisateur;


--------------------------------------------------
-- 4. CONVERSATION AVEC SARA
--------------------------------------------------

INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur,
       r.id_utilisateur,
       'Salut Sara ! Comment ça va ?',
       '2026-10-15 15:42:00',
       1,
       ':/icons/Emojis/like.svg'
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'yassine@example.com'
AND r.email = 'sara@example.com';


INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur,
       r.id_utilisateur,
       'Salut Yassine ! Ça va très bien 😊',
       '2026-10-15 15:43:00',
       1,
       ':/icons/Emojis/smile_smalleye.svg'
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'sara@example.com'
AND r.email = 'yassine@example.com';


INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur,
       r.id_utilisateur,
       'Tu as terminé le projet ?',
       '2026-10-15 15:46:00',
       1,
       ''
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'yassine@example.com'
AND r.email = 'sara@example.com';


INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur,
       r.id_utilisateur,
       'Presque ! Il me reste quelques détails.',
       '2026-10-15 15:48:00',
       1,
       ':/icons/Emojis/smile_smalleye.svg'
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'sara@example.com'
AND r.email = 'yassine@example.com';


INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur,
       r.id_utilisateur,
       'Moi aussi, je suis en train de finaliser l interface.',
       '2026-10-15 15:52:00',
       1,
       ''
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'yassine@example.com'
AND r.email = 'sara@example.com';


INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur,
       r.id_utilisateur,
       'Elle a l air vraiment bien !',
       '2026-10-15 15:54:00',
       1,
       ':/icons/Emojis/surprized.svg'
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'sara@example.com'
AND r.email = 'yassine@example.com';


INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur,
       r.id_utilisateur,
       'Merci ! Je veux encore améliorer la page des conversations.',
       '2026-10-15 16:01:00',
       1,
       ''
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'yassine@example.com'
AND r.email = 'sara@example.com';


-- NOUVEAUX MESSAGES NON LUS

INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur,
       r.id_utilisateur,
       'J ai une idée pour améliorer la page d accueil 👀',
       '2026-10-15 17:03:00',
       0,
       ':/icons/Emojis/surprized.svg'
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'sara@example.com'
AND r.email = 'yassine@example.com';


INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur,
       r.id_utilisateur,
       'Tu es disponible ce soir pour en discuter ?',
       '2026-10-15 17:04:00',
       0,
       ':/icons/Emojis/thinking.svg'
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'sara@example.com'
AND r.email = 'yassine@example.com';


--------------------------------------------------
-- 5. CONVERSATION AVEC AMINE
--------------------------------------------------

INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur, r.id_utilisateur,
       'Salut Amine, tu avances sur ton projet ?',
       '2026-10-15 13:20:00', 1, ':/icons/Emojis/like.svg'
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'yassine@example.com'
AND r.email = 'amine@example.com';


INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur, r.id_utilisateur,
       'Oui, ça avance bien !',
       '2026-10-15 13:24:00', 1, ':/icons/Emojis/like.svg'
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'amine@example.com'
AND r.email = 'yassine@example.com';


INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur, r.id_utilisateur,
       'Tu utilises toujours Qt ?',
       '2026-10-15 13:26:00', 1, ''
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'yassine@example.com'
AND r.email = 'amine@example.com';


INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur, r.id_utilisateur,
       'Oui, avec SQLite pour la base de données.',
       '2026-10-15 13:29:00', 1, ':/icons/Emojis/thinking.svg'
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'amine@example.com'
AND r.email = 'yassine@example.com';


INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur, r.id_utilisateur,
       'Excellent choix !',
       '2026-10-15 13:31:00', 1, ':/icons/Emojis/heart.svg'
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'amine@example.com'
AND r.email = 'yassine@example.com';


--------------------------------------------------
-- 6. CONVERSATION AVEC SALMA
--------------------------------------------------

INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur, r.id_utilisateur,
       'Bonjour Salma !',
       '2026-10-14 18:10:00', 1, ':/icons/Emojis/smile_smalleye.svg'
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'yassine@example.com'
AND r.email = 'salma@example.com';


INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur, r.id_utilisateur,
       'Coucou Yassine !',
       '2026-10-14 18:12:00', 1, ':/icons/Emojis/like.svg'
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'salma@example.com'
AND r.email = 'yassine@example.com';


INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur, r.id_utilisateur,
       'Tu as vu le nouveau design ?',
       '2026-10-14 18:15:00', 1, ''
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'yassine@example.com'
AND r.email = 'salma@example.com';


INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur, r.id_utilisateur,
       'Oui, il est beaucoup plus moderne.',
       '2026-10-14 18:18:00', 1, ':/icons/Emojis/surprized.svg'
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'salma@example.com'
AND r.email = 'yassine@example.com';


INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur, r.id_utilisateur,
       'J aime surtout la nouvelle interface.',
       '2026-10-14 18:21:00', 1, ''
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'salma@example.com'
AND r.email = 'yassine@example.com';


INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur, r.id_utilisateur,
       'Oui, elle rend l application beaucoup plus agréable.',
       '2026-10-14 18:25:00', 1, ':/icons/Emojis/like.svg'
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'yassine@example.com'
AND r.email = 'salma@example.com';


--------------------------------------------------
-- 7. CONVERSATION AVEC OMAR
--------------------------------------------------

INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur, r.id_utilisateur,
       'Salut Omar ! Tu es disponible demain ?',
       '2026-10-15 11:30:00', 1, ''
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'yassine@example.com'
AND r.email = 'omar@example.com';


INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur, r.id_utilisateur,
       'Salut ! Oui normalement.',
       '2026-10-15 11:33:00', 1, ':/icons/Emojis/like.svg'
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'omar@example.com'
AND r.email = 'yassine@example.com';


INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur, r.id_utilisateur,
       'On peut travailler sur le projet ensemble.',
       '2026-10-15 11:36:00', 1, ''
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'yassine@example.com'
AND r.email = 'omar@example.com';


INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur, r.id_utilisateur,
       'Bonne idée !',
       '2026-10-15 11:38:00', 1, ':/icons/Emojis/surprized.svg'
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'omar@example.com'
AND r.email = 'yassine@example.com';


INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur, r.id_utilisateur,
       'Je t envoie les fichiers ce soir.',
       '2026-10-15 11:40:00', 1, ''
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'omar@example.com'
AND r.email = 'yassine@example.com';


--------------------------------------------------
-- 8. CONVERSATION AVEC LINA
--------------------------------------------------

INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur, r.id_utilisateur,
       'Bonjour Lina !',
       '2026-10-13 16:10:00', 1, ':/icons/Emojis/like.svg'
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'yassine@example.com'
AND r.email = 'lina@example.com';


INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur, r.id_utilisateur,
       'Bonjour Yassine 😊',
       '2026-10-13 16:12:00', 1, ':/icons/Emojis/smile_smalleye.svg'
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'lina@example.com'
AND r.email = 'yassine@example.com';


INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur, r.id_utilisateur,
       'Tu as reçu mon document ?',
       '2026-10-13 16:15:00', 1, ''
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'lina@example.com'
AND r.email = 'yassine@example.com';


INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur, r.id_utilisateur,
       'Oui, je viens de le recevoir.',
       '2026-10-13 16:17:00', 1, ':/icons/Emojis/like.svg'
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'yassine@example.com'
AND r.email = 'lina@example.com';


INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur, r.id_utilisateur,
       'Parfait, merci !',
       '2026-10-13 16:20:00', 1, ':/icons/Emojis/surprized.svg'
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'lina@example.com'
AND r.email = 'yassine@example.com';


--------------------------------------------------
-- 9. CONVERSATION AVEC MEHDI
--------------------------------------------------

INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur, r.id_utilisateur,
       'Salut Mehdi !',
       '2026-10-15 10:20:00', 1, ':/icons/Emojis/like.svg'
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'yassine@example.com'
AND r.email = 'mehdi@example.com';


INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur, r.id_utilisateur,
       'Salut Yassine ! Ça va ?',
       '2026-10-15 10:22:00', 1, ':/icons/Emojis/smile_smalleye.svg'
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'mehdi@example.com'
AND r.email = 'yassine@example.com';


INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur, r.id_utilisateur,
       'Très bien. Tu travailles aujourd hui ?',
       '2026-10-15 10:25:00', 1, ''
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'yassine@example.com'
AND r.email = 'mehdi@example.com';


INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur, r.id_utilisateur,
       'Oui, je suis sur une nouvelle fonctionnalité.',
       '2026-10-15 10:28:00', 1, ':/icons/Emojis/thinking.svg'
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'mehdi@example.com'
AND r.email = 'yassine@example.com';


INSERT INTO Message
(sender_id, receiver_id, content, send_time, message_read, emoji)

SELECT s.id_utilisateur, r.id_utilisateur,
       'Bon courage alors ! 💪',
       '2026-10-15 10:30:00', 1, ':/icons/Emojis/like.svg'
FROM Utilisateur s, Utilisateur r
WHERE s.email = 'mehdi@example.com'
AND r.email = 'yassine@example.com';


--------------------------------------------------
-- 10. AMITIES
--------------------------------------------------

INSERT INTO Ami
(id_senderInvitation, id_ami, date_add, status)

SELECT a.id_utilisateur,
       b.id_utilisateur,
       '2026-10-01 10:00:00',
       1
FROM Utilisateur a, Utilisateur b
WHERE a.email = 'yassine@example.com'
AND b.email = 'sara@example.com';


INSERT INTO Ami
(id_senderInvitation, id_ami, date_add, status)

SELECT a.id_utilisateur,
       b.id_utilisateur,
       '2026-10-02 10:00:00',
       1
FROM Utilisateur a, Utilisateur b
WHERE a.email = 'yassine@example.com'
AND b.email = 'amine@example.com';


INSERT INTO Ami
(id_senderInvitation, id_ami, date_add, status)

SELECT a.id_utilisateur,
       b.id_utilisateur,
       '2026-10-03 10:00:00',
       1
FROM Utilisateur a, Utilisateur b
WHERE a.email = 'yassine@example.com'
AND b.email = 'salma@example.com';


INSERT INTO Ami
(id_senderInvitation, id_ami, date_add, status)

SELECT a.id_utilisateur,
       b.id_utilisateur,
       '2026-10-04 10:00:00',
       1
FROM Utilisateur a, Utilisateur b
WHERE a.email = 'yassine@example.com'
AND b.email = 'omar@example.com';


INSERT INTO Ami
(id_senderInvitation, id_ami, date_add, status)

SELECT a.id_utilisateur,
       b.id_utilisateur,
       '2026-10-05 10:00:00',
       1
FROM Utilisateur a, Utilisateur b
WHERE a.email = 'yassine@example.com'
AND b.email = 'lina@example.com';


INSERT INTO Ami
(id_senderInvitation, id_ami, date_add, status)

SELECT a.id_utilisateur,
       b.id_utilisateur,
       '2026-10-06 10:00:00',
       1
FROM Utilisateur a, Utilisateur b
WHERE a.email = 'yassine@example.com'
AND b.email = 'mehdi@example.com';


--------------------------------------------------
-- 11. UNE INVITATION EN ATTENTE
--------------------------------------------------

INSERT INTO Invitation
(id_sender, id_otherUser, date_add)

SELECT a.id_utilisateur,
       b.id_utilisateur,
       '2026-10-15 16:45:00'
FROM Utilisateur a, Utilisateur b
WHERE a.email = 'mehdi@example.com'
AND b.email = 'yassine@example.com';


COMMIT;
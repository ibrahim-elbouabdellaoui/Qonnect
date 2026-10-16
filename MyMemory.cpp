#include "MyMemory.h"
#include <QtNetwork/QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QtNetwork/QNetworkAccessManager>
#include <QEventLoop>
#include <QtNetwork/QNetworkReply>
#include <QUrlQuery>
#include <QDebug>

MyMemory::MyMemory() {
    API_URL = "https://api.mymemory.translated.net/get";
}

QString MyMemory::Traduire(const QString& message,const QString& langue_original,const QString& langue_cible) {
    QUrl url(API_URL);

    // Langue source = auto détectée
    QString sourceLang = langue_original;
    QString targetLang = langue_cible;

    // Construction de l'URL avec les paramètres GET
    QUrlQuery query;
    query.addQueryItem("q", message);
    query.addQueryItem("langpair", sourceLang + "|" + targetLang);
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "QtApp");

    static QNetworkAccessManager manager;
    QNetworkReply* reply = manager.get(request);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Erreur réseau:" << reply->errorString();
        reply->deleteLater();
        return "Erreur de traduction";
    }

    QByteArray responseData = reply->readAll();
    reply->deleteLater();

    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(responseData, &jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        qDebug() << "Erreur JSON:" << jsonError.errorString();
        return "Erreur JSON";
    }

    QJsonObject root = doc.object();
    QJsonObject responseDataObj = root["responseData"].toObject();
    QString translatedText = responseDataObj["translatedText"].toString();

    if (translatedText.isEmpty()) {
        return "Traduction vide";
    }

    qDebug() << "Traduction:" << translatedText;
    return translatedText;
}

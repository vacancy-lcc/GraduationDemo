#ifndef TEXTTOSOUR_H
#define TEXTTOSOUR_H
#include <QJsonParseError>
#include <QFile>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QByteArray>
#include <QDebug>
#include <QIODevice>
#include <QObject>
class texttosour:public QObject
{
    Q_OBJECT
public:

    texttosour(QObject *parent=Q_NULLPTR)
        :QObject(parent)
    {
        //connect(reply, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
    }

    void sour_requerse(QString str);
    //void readyReadSlot();
private:
    QString m_apiKey;
    QString  m_secretKey;
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QNetworkReply *reply;
    //QString str;
    QByteArray *retvalue;
private slots:
    void readyReadSlot();

signals:
    void finish();

};

#endif // TEXTTOSOUR_H

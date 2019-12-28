#include "recocard.h"


recoCard::recoCard(QObject *parent)
    :QObject(parent),
      status(-1)
{
    manager = new QNetworkAccessManager;
    QUrl url("http://anpr.sinosecu.com.cn/api/recogliu.do");

    request.setUrl(url);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded; charset=utf-8");
    request.setRawHeader("Authorization", "APPCODE b31aa4d6e32a4a8abed9a26ab07b510f");

}

recoCard::recoCard(const QString &img, QObject *parent)
    :QObject(parent),
    img(img), status(-1)

{
    manager = new QNetworkAccessManager;
    QUrl url("http://anpr.sinosecu.com.cn/api/recogliu.do");

    request.setUrl(url);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded; charset=utf-8");
    request.setRawHeader("Authorization", "APPCODE b31aa4d6e32a4a8abed9a26ab07b510f");


}


void recoCard::start()
{
    QFile f(img);
    if(!f.open(QIODevice::ReadWrite))
    {
//        QMessageBox::information(this, "错误", f.errorString(), QMessageBox::Ok);
        qDebug() << f.errorString();
    }
    QByteArray hexed =  f.readAll().toBase64();
    hexed.push_front("\r\n\r\nimg=");

    reply = manager->post(request, hexed);
    connect(reply, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));

    f.close();
}

recoCard::~recoCard()
{
    if(!manager)
        delete manager;
}

void recoCard::readyReadSlot()
{
    qDebug() << __LINE__;
    retvalue = 	reply->readAll();	//获得返回的json数据

    QJsonParseError Err;
    QJsonDocument document =  QJsonDocument::fromJson(retvalue.toUtf8(), &Err);
    if(!(Err.error == QJsonParseError::NoError))
    {
        qDebug()<<tr("解析json文件错误！");
        return;
    }

    QJsonObject  JObject= document.object();

    for(auto it = JObject.begin(); it != JObject.end(); ++it)
    {
        if(it.value().isObject())
        {
            QJsonObject statObject =  it.value().toObject();

            /* 获得查询状态信息 */
            status = statObject.value("status").toInt();
//            qDebug() << "status:"<<status;

            if(status == -1)
            {
                _cardid.clear();
                _color.clear();

                emit readyJson();
                return;
            }
            //status.value("value").toString();

        }
        if(it.value().isArray())
        {
            QJsonArray cardinfo = it.value().toArray().first().toObject().value("items").toArray();

            /* 获得车牌号、车牌颜色 */
            _cardid =  cardinfo[0].toObject().value("content").toString();
            _color =  cardinfo[1].toObject().value("content").toString();

        }
    }

    emit readyJson();

}

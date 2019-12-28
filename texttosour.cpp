#include "texttosour.h"
void texttosour::sour_requerse(QString str)
{
    //connect(reply, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
    manager = new QNetworkAccessManager;
    //QString tmp;
    QUrl url(QString("http://tsn.baidu.com/text2audio?&"
             "lan=zh&cuid=swbz1111&ctp=1&"
             "tok=24.c62403e9bf67cdc9efc1ae326e548e14.2592000.1575425388.282335-17324165&"
             "pit=8&per=3&aue=3&tex=%1").arg(str));
    request.setUrl(url);

    reply = manager->get(request);
    connect(reply, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));

}
void texttosour::readyReadSlot()
{
    qDebug() << __LINE__;

    //retvalue = 	reply->readAll();	//获得返回的json数据

    QFile wavfile("a.mp3");
    wavfile.open(QIODevice::WriteOnly);
    //wavfile.write(reply->readAll());
    QByteArray buf = reply->readAll();
    //QString tmp = buf;
    wavfile.write(buf);

    wavfile.close();
    emit finish();
    //qDebug()<<buf;

}

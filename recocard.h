#ifndef RECOCARD_H
#define RECOCARD_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QByteArray>
#include <QDebug>
#include <QIODevice>
#include <QFile>
#include <QMessageBox>
#include <QString>
#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonParseError>

/**
    1.生成对象
    rc = new recoCard;
    2.设置识别图片
    rc->setImg("../a.jpg");
    3.关联信号与槽函数
    connect(rc, SIGNAL(readyJson()), this, SLOT(readJsonSlot()));
    4.开始识别
    rc->start();
    5.设置槽函数读取车牌信息
    readJsonSlot(){}
 */

class recoCard : public QObject
{
    Q_OBJECT
public:
	/* 构造函数 */
    recoCard(const QString &img, QObject *parent = Q_NULLPTR);
    recoCard(QObject *parent = Q_NULLPTR);

    /* 设置要识别的图片路径（名字）*/
    void setImg(const QString &img){this->img = img;}

    /* 开始识别 */
    void start();

    QString cardid() const{return _cardid;}		//获得车牌号
    QString color() const{return _color;}		//获得车牌颜色
    QString imgName() const {return img;}		//获得图片的名字
    QString retValue() const {return retvalue;}	//获得全部JSON数据

    bool isRecoSuccess() const {				//获得识别是否成功的状态
        if(status == 0)
            return true;
        else
            return false;
    }

    void readJsom(){}
    ~recoCard();
private:

    QString img;
    int status;
    QString retvalue;

    QString _cardid;
    QString _color;


    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QNetworkReply *reply;

private slots:
    void readyReadSlot();

signals:
    /* 解析图片信息完毕 */
    void readyJson();
};

#endif // RECOCARD_H

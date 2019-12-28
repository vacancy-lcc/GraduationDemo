#ifndef WORKER_H
#define WORKER_H


#include <QThread>
#include <QProgressBar>
#include <QString>
#include <QLabel>
#include <QWidget>
#include <QObject>
#include "iso14443a.h"
#include "rfid.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>
#include <QFile>
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <recocard.h>
#include "texttosour.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

class worker : public QThread//语音播报线程
{
public:
    explicit worker();
    ~worker();
    void set_str(QString str){this->str = str;}
    void run() override;
private:
    QString str;
};

class videoworker : public QThread//视频输出线程
{
    Q_OBJECT

public:
    explicit videoworker(QLabel *label,int &id);
    ~videoworker();
    void run() override;

    bool flag;//该标志为真则拍照保存
    int id;//入库时产生的cardid
    bool stop_flag;//该标志为真则暂停睡眠
    bool destroy;
private:
    QLabel *label;
    recoCard *reco;

    // 摄像头
    cv::VideoCapture cap;

    void cvVideoShow(void);

signals:
    void sendjpgpath(char *path,QString _carid);
    void reco_start();
public slots:
    void begin_reco(){reco->start();}
    void readJsonSlot();
};


struct RFID
{
    int fd;
    bool flags;
    int x;
    RFID(int fd, int _x) : fd(fd),flags(true) , x(_x){}
};

class rfidWorker : public QThread
{
    Q_OBJECT

private:
//    int infd;
    videoworker *video;
    QSqlQuery *query;

//    bool outFlags = true;
//    bool inFlags = true;
    RFID inRFID;
    RFID outRFID;
    int innum;
    int outnum;


public:
    explicit rfidWorker(int infd, int outfd, videoworker *video, QSqlQuery *query);
     void detectCard(RFID &fd);

    void getCardNumber(RFID &fd);
    void outCardNumber(RFID &fd);
    void run() override;


signals:
    void flushSql();
    void sendcash(int cash);
    void sendcardid(int id);
};

#endif // WORKER_H

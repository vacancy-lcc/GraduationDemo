#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "iso14443a.h"
#include <QTimer>
#include "mysqlmodel.h"
#include "worker.h"
#include "recocard.h"
#include "texttosour.h"
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);

    friend class rfidWorker;

    ~Widget();

private slots:
    void on_pushButtonUART_clicked(bool checked);
    void detectCard();
    void getCardNumber();

    void getoutCardNumber();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void onflushSql();

    void getcardid(int cardid);
    void getjpgpath(char *path,QString _carid);
    void getcash(int cash);

    void sour_display();
signals:
    void onDetected();
    void onDetectedout();

private:
    void initTTY();
    void closeTTY();

    Ui::Widget *ui;
    QTimer t;

    //数据库相关
    mysqlmodel *sqlmodel;
//    QSqlDatabase db;
    QSqlQuery *query;


    int infd;//入库文件描述符
    int outfd;//出库文件描述符

    videoworker *video;//视频监控线程
    worker *sour;//语音播放线程
    bool flag;
    QLabel *label;
    int id;
    bool flagxx;
    rfidWorker *rfidThread;
    int tmpid = 0;
    QMessageBox *message;
    texttosour *text2sour;
    //recoCard *reco;
};

#endif // WIDGET_H

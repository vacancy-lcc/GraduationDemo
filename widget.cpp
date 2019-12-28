#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QDebug>
#include "rfid.h"
#include "mysqlmodel.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTime>
#include <QString>
#include "worker.h"
#include <QWaitCondition>
#include "recocard.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    rfidThread  = nullptr;
    // 使得按钮可以被按下
    sqlmodel = new mysqlmodel(this);
    label = new QLabel(this);
    label->setFixedHeight(480);
    label->setFixedWidth(640);
    label->move(160,0);
    id = 0;
    ui->tableView->setModel(sqlmodel);

    //reco = new recoCard(this);


    QSqlDatabase db =  QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("card.db");
    if(!db.open())
    {
        qDebug()<<"error info :"<<db.lastError();
        exit(1);
    }
    //sqlmodel->setQuery("SELECT * FROM card");

    //sqlmodel->setQuery();
    query = new QSqlQuery(db);
    query->exec("CREATE TABLE card(cardid INTEGER PRIMARY KEY NOT NULL, time TEXT,path TEXT,"
                "licence TEXT)");
    query->exec();
    sqlmodel->setQuery("SELECT * FROM card");
    sqlmodel->setHeaderData(0, Qt::Horizontal, "卡号");
    sqlmodel->setHeaderData(1, Qt::Horizontal, "时间");
    sqlmodel->setHeaderData(2, Qt::Horizontal, "路径");
    sqlmodel->setHeaderData(3, Qt::Horizontal, "车牌");

    ui->pushButtonUART->setCheckable(true);
    text2sour = new texttosour;
    flag = false;
    video = new videoworker(label,id);
    sour = new worker;
    video->start();

    ui->pushButton_2->setText("暂停录像");
//    connect(&t, SIGNAL(timeout()), this, SLOT(detectCard()));
    connect(this, SIGNAL(onDetected()), this, SLOT(getCardNumber()));

    connect(this, SIGNAL(onDetectedout()), this, SLOT(getoutCardNumber()));

    //connect(rfidThread,SIGNAL(sendcardid(int)),this,SLOT(getcardid(int)));
    //connect(video,SIGNAL(sendjpgpath(char*)),this,SLOT(getjpgpath(char*)));
    message = new QMessageBox(this);
    connect(text2sour,SIGNAL(finish()),this,SLOT(sour_display()));
}

Widget::~Widget()
{
    delete sour;
    //delete sqlmodel;
    delete ui;
    delete rfidThread;
    delete video;
    delete query;
    delete text2sour;
}
void Widget::getcash(int cash)
{
    //message = new;
    QString tmp = QString("收费 %1 元").arg(cash);
    message->setText(tmp);
    sour->set_str(tmp);
    text2sour->sour_requerse(tmp);
    //sour->set_str("this is getcash");
    //sour->start();
    message->exec();

    qDebug()<<"this is massagebox";
}
void Widget::getcardid(int cardid)
{
    //QString tmp = QString("insert into card(cardid,time,Yes) values('%1', '%2', 1)").arg(cardid).arg(time(NULL));
    //query->exec(tmp);
    tmpid = cardid;
    qDebug()<<"this is widget"<<cardid;

}
void  Widget::getjpgpath(char *path,QString _cardid)
{
    QString tmp = QString("insert into card(cardid,time,path,licence) "
                          "values('%1', '%2','%3', '%4')").arg(tmpid).arg(time(NULL)).arg(path).arg(_cardid);
    query->exec(tmp);
    sqlmodel->setQuery("SELECT * FROM card");
    if(_cardid=="识别失败")
    {

        sour->set_str("车牌识别失败");
        //sour->set_str("this is getjpgpath");
        sour->start();
        message->setText("车牌识别失败");
        text2sour->sour_requerse("车牌识别失败");
        message->exec();
        return;
    }
    QString str = QString("欢迎%1进场").arg(_cardid);
    text2sour->sour_requerse(str);

    //sour->set_str(str);
    //sour->set_str("this is getjpgpath");
    //sour->start();
    //message = new QMessageBox(this);
    message->setText(str);

    message->exec();
    qDebug()<<"this is getjogpath";
    //sour->set_str(str);
    //sour->start();

}

void  Widget::sour_display()
{
    sour->start();
}

void Widget::on_pushButtonUART_clicked(bool checked)
{
    if(checked)
    {
        qDebug() << __LINE__;
        initTTY();
        if(!rfidThread)
            rfidThread = new rfidWorker(this->infd,this->outfd, this->video, query);
        rfidThread->start();

        connect(rfidThread, SIGNAL(flushSql()), this, SLOT(onflushSql()));
        connect(rfidThread,SIGNAL(sendcardid(int)),this,SLOT(getcardid(int)));
        connect(video,SIGNAL(sendjpgpath(char*,QString)),this,SLOT(getjpgpath(char*,QString)));
        connect(rfidThread,SIGNAL(sendcash(int)),this,SLOT(getcash(int)));
        // 开关当前处于按下的状态
        ui->pushButtonUART->setText("探测中...");
        /*
        // 禁用所有的输入框
        ui->ComboBoxBaud->setEnabled(false);
        ui->ComboBoxCOM->setEnabled(false);
        ui->ComboBoxDataBits->setEnabled(false);
        */
        // 开启定时器，不断探查卡片是否在能量范围之内
//        t.start(200);
    }
    else
    {
        qDebug() << __LINE__;
        //closeTTY();

        // 开关当前处于弹起的状态
        ui->pushButtonUART->setText("开始刷卡");
            /*
        // 启用所有的输入框
        ui->ComboBoxBaud->setEnabled(true);
        ui->ComboBoxCOM->setEnabled(true);
        ui->ComboBoxDataBits->setEnabled(true);
        */
        // 暂停定时器，不询问卡片的状态
//        t.stop();

        rfidThread->wait();

        closeTTY();
    }
}


void Widget::detectCard()
{
    init_REQUEST();
    char recvinfo[128];
//    char outvinfo[128];
    // 向串口发送指令

//    tcflush (infd, TCIFLUSH);
    write(infd, PiccRequest_IDLE, PiccRequest_IDLE[0]);

//    tcflush(outfd,TCIFLUSH);
//    write(outfd, PiccRequest_IDLE,PiccRequest_IDLE[0]);

    usleep(10*1000);

    bzero(recvinfo, 128);
    qDebug() << __LINE__;
    read(infd, recvinfo, 128);
//    qDebug() << __LINE__;

/*
    bzero(outvinfo, 128);
    read(outfd,outvinfo,128);

*/
    //应答帧状态部分为0 则请求成功
    if(recvinfo[2] == 0x00)
    {
        qDebug() << "detected success!";
        //flagxx = true;
        emit onDetected();
    }
    else
    {
        flagxx = true;
        qDebug() << "no card...";
    }


//    if(outvinfo[2] == 0x00)
//    {
//        qDebug() << "detected!";
//        //flagxx = true;
//        emit onDetectedout();
//    }
//    else
//    {
//        flagxx = true;
//        qDebug() << "no card...";
//    }


    sqlmodel->setQuery("SELECT * FROM card");
}

void Widget::getCardNumber()
{
    static uint32_t oldid, newid;

    // 获取附近卡片的卡号... ...
    oldid = (newid == 0xFFFFFFFF ? oldid : newid);
    newid = get_id(infd);

    qDebug() << "fetch card Number:" << newid;

    if(newid == 0 || newid == 0xFFFFFFFF)
    {
        //flagxx=true;
        return;
    }

    // flag为真意味着：卡片刚放上去
    if(flagxx)
    {
        if(newid == oldid)
        {
            beep(5, 0.05);
        }
        else
        {
            qDebug() << __LINE__;
//            QSqlDatabase db =  QSqlDatabase::addDatabase("QSQLITE");

//            QSqlQuery query(db);
//            QString tmp = QString("insert into card(cardid,time) values('%1', '%2')").arg(newid).arg(time(NULL));
//            query.exec(tmp);

            video->id = newid;
            video->flag = true;

            //query.prepare("INSERT INTO card(cardid,time)""VALUES(:cardid,:time)");
            //query.bindValue(":cardid",QString("%1").arg(newid));
            //query.bindValue(":time",current_time.toString());

//            sour=new worker(QString("%1").arg(newid));
//            sour->run();//音频播放线程

//            query.exec();
//            sqlmodel->fresh_info();
            beep(1, 0.3);
        }

        flagxx = false;
    }
    qDebug() << "refres alarm";
    //alarm(1);
}

//出库，删除相应卡号；
//创建线程播放音频；
void Widget::getoutCardNumber()
{
    static uint32_t oldid, newid;

    // 获取附近卡片的卡号... ...
    oldid = newid==0xFFFFFFFF?oldid:newid;
    newid = get_id(outfd);
    qDebug() << "fetch card Number:" << newid;
    if(newid == 0 || newid == 0xFFFFFFFF)
    {
        //flagxx=true;
        return;
    }

    // flag为真意味着：卡片刚放上去
    if(flagxx)
    {
        if(newid == oldid)
        {
            beep(5, 0.05);
        }
        else
        {
            //ui->labelCardNumber->setText(QString("%1").arg(newid));


            QSqlDatabase db =  QSqlDatabase::addDatabase("QSQLITE");

            //QTime current_time =QTime::currentTime();
            //根据卡号查找,先查看数据是否存在
            QString str = QString("select *from card where carid = '%1'").arg(newid);
            QSqlQuery query(db);
            query.exec(str);
            int tmpid=0;
            while(query.next())
            {
                tmpid = query.value(0).toInt();
            }

            if(newid == tmpid)
            {
                str =  QString("delete from card where carid = '%1'").arg(newid);
                query.exec(str);
            }


            //query.bindValue(":cardid",QString("%1").arg(newid));
            //query.bindValue(":time",current_time.toString());
            query.exec();
            sqlmodel->fresh_info();
            beep(1, 0.3);
        }

        flagxx = false;
    }
    qDebug() << "refres alarm";
    //alarm(1);
}

void Widget::closeTTY()
{
    ::close(infd);
    ::close(outfd);
}

void Widget::initTTY()
{
    // 根据下拉框选项初始化串口
    //const char *tty = QString("/dev/%1").arg(ui->ComboBoxCOM->currentText()).toStdString().c_str();
    infd = open("/dev/ttySAC2", O_RDWR | O_NOCTTY);
    outfd = open("/dev/ttySAC3",O_RDWR|O_NOCTTY);
    if(infd == -1||outfd == -1)
    {
        QMessageBox::critical(this, "错误", strerror(errno));
        exit(0);
    }

    //声明设置串口的结构体
    struct termios config;
    bzero(&config, sizeof(config));

    // 设置无奇偶校验
    // 设置数据位为8位
    // 设置为非规范模式（对比与控制终端）
    config.c_iflag &= ~IXON;
    //
    // 2. 输出：禁用自定义输出模式
    //
    // 3. 控制：数据字符长度为8位
    //         禁用校验码
    config.c_cflag &= ~CSIZE;
    /*
    switch(ui->ComboBoxDataBits->currentText().toInt())
    {
    case 5: config.c_cflag |= CS5; break;
    case 6: config.c_cflag |= CS6; break;
    case 7: config.c_cflag |= CS7; break;
    case 8: config.c_cflag |= CS8; break;
    }
    */
    config.c_cflag |= CS8;
    config.c_cflag &= ~PARENB;

    //设置波特率
    /*
    switch(ui->ComboBoxBaud->currentText().toInt())
    {
    case 9600  : cfsetspeed(&config, B9600); break;
    case 115200: cfsetspeed(&config, B115200); break;
    }
    */
    cfsetispeed(&config,B9600);
    // CLOCAL和CREAD分别用于本地连接和接受使能
    // 首先要通过位掩码的方式激活这两个选项。
    config.c_cflag |= CLOCAL | CREAD;

    // 一位停止位
    config.c_cflag &= ~CSTOPB;

    // 可设置接收字符和等待时间，无特殊要求可以将其设置为0
    config.c_cc[VTIME] = 0;
    config.c_cc[VMIN] = 1;

    // 用于清空输入/输出缓冲区
    tcflush (infd, TCIFLUSH);
    tcflush (infd, TCOFLUSH);

    tcflush(outfd, TCIFLUSH);
    tcflush(outfd, TCOFLUSH);
    //完成配置后，可以使用以下函数激活串口设置
    tcsetattr(infd, TCSANOW, &config);

    tcsetattr(outfd,TCSANOW,&config);

}

void Widget::on_pushButton_clicked()
{
    video->flag = true;
}


void Widget::on_pushButton_2_clicked()
{
    if(ui->pushButton_2->text()=="开始录像")
    {
        //video->start();
        //QWaitCondition::wakeone();
        //QWaitCondition::wakeOne();
        video->stop_flag = false;
        ui->pushButton_2->setText("暂停录像");
    }
    else if(ui->pushButton_2->text()=="暂停录像")
    {
        //video->wait();
        //video->sleep(10000000);
        //video->exit();
        video->stop_flag = true;
        ui->pushButton_2->setText("开始录像");
    }
}

void Widget::on_pushButton_3_clicked()
{
    video->destroy = true;
    sleep(1);
    //delete video;
    //sleep(1);
    //exit(0);
    this->close();
    exit(0);
}

void Widget::onflushSql()
{
    qDebug() << "flush sql";
    sqlmodel->setQuery("SELECT * FROM card");
}

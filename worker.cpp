#include "worker.h"
#include "common.h"
#include "caminfo.h"
#include "video_yuv.h"
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QSqlQuery>
#include <iostream>
#include <QMessageBox>
#include <opencv2/opencv.hpp>
using std::cout;

extern int R[256][256];
extern int G[256][256][256];
extern int B[256][256];

extern struct v4l2_format  fmt;
extern int roffset;
extern int goffset;
extern int boffset;

extern int lcd;
extern uint8_t *fb_mem;
extern struct fb_var_screeninfo lcdinfo;

extern int CAMERA_W, CAMERA_H;
extern int SCREEN_W, SCREEN_H, SCREEN_BPP;
extern int SCREEN_SIZE;

extern char *jpgname;
worker::worker()
{

}

worker::~worker()
{
    this->terminate();
}
void worker::run()
{
    QByteArray ba=str.toLatin1();
    char *c=ba.data();
    qDebug()<<"this is worker run";
    qDebug()<<str;
    system("madplay a.mp3");
    //system(c);
}


videoworker::videoworker(QLabel *label,int &id)
    :flag(flag),label(label),id(id)
{
    stop_flag==false;
    flag = false;
    destroy = false;
    reco = new recoCard(this);
    connect(reco, SIGNAL(readyJson()), this, SLOT(readJsonSlot()));
    connect(this,SIGNAL(reco_start()),this,SLOT(begin_reco()));

    cap.open(std::string("/dev/video7"));

}

videoworker::~videoworker()
{
    if(cap.isOpened())
        cap.release();
}
void videoworker::run()
{
    cvVideoShow();
    return;
    //视频处理理理
    /************** 准备YUV-RGB映射表 **************/

    if(access("yuv-rgb", F_OK))
    {
        // 创建YUV-RGB映射表
        pthread_t tid;
        pthread_create(&tid, NULL, convert, NULL);
    }
    else
    {
        // 读取YUV-RGB映射表
        FILE *fp = fopen("yuv-rgb", "r");
        fread(R, 256*4, 256,     fp);
        fread(G, 256*4, 256*256, fp);
        fread(B, 256*4, 256,     fp);
        fclose(fp);
    }


    /**************** 准备摄像头设备 ***************/
    int camfd = open("/dev/video7", O_RDWR);
    if(camfd == -1)
    {
        printf("打开摄像头设备[%s]失败: %s\n", "/dev/video7", strerror(errno));
        exit(0);
    }

    // 配置摄像头的采集格式
    set_camfmt(camfd, "YUYV");
    printf("\n摄像头的基本参数：\n");

    get_camcap(camfd);
    get_camfmt(camfd);
    get_caminfo(camfd);

    CAMERA_W = fmt.fmt.pix.width;
    CAMERA_H = fmt.fmt.pix.height;

    // 设置即将要申请的摄像头缓存的参数
    int nbuf = 3;

    struct v4l2_requestbuffers reqbuf;
    bzero(&reqbuf, sizeof (reqbuf));
    reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbuf.memory = V4L2_MEMORY_MMAP;
    reqbuf.count = nbuf;

    // 使用该参数reqbuf来申请缓存
    ioctl(camfd, VIDIOC_REQBUFS, &reqbuf);

    // 根据刚刚设置的reqbuf.count的值，来定义相应数量的struct v4l2_buffer
    // 每一个struct v4l2_buffer对应内核摄像头驱动中的一个缓存
    struct v4l2_buffer buffer[nbuf];
    int length[nbuf];
    uint8_t *start[nbuf];

    for(int i = 0; i < nbuf; i++)
    {
        bzero(&buffer[i], sizeof(buffer[i]));
        buffer[i].index  = i;
        buffer[i].memory = V4L2_MEMORY_MMAP;
        buffer[i].type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        ioctl(camfd, VIDIOC_QUERYBUF, &buffer[i]);

        length[i] = buffer[i].length;
        start[i]  = (uint8_t *)mmap(NULL, buffer[i].length, PROT_WRITE | PROT_READ,
                 MAP_SHARED, camfd, buffer[i].m.offset);

        ioctl(camfd , VIDIOC_QBUF, &buffer[i]);
    }

    // 启动摄像头数据采集
    enum v4l2_buf_type vtype= V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl(camfd, VIDIOC_STREAMON, &vtype);

    struct v4l2_buffer v4lbuf;
    bzero(&v4lbuf, sizeof(v4lbuf));
    v4lbuf.type  = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4lbuf.memory = V4L2_MEMORY_MMAP;

    // 开始抓取摄像头数据并在屏幕播放视频
    int i=0;
    while(1)
    {
        // 从队列中取出填满数据的缓存
        v4lbuf.index = i%nbuf;
        ioctl(camfd , VIDIOC_DQBUF, &v4lbuf);

        //	通过display获得一帧摄像头rgb数据
        uint8_t *rgb = display(start[i%nbuf]);

        //将rgb数据直接填充到QImage ,再填充到label
        QImage img(rgb,640,480,QImage::Format_RGB888);
        label->setPixmap(QPixmap::fromImage(img));

        if(flag == true)
        {
            qDebug()<<"抓拍";

            rgbtojpg(rgb);
            //将图片路径存入数据库对应的位置
            reco->setImg(QString("%1").arg(jpgname));
            //reco->start();
            emit reco_start();
            //emit sendjpgpath(jpgname,"测试1122");

            //query.exec(tmp);
            flag=false;

        }
        free(rgb);
        while(stop_flag&&destroy!=true)
        {
            sleep(1);//避免循环占用过多资源
        }
        if(destroy)
        {
            //free(rgb)
            break;
        }

        // 将已经读取过数据的缓存块重新置入队列中
        v4lbuf.index = i%nbuf;
        ioctl(camfd , VIDIOC_QBUF, &v4lbuf);

        i++;
    }
    close(camfd);
}

void videoworker::cvVideoShow()
{
    cv::Mat mat;
    while(1)
    {
        cap >> mat;
        if(mat.empty())
            break;
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        label->setPixmap(QPixmap::fromImage(image));

        if(flag == true)
        {
            qDebug()<<"抓拍";

            //rgbtojpg(rgb);
            //将图片路径存入数据库对应的位置
            reco->setImg(QString("%1").arg(jpgname));
            //reco->start();
            emit reco_start();
            //emit sendjpgpath(jpgname,"测试1122");

            //query.exec(tmp);
            flag=false;

        }

        while(stop_flag&&destroy!=true)
        {
            sleep(1);//避免循环占用过多资源
        }

        if(destroy)
        {
            //free(rgb)
            break;
        }
    }
}
void videoworker::readJsonSlot()
{
    if(reco->isRecoSuccess())
    {
        /* 显示车牌号*/
        //ui->idLineEdit->setText(reco->cardid());
        qDebug()<<"this is readjsonslot";
        emit sendjpgpath(jpgname,reco->cardid());

    }
    else
    {
        qDebug()<<"this is readjsonslot";
        emit sendjpgpath(jpgname,"识别失败");
    }
}

rfidWorker::rfidWorker(int infd, int outfd, videoworker *video, QSqlQuery *query)
    : inRFID(infd, 1), outRFID(outfd, 0),video(video),
    query(query)
{
    innum = 0;
    outnum =0;
}

void rfidWorker::detectCard(RFID &fd)
{
    //static int num = 0;
    init_REQUEST();
    char recvinfo[128];
//    char outvinfo[128];
    // 向串口发送指令

//    tcflush (infd, TCIFLUSH);
    write(fd.fd, PiccRequest_IDLE, PiccRequest_IDLE[0]);

//    tcflush(outfd,TCIFLUSH);
//    write(outfd, PiccRequest_IDLE,PiccRequest_IDLE[0]);

    usleep(10*1000);

    bzero(recvinfo, 128);
    qDebug() << __LINE__;
    read(fd.fd, recvinfo, 128);

/*
    bzero(outvinfo, 128);
    read(outfd,outvinfo,128);

*/
    //应答帧状态部分为0 则请求成功
    if(recvinfo[2] == 0x00)
    {
        qDebug() << "detected success!";
        //inFlags = true;
//        emit onDetected();
        if(fd.x == 1)
        {
            innum = 0;
            getCardNumber(fd);
        }
        else
        {
            outnum = 0;
            outCardNumber(fd);
        }

    }
    else
    {
        if(fd.fd==inRFID.fd&&innum<5)
        {
            innum++;
            qDebug()<<innum;
        }
        else if(fd.fd==inRFID.fd&&innum==5)
        {
            innum=0;
            fd.flags = true;
        }

        if(fd.fd==outRFID.fd&&outnum<5)
        {
            outnum++;
        }
        else if(fd.fd==outRFID.fd&&outnum==5)
        {
            outnum=0;
            fd.flags = true;
        }
        qDebug() << "no card...";

    }


}

void rfidWorker::getCardNumber(RFID &fd)
{
//    static uint32_t oldid, newid;

    // 获取附近卡片的卡号... ...
//    oldid = (newid == 0xFFFFFFFF ? oldid : newid);
    int newid = get_id(fd.fd);



    qDebug() << "fetch card Number:" << newid;

    if(newid == 0 || newid == 0xFFFFFFFF)
    {
        //flagxx=true;
        return;
    }
        int cardid;
    // flag为真意味着：卡片刚放上去
    if(fd.flags)
    {

            QString SQL = QString("select cardid from card where cardid=%1").arg(newid);
            query->exec(SQL);
            while(query->next())
            {
                 cardid      = query->record().value(0).toInt();

            }
            qDebug()<<cardid;
            qDebug()<<newid;
        if(cardid  == newid)
        {

                beep(5, 0.05);
                //fd.flags = false;
                //return;
        }
        else
        {
            qDebug() << __LINE__;

            //QString tmp = QString("insert into card(cardid,time,Yes) values('%1', '%2', 1)").arg(newid).arg(time(NULL));
            //query->exec(tmp);

            //抓拍图片
            video->id = newid;
            video->flag = true;

            emit sendcardid(newid);

            emit flushSql();
            beep(1, 0.3);
        }

        fd.flags = false;
    }
    //else
        //beep(5, 0.05);
//    qDebug() << "refres alarm";
    //alarm(1);
}

void rfidWorker::outCardNumber(RFID &fd)
{

        // 获取附近卡片的卡号... ...
        int newid = get_id(fd.fd);


        qDebug() << "fetch card Number:" << newid;

        if(newid == 0 || newid == 0xFFFFFFFF)
        {
            //flagxx=true;
            return;
        }

        // flag为真意味着：卡片刚放上去
        if(fd.flags)
        {
            QString SQL = QString("select *from card where cardid=%1").arg(newid);
            query->exec(SQL);
            while(query->next())
            {
                int     id      = query->value(0).toInt();
                if(id  == newid)
                {
                    beep(1, 0.3);
                    QString s = QString(" delete FROM card WHERE cardid like '%1';").arg(newid);
                    int timetmp = query->value(1).toInt();

                    int getcash = time(NULL)-timetmp;
                    qDebug()<<"cash:"<<getcash;
                    //query->exec(s);
                    emit sendcash(getcash);


                    query->exec(s);
                    fd.flags = false;
                    emit flushSql();
                    //sqlmodel->setQuery("SELECT * FROM card");
                    return;
                }
            }

            qDebug() << __LINE__;


            beep(5, 0.05);
            emit flushSql();
            //beep(1, 0.3);

            fd.flags = false;
        }
}

void rfidWorker::run()
{

    qDebug() << "begin ......";
    while(1)
    {
        detectCard(inRFID);

        usleep(30*1000);

        detectCard(outRFID);

        usleep(30*1000);
    }
}

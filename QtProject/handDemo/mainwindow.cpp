#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myutils.h"
#include <QMetaType>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    thread_camera = new QThread();
    MyCamera = new MyTHread();
    MyCamera->moveToThread(thread_camera);
    detector = new NanoDet("./model/hand-nanodet/nanodet-hand.param", "./model/hand-nanodet/nanodet-hand.bin",true);
    hp_detector = new handpose("./model/hand-nanodet/hand_lite-op.param", "./model/hand-nanodet/hand_lite-op.bin",false);
    /***********************************************************
     * 枚举所有的相机设备
     ************************************************************/
    std::vector<std::string> camIdx;
    bool deviceNumFlag = MyUtils::EnumerateCameras(camIdx);

    if(deviceNumFlag)
    {
        for (int var = 0; var < camIdx.size(); ++var)
        {
            ui->cB_device->addItem(QString::number(var)/*camIdx[var].c_str()*/);
        }
    }
    //启动线程 on_cB_device_activated
    //开启相机&MyTHread::videoThread
    connect(this,&MainWindow::signal_openCamera,MyCamera,[&](int deviceid){
        MyCamera->videoThread(deviceid);
    });
    //相机返回每一帧
    connect(MyCamera,&MyTHread::signal_frame,[&](cv::Mat& camFrame){
        if(!camFrame.empty()){
            cv::flip(camFrame,frame,1);
            //this->frame = camFrame.clone();

            //创造一块单独的画布,用于展示手势或3D场景
            if(this->frame_3D.size()!=this->frame.size())
            {
                this->frame_3D = cv::Mat(this->frame.size(),CV_8UC3,cv::Scalar::all(0));
            }
            else
            {
                //将画布置零
                for (int row = 0; row < frame_3D.rows; ++row)
                {
                    cv::Vec3b* ptr = frame_3D.ptr<cv::Vec3b>(row);
                    for (int col = 0; col < frame_3D.cols; ++col)
                    {
                        ptr[col][0] = 0;
                        ptr[col][1] = 0;
                        ptr[col][2] = 0;
                    }
                }
            }
            //NCNN 推理,获取key points
            cv::Mat dst;
            auto results = detector->detect(this->frame, 0.7, 0.5);
            std::vector<cv::Rect> rects;
            if(results.size()>0)
            {
                detector->draw(this->frame, results);
                for (int i = 0; i < results.size(); i++)
                {
                    BoxInfo bbox = results[i];
                    cv::Rect rec(cv::Point(bbox.x1, bbox.y1), cv::Point(bbox.x2, bbox.y2));
                    rects.push_back(rec);
                    //qDebug()<<"rect"<<rec.x<<rec.y<<rec.width<<rec.height;
                }
                std::vector<plamObject> plam_Objects;
                bool flag_res = hp_detector->detect(this->frame, rects, plam_Objects, 0.5);

                //将图像显示在窗口(OpenGLWidget)
                if(flag_res)hp_detector->draw(this->frame_3D, plam_Objects);
            }
            MyUtils::Mat2QImage(this->frame_3D,this->frame2QImage_3D);
            MyUtils::Mat2QImage(this->frame,this->frame2QImage);
            this->update();
        }
    });

    //===========================================================
    //关闭线程
    connect(this,&MainWindow::destroyed,[&](){
        //MyCamera->flag_closeCamera=true;
        MyCamera->flag_exitCamera=true;
        thread_camera->quit();
        thread_camera->wait();
        delete thread_camera;
        delete MyCamera;
        delete detector;
        delete hp_detector;
        qDebug()<<"关闭线程====";
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_cB_device_activated(int index)
{
    qDebug()<<"最近的相机ID"<<index;
    //启动线程
    if(!thread_camera->isRunning()){
        thread_camera->start();
        emit signal_openCamera(0);
    }
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter painter,painter2;
    painter.begin(ui->openGLWidget);
    //painter.drawImage(QPoint(0, 0),this->frame2QImage);
    painter.drawPixmap(0,0,ui->openGLWidget->width(),ui->openGLWidget->height(),QPixmap::fromImage(this->frame2QImage_3D));

    painter2.begin(ui->openGLWidget_2);
    painter2.drawPixmap(0,0,ui->openGLWidget_2->width(),ui->openGLWidget_2->height(),QPixmap::fromImage(this->frame2QImage));
    painter2.end();
    painter.end();
}

void MainWindow::on_pb_changeCam_clicked()
{
    qDebug()<<"启相机参数自调节模式";
    MyCamera->Capture.set(cv::CAP_PROP_SETTINGS, 1);
//    if(thread_camera->isRunning()){
//        MyCamera->Capture.set(cv::CAP_PROP_SETTINGS, 1);//开启相机参数自调节模式
//    }
}

void MainWindow::on_pb_openCam_clicked()
{
    int index = 0;
    qDebug()<<"最近的相机ID"<<index;
    //启动线程
    if(!thread_camera->isRunning()){
        thread_camera->start();
        emit signal_openCamera(0);
    }
}

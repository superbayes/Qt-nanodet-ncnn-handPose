#include "mycamerathread.h"
#include <QDebug>
#include <QThread>
MyCameraThread::MyCameraThread(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<cv::Mat>("cv::Mat&");

}

void MyCameraThread::cameraProcess(int camID)
{

    //打开相机
    this->Capture.open(camID);
    if(!Capture.isOpened()) return;
    qDebug()<<"==相机打开成功!!!";
//    Capture.set(cv::CAP_PROP_FRAME_WIDTH, 1920);//宽度
//    Capture.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);//高度
//    Capture.set(cv::CAP_PROP_FPS, 30);//帧数
    Capture.set(cv::CAP_PROP_BRIGHTNESS, 100);//亮度 1
//    Capture.set(cv::CAP_PROP_CONTRAST,40);//对比度 40
//    Capture.set(cv::CAP_PROP_SATURATION, 50);//饱和度 50
//    Capture.set(cv::CAP_PROP_HUE, 50);//色调 50
//    Capture.set(cv::CAP_PROP_AUTO_EXPOSURE, 0.25);
//    Capture.set(cv::CAP_PROP_EXPOSURE, -6);//曝光 50
    //Capture.set(cv::CAP_PROP_AUTOFOCUS, 0);//开启自动对焦功能(AutoFocus)
    //Capture.set(cv::CAP_PROP_SETTINGS, 1);//开启参数自调节模式

    //循环
    while(1){
        if(flag_closeCamera==true) {Capture.release();break;};
        Capture >> this->frame;
        emit this->signal_sentFrame(frame);

        //qDebug()<<"src_width:"<<frame.cols<<";src_height:"<<frame.rows;
        QThread::msleep(50);
    }
}

void MyCameraThread::slot_closeCamera(bool flag_closeCamera)
{
    this->flag_closeCamera = flag_closeCamera;
};

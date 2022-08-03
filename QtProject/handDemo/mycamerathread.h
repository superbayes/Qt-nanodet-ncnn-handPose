#ifndef MYCAMERATHREAD_H
#define MYCAMERATHREAD_H
#include <QMetaType>
#include <QObject>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/imgcodecs.hpp>
class MyCameraThread : public QObject
{
    Q_OBJECT
public:
    explicit MyCameraThread(QObject *parent = nullptr);

public:
    //相机线程,持续不断的获取帧图像
    void cameraProcess(int camID);
public:
    cv::Mat frame;//帧图像
    bool flag_closeCamera = false;//是否关闭相机
    cv::VideoCapture Capture;

signals:
    //将获取的帧图像传递出去
    void signal_sentFrame(cv::Mat& frame__);
public slots:
    //接受外界传来的信号,关闭相机
    void slot_closeCamera(bool flag_closeCamera);

};

#endif // MYCAMERATHREAD_H

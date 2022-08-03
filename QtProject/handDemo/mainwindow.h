#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QComboBox>
#include "mycamerathread.h"
#include "mythread.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "handAI/nanodetDemo.h"
#include "handAI/handpose.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QThread *thread_camera;
    MyTHread* MyCamera;
    cv::Mat frame;
    cv::Mat frame_3D;
    QImage frame2QImage;
    QImage frame2QImage_3D;
    NanoDet *detector;
    handpose *hp_detector;
public:
    void paintEvent(QPaintEvent *e);

private slots:
    //选定需要打开的相机ID(可能有n个USB相机)
    void on_cB_device_activated(int index);
    void on_pb_changeCam_clicked();

    void on_pb_openCam_clicked();

signals:
    void signal_openCamera(int index);
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

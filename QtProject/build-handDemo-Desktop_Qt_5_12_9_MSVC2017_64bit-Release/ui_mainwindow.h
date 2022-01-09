/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QOpenGLWidget *openGLWidget;
    QOpenGLWidget *openGLWidget_2;
    QComboBox *cB_device;
    QPushButton *pb_openCam;
    QPushButton *pb_changeCam;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1316, 814);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        openGLWidget = new QOpenGLWidget(centralwidget);
        openGLWidget->setObjectName(QString::fromUtf8("openGLWidget"));
        openGLWidget->setGeometry(QRect(10, 10, 960, 720));
        openGLWidget_2 = new QOpenGLWidget(centralwidget);
        openGLWidget_2->setObjectName(QString::fromUtf8("openGLWidget_2"));
        openGLWidget_2->setGeometry(QRect(980, 10, 320, 240));
        cB_device = new QComboBox(centralwidget);
        cB_device->setObjectName(QString::fromUtf8("cB_device"));
        cB_device->setGeometry(QRect(1070, 300, 81, 41));
        pb_openCam = new QPushButton(centralwidget);
        pb_openCam->setObjectName(QString::fromUtf8("pb_openCam"));
        pb_openCam->setGeometry(QRect(980, 300, 81, 41));
        pb_changeCam = new QPushButton(centralwidget);
        pb_changeCam->setObjectName(QString::fromUtf8("pb_changeCam"));
        pb_changeCam->setGeometry(QRect(980, 350, 81, 41));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1316, 23));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        pb_openCam->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\347\233\270\346\234\272:", nullptr));
        pb_changeCam->setText(QApplication::translate("MainWindow", "\350\260\203\350\212\202\347\233\270\346\234\272:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

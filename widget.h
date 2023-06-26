#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QMessageBox>
#include <QDir>
#include <QTimer>
#include <QTime>
#include <QMouseEvent>
#include <QKeyEvent>

// 載入相機文件
#include <QCamera>
#include <QMediaDevices>
#include <QMediaCaptureSession>
#include <QMediaRecorder>
#include <QImageCapture>
#include <QVideoWidget>
#include <QVideoSink>


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_comboBox_list_currentIndexChanged(int index);

    void on_pushButton_Open_clicked();

    void on_pushButton_Close_clicked();

    void on_pushButton_Photo_clicked();

private:
    Ui::Widget *ui;

    // 建立相機設備
    QList<QCameraDevice> list_cameras;

    // QScopedPointer保證指向的對象超出作用域後會自動被刪除
    QScopedPointer<QCamera> my_camera;  // 建立相機對象
    QScopedPointer<QMediaRecorder> my_mediaRecorder;
    QMediaCaptureSession my_captureSession;
    QImageCapture *imageCapture;

    bool camera_state=false;

};
#endif // WIDGET_H

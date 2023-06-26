#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->widget_image->hide();

    // 取得可用的相機設備列表
    list_cameras = QMediaDevices::videoInputs();
    int n = list_cameras.length();

    // 將相機列表添加到選單中
    if(n!=0){
        ui->comboBox_list->clear();
        for(int i=0; i<n; i++)
            ui->comboBox_list->addItem(list_cameras[i].description());  // addItem()添加選項
    }

}

Widget::~Widget()
{
    delete ui;
}

// 切換相機
void Widget::on_comboBox_list_currentIndexChanged(int index)
{
    // 關閉相機
    if(camera_state)
    {
        my_camera->stop();
        camera_state = false;   // 相機狀態:關閉
    }

    if(index!=-1){
        // 重新設定相機設備
        my_camera.reset(new QCamera(list_cameras[index],this));
        // 獲得相機畫面
        my_captureSession.setCamera(my_camera.data());
        // 設定畫面顯示窗口
        my_captureSession.setVideoOutput(ui->widget_display);
        // 啟動相機
        my_camera->start();
        // 更新相機狀態:開啟
        camera_state = true;
        // 暫存每幀圖像
        imageCapture = new QImageCapture;
        my_captureSession.setImageCapture(imageCapture);
    }

}

// 開啟相機
void Widget::on_pushButton_Open_clicked()
{
    if(!camera_state)  // 如果相機狀態:關閉(false)
    {
        int index = ui->comboBox_list->currentIndex();
        if (index!=-1){
            ui->widget_image->hide();
            // 重新設定相機設備
            my_camera.reset(new QCamera(list_cameras[index],this));
            // 獲得相機畫面
            my_captureSession.setCamera(my_camera.data());
            // 設定畫面顯示窗口
            my_captureSession.setVideoOutput(ui->widget_display);
            // 啟動相機
            my_camera->start();
            // 更新相機狀態:開啟
            camera_state = true;
            // 暫存每幀圖像
            imageCapture = new QImageCapture;
            my_captureSession.setImageCapture(imageCapture);
        }
    }
}

// 關閉相機
void Widget::on_pushButton_Close_clicked()
{
    if(camera_state)  // 如果相機狀態:開啟(true)
    {
        ui->widget_image->show();
        my_camera->stop();
        camera_state = false;   // 相機狀態:關閉
    }
}


void Widget::on_pushButton_Photo_clicked()
{
    QDir dir;
    // 設定儲存照片的資料夾，不存在則創建
    if(dir.exists("./photograph") == false)
    {
        dir.mkpath("./photograph");
    }

    // 使用拍照時間命名(達到檔名唯一性)
    QDateTime dateTime(QDateTime::currentDateTime());
    QString time = dateTime.toString("yyyy-MM-dd-hh-mm-ss");

    //設定圖片儲存路徑
    QString filename = QDir::currentPath();    // 取得當前路徑
    filename += "/photograph/photo_";
    filename += QString("%1.jpg").arg(time);

    if(!camera_state)
    {
        QMessageBox::warning(this, "警告", tr("拍照失敗"));
        return;
    }
    QMessageBox::information(this, "提示", tr("拍照成功"));
    imageCapture->captureToFile(filename);      // 儲存照片
}


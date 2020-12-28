#pragma once
#include <QMainWindow>
#include <QWidget>
#include <QCloseEvent>
#include <memory>
#include <atomic>
#include "ui_videowindow.h"
#include "nrtc_engine.h"

class VideoWindow : public QMainWindow
{
    Q_OBJECT
public:
    VideoWindow(QWidget *parent = Q_NULLPTR);
    ~VideoWindow();

    void closeEvent(QCloseEvent *event) override;

public: 
    void setNeRtcEngine(std::shared_ptr<NRTCEngine> ptr);

    QWidget* getRenderObject(int i);
signals:
    void closeVideoWindowSignal();
public slots:
    void onJoinChannel(QString& roomid, QString& usrId);
    void on_disconnectBtn_clicked(bool checked);
    void onUserJoined(quint64 uid);
    void onUserLeft(quint64 uid);
    void onSmoothSwitch(int status);

protected:
private:
    Ui::MainWindow ui;
    std::shared_ptr<NRTCEngine> m_engine;

    std::map<int, VideoWidget*> m_videoWindowMap;
    //自身不统计，第一窗口为自己视频
    std::atomic_short       m_videoCount = {1};
};

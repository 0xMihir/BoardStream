#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <iostream>

static cv::VideoCapture capture;

class CameraThread : public QObject
{
    Q_OBJECT
public:
    CameraThread();
    ~CameraThread();
    void startCapture(int cameraNumber);
    void stopCapture();
    void changeCamera(int cameraNumber);

private slots:
    void streamThread();
    void copyFrame(cv::Mat threadFrame);
    void frameToQt();

signals:
    void newImage(QImage &);
    void newFrame(cv::Mat frame);
    void detectedMarkers();
private:
    int m_cameraNumber;
    QTimer m_timer;
    QThread *m_videoStreamThread;
    cv::Mat m_frame;

};

#endif // CAMERATHREAD_H

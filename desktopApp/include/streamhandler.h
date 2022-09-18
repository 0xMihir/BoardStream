#ifndef STREAMHANDLER_H
#define STREAMHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <opencv2/opencv.hpp>

class StreamHandler : public QObject
{
    Q_OBJECT
public:
    StreamHandler();

public slots:
    void processFrame(cv::Mat frame);

private:
    QTcpSocket *m_socket;
    std::vector<uchar> m_imageBuffer;
    std::vector<int> m_compression_params;
};

#endif // STREAMHANDLER_H

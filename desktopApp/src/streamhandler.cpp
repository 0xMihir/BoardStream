#include "streamhandler.h"

StreamHandler::StreamHandler() : QObject()
{
    m_socket = new QTcpSocket(this);
    m_socket->connectToHost(QHostAddress::LocalHost,9000);
    m_imageBuffer.resize(1024*1024*10);


    m_compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
    m_compression_params.push_back(50);
    m_compression_params.push_back(cv::IMWRITE_JPEG_PROGRESSIVE);
    m_compression_params.push_back(1);
    m_compression_params.push_back(cv::IMWRITE_JPEG_OPTIMIZE);
    m_compression_params.push_back(1);
    m_compression_params.push_back(cv::IMWRITE_JPEG_LUMA_QUALITY);
    m_compression_params.push_back(50);
}

void StreamHandler::processFrame(cv::Mat frame)
{
    m_imageBuffer.clear();
    cv::imencode(".jpg",frame, m_imageBuffer, m_compression_params);
    char *data = (char *)(m_imageBuffer.data());
    m_socket->write(data, m_imageBuffer.size());
}

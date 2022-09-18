#include "whiteboardimageprovider.h"

WhiteboardImageProvider::WhiteboardImageProvider():  QQuickImageProvider(QQuickImageProvider::Image)
{
    m_image = QImage(1920, 1080, QImage::Format_RGB32);
}

QImage WhiteboardImageProvider::requestImage(const QString &id, QSize *size, const QSize &req) {

    if(size) {
        *size = m_image.size();
    }

    if(req.width() > 0 && req.height() > 0) {
        m_image = m_image.scaled(req.width(), req.height(), Qt::KeepAspectRatio);
    }
    return m_image;
}

void WhiteboardImageProvider::update(QImage &newImage)
{
    if(!newImage.isNull() && this->m_image != newImage) {
        this->m_image = newImage;
        emit updated();
    }
}

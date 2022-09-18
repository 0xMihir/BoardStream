#ifndef WHITEBOARDIMAGEPROVIDER_H
#define WHITEBOARDIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QObject>

class WhiteboardImageProvider : public QQuickImageProvider
{
    Q_OBJECT
public:
    WhiteboardImageProvider();
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
public slots:
    void update(QImage &newImage);
signals:
   void updated();
private:
   QImage m_image;
};

#endif // WHITEBOARDIMAGEPROVIDER_H

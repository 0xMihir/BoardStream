#include "camerathread.h"
#include <QThread>
#include <QImage>
#include <opencv2/aruco.hpp>
#include "streamhandler.h"

CameraThread::CameraThread() : QObject()
{
    m_videoStreamThread = new QThread();
    connect(&m_timer, &QTimer::timeout, this, &CameraThread::frameToQt);
}

CameraThread::~CameraThread()
{
    stopCapture();
    delete m_videoStreamThread;
}

void CameraThread::startCapture(int cameraNumber)
{
    m_cameraNumber = cameraNumber;
    capture.open(cameraNumber);

    CameraThread *camThread = new CameraThread;
    connect(m_videoStreamThread, &QThread::started, camThread, &CameraThread::streamThread);


    camThread->moveToThread(m_videoStreamThread);
    connect(camThread, &CameraThread::newFrame, this, &CameraThread::copyFrame);

    StreamHandler *streamHandler = new StreamHandler();

    connect(camThread, &CameraThread::newFrame, streamHandler, &StreamHandler::processFrame);


    m_videoStreamThread->start();

    double fps = capture.get(cv::CAP_PROP_FPS);
    m_timer.start(1000 / fps);

}

void CameraThread::stopCapture()
{
    capture.release();
    m_videoStreamThread->requestInterruption();
    m_timer.stop();
}

void CameraThread::changeCamera(int cameraNumber)
{
    stopCapture();
    startCapture(cameraNumber);
}


void CameraThread::frameToQt()
{
    if (m_frame.data) {
        QImage img = QImage(m_frame.data,m_frame.cols, m_frame.rows, QImage::Format_RGB888).rgbSwapped();
        emit newImage(img);
    }
}

void CameraThread::copyFrame(cv::Mat threadFrame)
{
    m_frame = threadFrame;
}

static double distance(const cv::Point2f *a, const cv::Point2f *b)
{
    return std::sqrt((pow(a->x - b->x, 2)) + (pow(a->y - b->y, 2)));
}

cv::Mat makeBlackBorder(const cv::Mat &input, const cv::Size &dst, const cv::Scalar &bgcolor)
{
    cv::Mat output;

    double h1 = dst.width * (input.rows / (double)input.cols);
    double w2 = dst.height * (input.cols / (double)input.rows);

    if (h1 <= dst.height)
        cv::resize(input, output, cv::Size(dst.width, h1));
    else
        cv::resize(input, output, cv::Size(w2, dst.height));

    int top = (dst.height - output.rows) / 2;
    int down = (dst.height - output.rows + 1) / 2;
    int left = (dst.width - output.cols) / 2;
    int right = (dst.width - output.cols + 1) / 2;

    cv::copyMakeBorder(output, output, top, down, left, right, cv::BORDER_CONSTANT, bgcolor);

    return output;
}

void CameraThread::streamThread() {
    cv::Mat frame,frame_copy;

    cv::Ptr<cv::aruco::Dictionary> dictionary =
            cv::aruco::getPredefinedDictionary(
                cv::aruco::PREDEFINED_DICTIONARY_NAME(11));
    bool firstDetected = false;
    std::array<cv::Point2f, 4> rect;

    cv::Mat dst;
    int maxWidth, maxHeight;
    std::once_flag flag;

    while (!QThread::currentThread()->isInterruptionRequested()) {

        capture >> frame;

        if(frame.data) {
            frame.copyTo(frame_copy);


            std::vector<int> ids;
            std::vector<std::vector<cv::Point2f>> corners;

            cv::aruco::detectMarkers(frame, dictionary, corners, ids);

            // If at least one marker detected
            if (ids.size() == 4)
            {
                std::vector<int> indicies{0, 1, 2, 3};
                std::sort(indicies.begin(), indicies.end(), [&ids](int a, int b)
                { return ids[a] < ids[b]; });
                int j = 0;
                for (int i : indicies)
                {
                    rect[j] = corners[i][0];
                    j++;
                }


                maxWidth = std::max(distance(&rect[2], &rect[3]), distance(&rect[1], &rect[0]));

                maxHeight = std::max(distance(&rect[1], &rect[2]), distance(&rect[0], &rect[3]));
                // std::cout << maxWidth << " " << maxHeight << std::endl;
                dst = (cv::Mat_<float>(4, 2) << 0, 0, maxWidth - 1, 0, maxWidth - 1, maxHeight - 1, 0, maxHeight - 1);

                firstDetected = true;
            }
            if (firstDetected)
            {
                std::call_once(flag, [this]{emit detectedMarkers();});
                cv::Mat transform = cv::getPerspectiveTransform(rect, dst);
                cv::Size size{maxWidth, maxHeight};
                cv::warpPerspective(frame, frame_copy, transform, size);
                emit newFrame(makeBlackBorder(frame_copy, {1920, 1080}, cv::Scalar(0)));
            } else {
                emit newFrame(frame);
            }
        }
    }
    capture.release();
}

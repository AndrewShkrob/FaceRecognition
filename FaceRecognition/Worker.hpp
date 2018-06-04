#ifndef WORKER_HPP
#define WORKER_HPP

#include <QObject>
#include <QImage>
#include <QDir>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <string>
#include <vector>

#include "FaceDetector.hpp"
#include "FaceRecognizer.hpp"
#include "Settings.hpp"

enum StreamStatus
{
    ENABLED,
    DISABLED
};

enum StreamType
{
    WEBCAM,
    FILe,
    EMPTY
};

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);
    ~Worker();

private:
    cv::Mat _frameOriginal;
    cv::Mat _frameProcessed;
    cv::VideoCapture *_capture;

    StreamStatus _streamStatus;
    StreamType _streamType;

    FaceDetector &_faceDetector = FaceDetector::getInstance();
    FaceRecognizer &_faceRecognizer = FaceRecognizer::getInstance();
    Settings &_settings = Settings::getInstance();

    void checkIfDeviceAlreadyOpened(const StreamType streamType, const std::string path);
    void process();

signals:
    void sendFrame(const QImage frame) const;

public slots:
    void receiveGrabFrame();
    void receiveSetup(const StreamType streamType, const QString path = "/dev/video0");

    void receiveChangeStreamStatus(const StreamStatus status);

};

#endif // WORKER_HPP

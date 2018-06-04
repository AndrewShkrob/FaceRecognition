#ifndef ADDFACEWORKER_HPP
#define ADDFACEWORKER_HPP

#include <QObject>
#include <QImage>
#include <QDir>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <string>
#include <vector>

#include "Worker.hpp"
#include "FaceDetector.hpp"
#include "FaceRecognizer.hpp"

class AddFaceWorker : public QObject
{
    Q_OBJECT
public:
    explicit AddFaceWorker(QObject *parent = nullptr);
    ~AddFaceWorker();

private:
    cv::Mat _frameOriginal;
    cv::Mat _frameProcessed;
    cv::VideoCapture *_capture;

    StreamStatus _streamStatus;
    StreamType _streamType;

    dlib::matrix<dlib::rgb_pixel> _detectedFace;

    FaceDetector &_faceDetector = FaceDetector::getInstance();
    FaceRecognizer &_faceRecognizer = FaceRecognizer::getInstance();

    void checkIfDeviceAlreadyOpened(const StreamType streamType, const std::string path);
    void process();

signals:
    void sendFrame(const QImage frame);

    void sendMoreThanOneFaceDetected();
    void sendNoFaceDetected();
    void sendNoFaceDetectedWithoutMessage();
    void sendFaceDetected();

public slots:
    void receiveGrabFrame();
    void receiveSetup(const StreamType streamType, const QString path = "/dev/video0");

    void receiveChangeStreamStatus(const StreamStatus status);

    void receiveAddFace(const QString name);
};

#endif // ADDFACEWORKER_HPP

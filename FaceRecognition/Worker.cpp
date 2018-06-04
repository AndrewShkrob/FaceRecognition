#include "Worker.hpp"

// PUBLIC
Worker::Worker(QObject *parent) :
    QObject(parent),
    _streamStatus(DISABLED),
    _streamType(EMPTY)
{
    _capture = new cv::VideoCapture();
}

Worker::~Worker()
{
    if (_capture->isOpened())
    {
        _capture->release();
    }

    delete _capture;
}

// PRIVATE
void Worker::checkIfDeviceAlreadyOpened(const StreamType streamType, const std::string path)
{
    if (_capture->isOpened())
    {
        _capture->release();
    }

    _capture->open(path);

    _streamType = streamType;
}

void Worker::process()
{
    cv::cvtColor(_frameOriginal, _frameProcessed, cv::COLOR_BGR2RGB);

    auto detectedFaces = _faceDetector.detectFaces(_frameProcessed);
    auto recognizedFaces = _faceRecognizer.recognizeFaces(detectedFaces);

    if (_settings.showRecognizedFaces())
    {
        for (unsigned i = 0; i < recognizedFaces.size(); i++)
        {
            cv::putText(
                        _frameProcessed,
                        recognizedFaces[i].getName(),
                        cv::Point(recognizedFaces[i].getPosition().x, recognizedFaces[i].getPosition().y - 3),
                        cv::FONT_HERSHEY_COMPLEX, 1.0,
                        _settings.getRecognizedFaceTextColor()
            );
            cv::rectangle(_frameProcessed, recognizedFaces[i].getPosition(), _settings.getRecognizedFaceFrameColor());
        }
    }
}

// SLOTS
void Worker::receiveGrabFrame()
{
    if (_streamStatus == DISABLED)
    {
        return;
    }

    (*_capture) >> _frameOriginal;

    if (_frameOriginal.empty())
    {
        return;
    }

    process();

    const QImage outputFrame = QImage(
                (const unsigned char *) _frameProcessed.data,
                _frameProcessed.cols,
                _frameProcessed.rows,
                QImage::Format_RGB888
    );

    emit sendFrame(outputFrame);
}

void Worker::receiveSetup(const StreamType streamType, const QString path)
{
    if ((_streamType == streamType) && (streamType == WEBCAM))
    {
        return;
    }

    checkIfDeviceAlreadyOpened(streamType, path.toUtf8().constData());

    if (!_capture->isOpened())
    {
        _streamStatus = DISABLED;
        _streamType = EMPTY;
        return;
    }

    _streamStatus = ENABLED;
    _streamType = streamType;
}

void Worker::receiveChangeStreamStatus(StreamStatus status)
{
    _streamStatus = status;
}

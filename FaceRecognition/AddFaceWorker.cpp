#include "AddFaceWorker.hpp"

// PUBLIC
AddFaceWorker::AddFaceWorker(QObject *parent) :
    QObject(parent),
    _streamStatus(DISABLED),
    _streamType(EMPTY)
{
    _capture = new cv::VideoCapture();
}

AddFaceWorker::~AddFaceWorker()
{
    if (_capture->isOpened())
    {
        _capture->release();
    }

    delete _capture;
}

// PRIVATE
void AddFaceWorker::checkIfDeviceAlreadyOpened(const StreamType streamType, const std::string path)
{
    if (_capture->isOpened())
    {
        _capture->release();
    }

    _capture->open(path);

    _streamType = streamType;
}

void AddFaceWorker::process()
{
    cv::cvtColor(_frameOriginal, _frameProcessed, cv::COLOR_BGR2RGB);

    auto detectedFaces = _faceDetector.detectFaces(_frameProcessed);

    for (unsigned i = 0; i < detectedFaces.size(); i++)
    {
        cv::rectangle(_frameProcessed, detectedFaces[i].first, cv::Scalar(255, 0, 0), 3);
    }

    cv::resize(_frameProcessed, _frameProcessed, cv::Size(640, 480));

    if (_streamType != WEBCAM)
    {
        if (detectedFaces.size() == 0)
        {
            emit sendNoFaceDetected();
            return;
        }

        if (detectedFaces.size() > 1)
        {
            emit sendMoreThanOneFaceDetected();
            return;
        }
    }

    if (detectedFaces.size() != 0)
    {
        _detectedFace = detectedFaces[0].second;
    }
    else
    {
        emit sendNoFaceDetectedWithoutMessage();
        return;
    }

    emit sendFaceDetected();
}


// SLOTS
void AddFaceWorker::receiveGrabFrame()
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

void AddFaceWorker::receiveSetup(const StreamType streamType, const QString path)
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

void AddFaceWorker::receiveChangeStreamStatus(const StreamStatus status)
{
    _streamStatus = status;
}

void AddFaceWorker::receiveAddFace(const QString name)
{
    _faceRecognizer.addFace(name.toUtf8().constData(), _detectedFace);
}

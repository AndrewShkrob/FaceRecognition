#ifndef FACERECOGNIZER_HPP
#define FACERECOGNIZER_HPP

#include <QDir>

#include <opencv2/imgproc.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "ResNetwork.hpp"
#include "PersonBaseEntity.hpp"
#include "PersonBaseHelper.hpp"
#include "DetectedFace.hpp"
#include "Settings.hpp"

#define DLIB_USE_CUDA

class FaceRecognizer
{
public:

    static FaceRecognizer & getInstance()
    {
        static FaceRecognizer faceRecognizer;
        return faceRecognizer;
    }

    FaceRecognizer (FaceRecognizer const &) = delete;
    void operator = (FaceRecognizer const &) = delete;


    std::vector<DetectedFace> recognizeFaces(std::vector<std::pair<cv::Rect, dlib::matrix<dlib::rgb_pixel>>> faces);
    void addFace(const std::string name, dlib::matrix<dlib::rgb_pixel> face);

private:
    resnet::ResNetDNN _network;

    PersonBaseHelper &_personBaseHelper = PersonBaseHelper::getInstance();
    Settings &_settings = Settings::getInstance();

    std::mutex _networkMutex;
    std::mutex _personBaseHelperMutex;

    FaceRecognizer();
    ~FaceRecognizer();

    void loadResNet(const std::string file);

    std::vector<dlib::matrix<dlib::rgb_pixel>> jitterFace(const dlib::matrix<dlib::rgb_pixel> &face);
};

#endif // FACERECOGNIZER_HPP

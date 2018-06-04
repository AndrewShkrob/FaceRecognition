#ifndef FACEDETECTOR_HPP
#define FACEDETECTOR_HPP

#include <QDir>

#include <vector>
#include <string>

#include <dlib/image_processing/shape_predictor.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/opencv/cv_image.h>
#include <opencv2/imgproc.hpp>

#include <iostream>

#include "Settings.hpp"

#define DLIB_USE_CUDA

typedef std::pair<cv::Rect, dlib::matrix<dlib::rgb_pixel>> Face;

class FaceDetector
{
public:
    static FaceDetector & getInstance()
    {
        static FaceDetector faceDetector;
        return faceDetector;
    }

    FaceDetector (FaceDetector const &) = delete;
    void operator = (FaceDetector const &) = delete;

    std::vector<Face> detectFaces(const cv::Mat frame);

private:
    dlib::frontal_face_detector _faceDetector;
    dlib::shape_predictor _shapePredictor;

    std::mutex _face_detector_mutex;
    std::mutex _shape_predictor_mutex;

    FaceDetector();
    ~FaceDetector();

    void loadShapePredictor(const std::string file);
};

#endif // FACEDETECTOR_HPP

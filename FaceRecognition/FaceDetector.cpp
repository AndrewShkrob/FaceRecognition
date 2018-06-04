#include "FaceDetector.hpp"

#include <iostream>

// PUBLIC
std::vector<Face> FaceDetector::detectFaces(const cv::Mat frame)
{
    // Массив, в котором находятся все найденные лица
    std::vector<Face> detectedFaces;

    // Конвертируем кадр из формата cv::Mat в dlib::matrix<dlib::rgb_pixel>>
    // Для дальнейшей обработки библиоткеой Dlib
    const dlib::cv_image<dlib::rgb_pixel> cvImage(frame);
    dlib::matrix<dlib::rgb_pixel> dlibImage;
    dlib::assign_image(dlibImage, cvImage);

    // Т.к. класс реализован с исользованием паттерна Singleton,
    // нам необходимо обеспечить синхронизированный доступ из разных потоков
    // к единственному экземпляру нейронной сети, которая опередяет лица на
    // изображении
    _face_detector_mutex.lock();
    // Обрабатываем кадр нейронной сетью
    auto faces = _faceDetector(dlibImage);
    _face_detector_mutex.unlock();

    // Для всех найденных лиц ...
    for (unsigned i = 0; i < faces.size(); i++)
    {
        // Аналогично
        _shape_predictor_mutex.lock();
        const auto shape = _shapePredictor(dlibImage, faces[i]);
        _shape_predictor_mutex.unlock();

        // Уменьшаем изображение лица до 150х150
        // для дальнейшей обработки нейронной сетью
        dlib::matrix<dlib::rgb_pixel> faceChip;
        dlib::extract_image_chip(
                    dlibImage,
                    dlib::get_face_chip_details(shape, 150, 0.25),
                    faceChip
        );

        // Запоминаем координаты лица на изображении
        cv::Rect facePosition(
                    cv::Point2i(faces[i].left(), faces[i].top()),
                    cv::Point2i(faces[i].right() + 1, faces[i].bottom() + 1)
        );

        // Записываем лицо в массив
        detectedFaces.push_back(std::make_pair(std::move(facePosition), std::move(faceChip)));
    }

    return detectedFaces;
}

// PRIVATE
FaceDetector::FaceDetector()
{
    std::cout << "[LOAD] " << "Loading frontal face detector." << std::endl;
    _faceDetector = dlib::get_frontal_face_detector();

    std::cout << "[LOAD] " << "Loading shape predictor 68 face landmarks." << std::endl;
    loadShapePredictor(Settings::getInstance().getShapePredictor68Path());
}

FaceDetector::~FaceDetector()
{

}


void FaceDetector::loadShapePredictor(const std::string file)
{
    dlib::deserialize(file) >> _shapePredictor;
}

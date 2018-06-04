#ifndef FACE_HPP
#define FACE_HPP

#include <opencv2/highgui.hpp>

#include <string>

class DetectedFace
{
public:
    DetectedFace();
    DetectedFace(std::string name, cv::Rect position);

    std::string getName() const;
    cv::Rect getPosition() const;

private:
    std::string _name;
    cv::Rect _position;
};

#endif // FACE_HPP

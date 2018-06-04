#include "DetectedFace.hpp"

DetectedFace::DetectedFace()
{

}

DetectedFace::DetectedFace(std::string name, cv::Rect position)
{
    _name = name;
    _position = position;
}

std::string DetectedFace::getName() const
{
    return _name;
}

cv::Rect DetectedFace::getPosition() const
{
    return _position;
}

#include "FaceRecognizer.hpp"

#include <iostream>

// PUBLIC
std::vector<DetectedFace> FaceRecognizer::recognizeFaces(std::vector<std::pair<cv::Rect, dlib::matrix<dlib::rgb_pixel>>> faces)
{
    _personBaseHelperMutex.lock();
    auto pbase = _personBaseHelper.getPersonBase();
    _personBaseHelperMutex.unlock();

    std::vector<DetectedFace> recognizedFaces;

    for (unsigned i = 0; i < faces.size(); i++)
    {
        std::pair<double, std::string> minLength = std::make_pair(1000, "");

        _networkMutex.lock();
        const dlib::matrix<float, 0, 1> faceDescriptor = _network(faces[i].second);
        _networkMutex.unlock();

        for (unsigned j = 0; j < pbase.size(); j++)
        {
            double dist = dlib::length(pbase[j].getFaceDescriptor() - faceDescriptor);

            if (dist < _settings.getFaceRecognitionThreshold() && dist < minLength.first)
            {
                minLength = std::make_pair(dist, pbase[j].getName());
            }
        }

        if (minLength.first < _settings.getFaceRecognitionThreshold())
        {
            std::cout << "[DEBUG] " << "Found person: " << minLength.second << ". Length: " << minLength.first << std::endl;
            recognizedFaces.push_back(DetectedFace(minLength.second, faces[i].first));
        }
    }

    return recognizedFaces;
}

void FaceRecognizer::addFace(const std::string name, dlib::matrix<dlib::rgb_pixel> face)
{
    std::vector<dlib::matrix<dlib::rgb_pixel>> jitteredFace = jitterFace(face);

    _networkMutex.lock();
    auto netFace = _network(jitteredFace);
    _networkMutex.unlock();

    dlib::matrix<float, 0, 1> faceDescriptor = dlib::mean(dlib::mat(netFace));

    // check if person exists in base
    _personBaseHelperMutex.lock();
    std::vector<PersonBaseEntity> & pbase = _personBaseHelper.getPersonBase();
    _personBaseHelperMutex.unlock();

    double distMin = 1000;
    int id;

    for (unsigned i = 0; i < pbase.size(); i++)
    {
        double dist = dlib::length(pbase[i].getFaceDescriptor() - faceDescriptor);
        if (dist < _settings.getFaceRecognitionThreshold() && dist < distMin)
        {
            distMin = dist;
            id = i;
        }
    }

    if (!(std::fabs(1000 - distMin) < 0.001))
    {

        std::vector<dlib::matrix<float, 0, 1>> ff = {faceDescriptor, pbase[id].getFaceDescriptor()};
        faceDescriptor = dlib::mean(dlib::mat(ff));

        pbase[id].setFaceDescriptor(faceDescriptor);
        std::cout << "[WARNING] " << "Found same person with name: " << pbase[id].getName()
                  << ". This image will be used to improve the recognition of that face." << std::endl;
    }
    else
    {

        PersonBaseEntity newPerson(name, faceDescriptor);
        _personBaseHelperMutex.lock();
        _personBaseHelper.addToBase(newPerson);
        _personBaseHelperMutex.unlock();
    }
}

// PRIVATE
FaceRecognizer::FaceRecognizer()
{
    std::cout << "[LOAD] " << "Loading deep neural network responsible for face recognition." << std::endl;
    loadResNet(_settings.getResnetPath());
}

FaceRecognizer::~FaceRecognizer()
{

}

void FaceRecognizer::loadResNet(const std::string file)
{
    dlib::deserialize(file) >> _network;
}

std::vector<dlib::matrix<dlib::rgb_pixel>> FaceRecognizer::jitterFace(const dlib::matrix<dlib::rgb_pixel> &face)
{
    thread_local dlib::rand rnd;

    std::vector<dlib::matrix<dlib::rgb_pixel>> crops;
    for (int i = 0; i < 100; ++i)
    {
        crops.push_back(dlib::jitter_image(face, rnd));
    }

    return crops;
}

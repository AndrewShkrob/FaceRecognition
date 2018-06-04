#include "Settings.hpp"

// PUBLIC
std::string Settings::getProgramPath() const
{
    return _programPath;
}

std::string Settings::getDlibFloder() const
{
    return _dlibFolder;
}

std::string Settings::getPersonBasePath() const
{
    return _personBasePath;
}

std::string Settings::getShapePredictor68Path() const
{
    return _shapePredictor68Path;
}

std::string Settings::getShapePredictor5Path() const
{
    return _shapePredictor5Path;
}

std::string Settings::getResnetPath() const
{
    return _resnetPath;
}

cv::Scalar Settings::getDetectedFaceFrameColor() const
{
    return _detectedFaceFrameColor;
}

cv::Scalar Settings::getRecognizedFaceFrameColor() const
{
    return _recognizedFaceFrameColor;
}

cv::Scalar Settings::getRecognizedFaceTextColor() const
{
    return _recognizedFaceTextColor;
}

bool Settings::showDetectedFaces() const
{
    return _showDetectedFaces;
}

bool Settings::showRecognizedFaces() const
{
    return _showRecognizedFaces;
}

double Settings::getFaceRecognitionThreshold() const
{
    return _faceRecognitionThreshold;
}

// PRIVATE
Settings::Settings() :
    _settingsPath("/settings.set"),
    _dlibFolder("/dlib")
{
    std::cout << "[LOAD] " << "Loading settings." << std::endl;
    setupProgramPath();
    loadSettings();
}

Settings::~Settings()
{
    std::cout << "[SAVE] " << "Saving settings." << std::endl;
    saveSettings();
}

void Settings::loadSettings()
{
    const std::string settingsPath = _programPath + _settingsPath;

    if (!boost::filesystem::exists(settingsPath))
    {
        std::cout << "[ERROR] Cannot find settings file. Loading default settings." << std::endl;
        useDefaultSettings();
        return;
    }

    settings::SerializedSettings serializedSettings;
    dlib::deserialize(settingsPath) >> serializedSettings;
    deserializeSettings(serializedSettings);
}

void Settings::saveSettings()
{
    const std::string settingsPath = _programPath + _settingsPath;

    const settings::SerializedSettings serializedSettings = serializeSettings();
    dlib::serialize(settingsPath) << serializedSettings;
}

settings::SerializedSettings Settings::serializeSettings()
{
    std::vector<std::string> strings;
    strings.push_back(_personBasePath);
    strings.push_back(_shapePredictor68Path);
    strings.push_back(_shapePredictor5Path);
    strings.push_back(_resnetPath);

    std::vector<settings::SerializedScalar> colors;
    colors.push_back({_detectedFaceFrameColor[0], _detectedFaceFrameColor[1], _detectedFaceFrameColor[2]});
    colors.push_back({_recognizedFaceFrameColor[0], _recognizedFaceFrameColor[1], _recognizedFaceFrameColor[2]});
    colors.push_back({_recognizedFaceTextColor[0], _recognizedFaceTextColor[1], _recognizedFaceTextColor[2]});

    std::vector<short> booleans;
    booleans.push_back(_showDetectedFaces);
    booleans.push_back(_showRecognizedFaces);

    std::vector<double> doubles;
    doubles.push_back(_faceRecognitionThreshold);

    auto firstPair = std::make_pair(std::move(strings), std::move(colors));
    auto secondPair = std::make_pair(std::move(booleans), std::move(doubles));

    return std::make_pair(std::move(firstPair), std::move(secondPair));
}

void Settings::deserializeSettings(const settings::SerializedSettings serializedSettings)
{
    // strings
    _personBasePath = serializedSettings.first.first[0];
    _shapePredictor68Path = serializedSettings.first.first[1];
    _shapePredictor5Path = serializedSettings.first.first[2];
    _resnetPath = serializedSettings.first.first[3];

    // colors
    _detectedFaceFrameColor = cv::Scalar(
                serializedSettings.first.second[0][0],
                serializedSettings.first.second[0][1],
                serializedSettings.first.second[0][2]
    );
    _recognizedFaceFrameColor = cv::Scalar(
                serializedSettings.first.second[1][0],
                serializedSettings.first.second[1][1],
                serializedSettings.first.second[1][2]
    );
    _recognizedFaceTextColor = cv::Scalar(
                serializedSettings.first.second[2][0],
                serializedSettings.first.second[2][1],
                serializedSettings.first.second[2][2]
    );

    // booleans
    _showDetectedFaces = serializedSettings.second.first[0];
    _showRecognizedFaces = serializedSettings.second.first[1];

    // doubles
    _faceRecognitionThreshold = serializedSettings.second.second[0];
}

void Settings::useDefaultSettings()
{
    _personBasePath = _programPath + "/person_base.pbase";
    _shapePredictor5Path = _programPath + _dlibFolder + "/shape_predictor_5_face_landmarks.dat";
    _shapePredictor68Path = _programPath + _dlibFolder + "/shape_predictor_68_face_landmarks.dat";
    _resnetPath = _programPath + _dlibFolder + "/dlib_face_recognition_resnet_model_v1.dat";

    _detectedFaceFrameColor = cv::Scalar(0, 0, 255);
    _recognizedFaceFrameColor = cv::Scalar(255, 0, 0);
    _recognizedFaceTextColor = cv::Scalar(0, 255, 0);

    _showDetectedFaces = false;
    _showRecognizedFaces = true;

    _faceRecognitionThreshold = 0.55;

}

void Settings::setupProgramPath()
{
    _programPath = QDir::currentPath().toUtf8().constData();
}

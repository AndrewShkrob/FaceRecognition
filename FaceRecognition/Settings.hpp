#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <QDir>

#include <iostream>
#include <string>

#include <opencv2/imgproc.hpp>

#include <boost/filesystem.hpp>

#include <dlib/serialize.h>

namespace settings {
    typedef std::vector<double> SerializedScalar;

    // first.first = vector<string>
    // first.second = vector<SerializedScalar>
    // second.first = vector<bool>
    // second.second = vector<double>
    typedef std::pair<std::pair<std::vector<std::string>, std::vector<SerializedScalar>>,
                      std::pair<std::vector<short>, std::vector<double>>> SerializedSettings;
}

class Settings
{
public:
    static Settings & getInstance()
    {
        static Settings settings;
        return settings;
    }

    Settings (Settings const &) = delete;
    void operator = (Settings const &) = delete;

    std::string getProgramPath() const;
    std::string getDlibFloder() const;
    std::string getPersonBasePath() const;
    std::string getShapePredictor68Path() const;
    std::string getShapePredictor5Path() const;
    std::string getResnetPath() const;

    cv::Scalar getDetectedFaceFrameColor() const;
    cv::Scalar getRecognizedFaceFrameColor() const;
    cv::Scalar getRecognizedFaceTextColor() const;

    bool showDetectedFaces() const;
    bool showRecognizedFaces() const;

    double getFaceRecognitionThreshold() const;

private:
    std::string _programPath;

    const std::string _settingsPath;
    const std::string _dlibFolder;

    std::string _personBasePath;
    std::string _shapePredictor68Path;
    std::string _shapePredictor5Path;
    std::string _resnetPath;

    cv::Scalar _detectedFaceFrameColor;
    cv::Scalar _recognizedFaceFrameColor;
    cv::Scalar _recognizedFaceTextColor;

    bool _showDetectedFaces;
    bool _showRecognizedFaces;

    double _faceRecognitionThreshold;

    Settings();
    ~Settings();

    void loadSettings();
    void saveSettings();

    settings::SerializedSettings serializeSettings();
    void deserializeSettings(const settings::SerializedSettings serializedSettings);

    void useDefaultSettings();

    void setupProgramPath();
};

#endif // SETTINGS_HPP

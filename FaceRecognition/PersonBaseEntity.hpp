#ifndef PERSON_HPP
#define PERSON_HPP

#include <dlib/clustering.h>

#include <string>

typedef std::pair<std::string, dlib::matrix<float, 0, 1>> SerializedPersonBaseEntity;

class PersonBaseEntity
{
public:
    PersonBaseEntity();
    PersonBaseEntity(const std::string name, const dlib::matrix<float, 0, 1> faceDescriptor);
    PersonBaseEntity(const SerializedPersonBaseEntity person);
    ~PersonBaseEntity();

    const std::string getName() const;
    const dlib::matrix<float, 0, 1> getFaceDescriptor() const;

    void setName(const std::string name);
    void setFaceDescriptor(const dlib::matrix<float, 0, 1> faceDescriptor);

private:
    dlib::matrix<float, 0, 1> _faceDescriptor;
    std::string _name;
};

#endif // PERSON_HPP

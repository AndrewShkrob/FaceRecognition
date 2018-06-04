#include "PersonBaseEntity.hpp"

// PUBLIC
PersonBaseEntity::PersonBaseEntity(const std::string name, const dlib::matrix<float, 0, 1> faceDescriptor)
{
    _name = name;
    _faceDescriptor = faceDescriptor;
}

PersonBaseEntity::PersonBaseEntity(const SerializedPersonBaseEntity person)
{
    _name = person.first;
    _faceDescriptor = person.second;
}

PersonBaseEntity::~PersonBaseEntity()
{

}

const std::string PersonBaseEntity::getName() const
{
    return _name;
}

const dlib::matrix<float, 0, 1> PersonBaseEntity::getFaceDescriptor() const
{
    return _faceDescriptor;
}

void PersonBaseEntity::setName(const std::string name)
{
    _name = name;
}

void PersonBaseEntity::setFaceDescriptor(const dlib::matrix<float, 0, 1> faceDescriptor)
{
    _faceDescriptor = faceDescriptor;
}

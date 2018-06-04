#include "PersonBaseHelper.hpp"

// PUBLIC

void PersonBaseHelper::addToBase(const PersonBaseEntity person)
{
    _persons.push_back(person);
}

std::vector<PersonBaseEntity> & PersonBaseHelper::getPersonBase()
{
    return _persons;
}

// PRIVATE
PersonBaseHelper::PersonBaseHelper()
{
    std::cout << "[LOAD] " << "Loading person base." << std::endl;
    loadBase();
}

PersonBaseHelper::~PersonBaseHelper()
{
    std::cout << "[SAVE] " << "Saving person base." << std::endl;
    saveBase();
}

void PersonBaseHelper::loadBase()
{
    if (!boost::filesystem::exists(Settings::getInstance().getPersonBasePath()))
    {
        return;
    }

    std::vector<SerializedPersonBaseEntity> array;
    dlib::deserialize(Settings::getInstance().getPersonBasePath()) >> array;
    _persons = deserializePersons(array);
}

void PersonBaseHelper::saveBase()
{
    if (_persons.size() == 0)
    {
        return;
    }

    const std::string path = Settings::getInstance().getPersonBasePath();
    std::vector<SerializedPersonBaseEntity> array;
    array = serializePersons();
    dlib::serialize(path) << array;
}

std::vector<SerializedPersonBaseEntity> PersonBaseHelper::serializePersons()
{
    std::vector<SerializedPersonBaseEntity> serializedPersons;

    for (unsigned i = 0; i < _persons.size(); i++)
    {
        std::cout << "[SERIALIZE] " << "Serializing person with name " << _persons[i].getName() << "." << std::endl;
        serializedPersons.push_back(std::make_pair(_persons[i].getName(), _persons[i].getFaceDescriptor()));
    }

    return serializedPersons;
}

std::vector<PersonBaseEntity> PersonBaseHelper::deserializePersons(const std::vector<SerializedPersonBaseEntity> serializedPersons)
{
    std::vector<PersonBaseEntity> persons;

    for (unsigned i = 0; i < serializedPersons.size(); i++)
    {
        std::cout << "[DESERIALIZE] " << "Deserializing person with name " << serializedPersons[i].first << "." << std::endl;
        persons.push_back(PersonBaseEntity(serializedPersons[i]));
    }

    return persons;
}

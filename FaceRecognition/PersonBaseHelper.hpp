#ifndef PERSONBASEHELPER_HPP
#define PERSONBASEHELPER_HPP

#include <iostream>
#include <vector>

#include <boost/filesystem.hpp>

#include "PersonBaseEntity.hpp"
#include "Settings.hpp"

class PersonBaseHelper
{
public:
    static PersonBaseHelper & getInstance()
    {
        static PersonBaseHelper personBaseHelper;
        return personBaseHelper;
    }

    PersonBaseHelper (PersonBaseHelper const &) = delete;
    void operator = (PersonBaseHelper const &) = delete;

    void addToBase(const PersonBaseEntity person);

    std::vector<PersonBaseEntity> & getPersonBase();

private:
    std::vector<PersonBaseEntity> _persons;

    PersonBaseHelper();
    ~PersonBaseHelper();

    void loadBase();
    void saveBase();

    std::vector<SerializedPersonBaseEntity> serializePersons();
    std::vector<PersonBaseEntity> deserializePersons(const std::vector<SerializedPersonBaseEntity> serializedPersons);

};

#endif // PERSONBASEHELPER_HPP

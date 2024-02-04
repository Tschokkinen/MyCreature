#define _CRT_SECURE_NO_WARNINGS
#include "Creature.h"
#include <thread>
#include "Helpers.h"

void Creature::changeName()
{
    std::cout << "Give your creature a name:\n";
    char* newName = const_cast<char*>(name_ptr);
    char tmpName[256]{ '\0' };
    std::cin >> tmpName;
    // Capitalize first letter.
    CapFirstLetter(tmpName);
    strcpy(newName, tmpName);
}

const char* Creature::getName() const
{
    return name_ptr;
}

void Creature::feed()
{
    std::cout << "You feed " << name << '\n';
    energyLevel += 1.5;
};

const double* Creature::getEnergyLevel() const
{
    const double* currentFoodLevel = &energyLevel;
    return currentFoodLevel;
}

void Creature::consumeEnergy(std::atomic<bool>& running)
{
    while (running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(consumeEnergyTimer));
        if (energyLevel > 0) {
            energyLevel -= 0.5;
        }
        else
        {
            std::cout << name << " needs food!\n";
        }
    }
}

void Creature::pet()
{
    start = std::chrono::high_resolution_clock::now();
    isSatisfied = true;
};

//Creature::MOOD Creature::getMood() {
//    if (isSatisfied) {
//        return Creature::MOOD::HAPPY;
//    }
//    else if (!isSatisfied) {
//        return Creature::MOOD::SAD;
//    }
//}

const char* Creature::getMood()
{
    const char* happy = "Happy";
    const char* sad = "Sad";
    const char* moodError = "Mood error";
    char* newMood = const_cast<char*>(currentMood_ptr);

    if (isSatisfied)
    {
        strcpy(newMood, happy);
    }
    else if (!isSatisfied)
    {
        strcpy(newMood, sad);
    }
    else
    {
        strcpy(newMood, moodError);
    }

    return currentMood_ptr;
}

void Creature::setMood(std::atomic<bool>& running)
{
    // Compare two different times here: Current time and time since isSatisfied was set to true.
    while (running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(setMoodTimer));
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> timeSpan = duration_cast<std::chrono::duration<double>>(end - start);
        if (timeSpan.count() > 5 && !stopStatusUpdate)
        {
            std::cout << "Your creature is sad. Try petting it.\n";
            isSatisfied = false;
        }
    }
}

void Creature::getStats()
{
    std::cout << "\n########\n";
    std::cout << "My creature's name: " << getName() << '\n';
    std::cout << "Food level: " << *getEnergyLevel() << '\n';
    std::cout << "Mood: " << getMood() << '\n';
    std::cout << "########" << '\n';
}

void Creature::loadCreature(Creature* myCreature, std::string& existingCreature)
{
    // If existingCreature string is empty, create a new creature
    if (existingCreature.empty())
    {
        std::cout << "No creature name given... Creating a new creature.\n";
        myCreature->changeName();
        return;
    };

    // Find existing creature file
    //std::string creatureName{ existingCreature };
    boost::algorithm::to_lower(existingCreature);
    std::ifstream creatureData(existingCreature + ".txt");
    if (!creatureData) std::cout << "Error opening file" << '\n';

    if (creatureData.is_open())
    {
        std::string line;
        for (int i{}; getline(creatureData, line); i++)
        {
            switch (i)
            {
            case 0:
                myCreature->setNameInFile(line);
                break;
            case 1:
                myCreature->setEnergyLevelInFile(stod(line));
                break;
            case 2:
                myCreature->setMoodInFile(line);
                break;
            case 3:
                myCreature->isSatisfied = stoi(line);
            }
        }
    }
    else
    {
        std::cout << "No previous creature found.\n";
        myCreature->changeName();
    }
    creatureData.close();
}

void Creature::saveCreature(Creature* myCreature)
{
    std::string creatureName{ myCreature->getName()};
    boost::algorithm::to_lower(creatureName);
    std::filesystem::remove(creatureName + ".txt");

    std::ofstream creatureData(creatureName + ".txt", std::ios::out);

    if (creatureData.is_open())
    {
        creatureData << myCreature->getName() << '\n' << *myCreature->getEnergyLevel() << '\n' << myCreature->getMood() << '\n' << myCreature->isSatisfied;
    }

    creatureData.close();
}

// Data management functions.
void Creature::setNameInFile(std::string nameInFile)
{
    char* newName = const_cast<char*>(name_ptr);
    // Capitalize first letter.
    CapFirstLetter(newName);
    strcpy(newName, nameInFile.c_str());
}

void Creature::setMoodInFile(std::string moodInFile)
{
    char* newMood = const_cast<char*>(currentMood_ptr);

    strcpy(newMood, moodInFile.c_str());
}

void Creature::setEnergyLevelInFile(double energyLevelInFile)
{
    energyLevel = energyLevelInFile;
};
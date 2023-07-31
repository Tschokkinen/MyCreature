#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <thread>
#include <atomic>
#include <chrono>
#include <ratio>
#include <ctime>
#include <string>
#include <limits>
#include <filesystem>
//using namespace std;

class Creature 
{
public:
    Creature() : energyLevel(defaultFoodLevel), isSatisfied(true){};

    void changeName(); // Change creature's name.
    const char* getName() const; // Return pointer to creature's name.
    void feed(); // Feeds creature and gives more energy.
    const double* getEnergyLevel(); // Return pointer to current energy level.
    void consumeEnergy(std::atomic<bool> &running); // Consumes energy while program is running.
    void pet(); // Pets the creature and improves creature's mood.
    const char* getMood(); // Returns a pointer to creature's mood level.
    void setMood(std::atomic<bool> &running); // Sets the mood of the creature at given intervals.
    void getStats(); // Get creature stats (name, age, energy level etc.)

    // Data management.
    void saveCreature(Creature* myCreature);
    void loadCreature(Creature* myCreature);

    // Functions for data loading.
    void setNameInFile(std::string nameInFile);
    void setMoodInFile(std::string moodInFile);
    void setEnergyLevelInFile(double energyLevelInFile);

    enum MOOD
    {
        SAD = 0,
        HAPPY = 1
    };

    enum SELECTION 
    {
        QUIT = 0,
        CHANGE_NAME = 1,
        FEED = 2,
        PET = 3,
        GET_STATS = 4
    };

private:
    //int age;
    double energyLevel;
    const double defaultFoodLevel{ 5.0 };

    const char name[256]{ '\0' };
    const char* name_ptr = name;

    const char currentMood[10]{ '\0' };
    const char* currentMood_ptr = currentMood;

    bool isSatisfied;

    const int setMoodTimer = 5000;
    const int consumeEnergyTimer = 5000;

    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point end;
};

void Creature::changeName()
{
    std::cout << "Give your creature a name:\n";
    char* newName = const_cast<char*>(name_ptr);
    char tmpName[256]{ '\0' };
    std::cin >> tmpName;
    strcpy(newName, tmpName);
    //std::cout << "My name is now " << name << std::endl;
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

const double* Creature::getEnergyLevel()
{
    const double* currentFoodLevel = &energyLevel;
    return currentFoodLevel;
}

void Creature::consumeEnergy(std::atomic<bool> &running) 
{
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(consumeEnergyTimer));
        if (energyLevel > 0) {
            energyLevel -= 0.5;
        }
        else {
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

void Creature::setMood(std::atomic<bool> &running) 
{
    // Compare two different times here: Current time and time since isSatisfied was set to true.
    while (running) 
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(setMoodTimer));
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> timeSpan = duration_cast<std::chrono::duration<double>>(end - start);
        if (timeSpan.count() > 5)
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

void Creature::saveCreature(Creature* myCreature)
{
    std::filesystem::remove("creature_data.txt");
    //static const char* const testFile = "creature_data.txt";
    std::ofstream creatureData("creature_data.txt");
    //fout.open(testFile);
    if (creatureData.is_open())
    {
        creatureData << myCreature->getName() << '\n' << *myCreature->getEnergyLevel() << '\n' << myCreature->getMood() << '\n' << myCreature->isSatisfied;
    }

    creatureData.close();
}

void Creature::loadCreature(Creature* myCreature)
{
    std::ifstream creatureData("creature_data.txt");
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

// Data management functions.
void Creature::setNameInFile(std::string nameInFile)
{
    char* newName = const_cast<char*>(name_ptr);
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




// Function pointer for energy consumption.
void (consumeEnergy_ptr)(Creature* myCreature, void(Creature::*consumeEnergy)(std::atomic<bool>&), std::atomic<bool>& running) 
{
    (myCreature->*consumeEnergy)(running);
}

// Function pointer for current mood setting.
void (setMood_ptr)(Creature* myCreature, void(Creature::* setMood)(std::atomic<bool>&), std::atomic<bool> &running) 
{
    (myCreature->*setMood)(running);
}

int main()
{
    std::cout << "Welcome to MyCreature.\n";

    Creature myCreature{};

    myCreature.loadCreature(&myCreature);

    std::atomic<bool> running{ true }; // If program is quit, set to false.
    std::thread t1(consumeEnergy_ptr, &myCreature, &Creature::consumeEnergy, std::ref(running));
    std::thread t2(setMood_ptr, &myCreature, &Creature::setMood, std::ref(running));

    myCreature.getStats();

    while (true) 
    {
        int selection;
        std::cout << "What do you want to do (enter number): \n";
        std::cout << "1) Change your creature's name.\n";
        std::cout << "2) Feed your creature.\n";
        std::cout << "3) Pet your creature.\n";
        std::cout << "4) Get stats.\n";
        std::cout << "0) Quit.\n";
        std::cin >> selection;

        if (selection == Creature::CHANGE_NAME) 
        {
            myCreature.changeName();
            continue;
        }
        else if (selection == Creature::FEED) 
        {
            myCreature.feed();
            continue;
        }
        else if (selection == Creature::PET) 
        {
            myCreature.pet();
            continue;
        }
        else if (selection == Creature::GET_STATS) 
        {
            myCreature.getStats();
        }
        else if (selection == Creature::QUIT) 
        {
            running = false;
            t1.join();
            t2.join();
            break;
        }
        else 
        {
            continue;
        }
    }

    myCreature.saveCreature(&myCreature);
    return 0;
}
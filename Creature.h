#pragma once
#include<atomic>
#include<string>
#include<chrono>
#include<iostream>
#include<filesystem>
#include<fstream>
#include<boost/algorithm/string.hpp>

class Creature
{
public:
    Creature() : energyLevel(defaultFoodLevel), isSatisfied(true) {};

    void changeName(); // Change creature's name.
    const char* getName() const; // Return pointer to creature's name.
    void feed(); // Feeds creature and gives more energy.
    const double* getEnergyLevel() const; // Return pointer to current energy level.
    void consumeEnergy(std::atomic<bool>& running); // Consumes energy while program is running.
    void pet(); // Pets the creature and improves creature's mood.
    const char* getMood(); // Returns a pointer to creature's mood level.
    void setMood(std::atomic<bool>& running); // Sets the mood of the creature at given intervals.
    void getStats(); // Get creature stats (name, age, energy level etc.)

    // Data management.
    void saveCreature(Creature* myCreature);
    //void loadCreature(Creature* myCreature);
    void loadCreature(Creature* myCreature, std::string& creatureName);

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

    bool stopStatusUpdate{ false };

private:
    //int age;
    double energyLevel;
    const double defaultFoodLevel{ 5.0 };

    const char name[256]{ '\0' };
    const char* name_ptr = name;

    const char currentMood[10]{ '\0' };
    const char* currentMood_ptr = currentMood;

    bool isSatisfied;

    const int setMoodTimer{ 5000 };
    const int consumeEnergyTimer{ 5000 };

    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point end;
};




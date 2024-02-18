#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include <Windows.h>
#include <thread>
#include <atomic>
#include <ratio>
#include <string>
#include <limits>
#include <future>
#include <chrono>

#include "Creature.h"
//using namespace std;

// Function pointer for energy consumption.
void (consumeEnergy_ptr)(Creature* myCreature, void(Creature::* consumeEnergy)(std::atomic<bool>&), std::atomic<bool>& running)
{
    (myCreature->*consumeEnergy)(running);
}

// Function pointer for current mood setting.
void (setMood_ptr)(Creature* myCreature, void(Creature::* setMood)(std::atomic<bool>&), std::atomic<bool>& running)
{
    (myCreature->*setMood)(running);
}

void sayGoodbye(void)
{
    std::cout << "Goodbye!" << std::endl;
}

void clearScreen()
{
    system("CLS");
}



//////////////////////////////////////

void ClearLine(int lineNumber) {
    std::cout << "\033[" << lineNumber << ";0H";  // Move to the specified line
    std::cout << "\033[2K";  // Clear the line
    std::cout << "content" << "\r";  // Print the updated content and move back to the beginning of the line
    std::cout.flush();  // Ensure the output is flushed
}
//////////////////////////////////////


std::string existingCreature{};

int main(int argc, const char** argv)
{
    if (argc == 1)
    {
        std::cout << "Welcome to MyCreature.\n";
        std::cout << "To load an existing creature, please enter its name.\n";
        std::cout << "To create a new creature press enter.\n";

        std::getline(std::cin, existingCreature);
    }
    else if (argc > 1)
    {
        existingCreature = argv[1];
        std::cout << "Loading existing creature: " << existingCreature << std::endl;
    }

    Creature myCreature{};

    myCreature.loadCreature(&myCreature, existingCreature);

    std::atomic<bool> running{ true }; // If program is quit, set to false.
    std::thread t1(consumeEnergy_ptr, &myCreature, &Creature::consumeEnergy, std::ref(running));
    std::thread t2(setMood_ptr, &myCreature, &Creature::setMood, std::ref(running));

    std::atexit(sayGoodbye);

    clearScreen();

    myCreature.getStats();

    while (true)
    {
        int selection{};
        clearScreen();
        myCreature.getStats();

        std::cout << "What do you want to do (enter number): \n";
        std::cout << "1) Change your creature's name.\n";
        std::cout << "2) Feed your creature.\n";
        std::cout << "3) Pet your creature.\n";
        std::cout << "4) Update stats.\n";
        std::cout << "0) Quit.\n";

        std::cin >> selection;

        if (selection == Creature::CHANGE_NAME)
        {
            clearScreen();
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
            //myCreature.getStats();
            continue;
        }
        else if (selection == Creature::QUIT)
        {
            running = false;
            myCreature.stopStatusUpdate = true;
            clearScreen();
            myCreature.saveCreature(&myCreature);
            t1.join();
            t2.join();
            break;
        }
        else
        {
            continue;
        }
    }
    return 0;
}
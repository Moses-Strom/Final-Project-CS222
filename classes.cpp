#include "classes.h"
#include <fstream>

void Character::save(std::ofstream& file)
{
    file << characterClass << "\n";
    file << health << " " << max_health << "\n";
    file << attack << " " << defense << " " << speed << "\n";
    file << defending << " " << dodge_next << "\n";
}

void Character::load(std::ifstream& file)
{
    file >> health >> max_health;
    file >> attack >> defense >> speed;
    file >> defending >> dodge_next;
}
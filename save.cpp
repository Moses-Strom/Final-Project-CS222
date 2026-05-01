#include <fstream>
#include <iostream>
#include <string>
#include "save.h"

//save class stats inventory and progress in dungeon

void save_game(Character* player, int potions, int floor) {
	std::ofstream save_file("save_file.txt");
    if (!save_file.is_open()) {
        std::cout << "No save file created.";
    }

	player->save(save_file);
	save_file << potions << "\n";
	save_file << floor << "\n";

	save_file.close();
}

Character* load_game(int& potions, int& floor)
{
    std::ifstream save_file("save_file.txt");

    if (!save_file.is_open())
    {
        std::cout << "No save file found.\n";
        return nullptr;
    }

    std::string type;
    save_file >> type;

    Character* player = nullptr;

    if (type == "Warrior") {
        Warrior player;
    }
    else if (type == "Wizard") {
        Wizard player;
    } 
    else if (type == "Archer") {
        Archer player;
    }
    else if (type == "Thief") {
        Thief player;
    }
    else {
        std::cout << "Corrupt save.\n";
        return nullptr;
    }

    int health, max_health;
    int attack, defense, speed;
    bool defending, dodge_next;

    save_file >> health >> max_health;
    save_file >> attack >> defense >> speed;
    save_file >> defending >> dodge_next;

    player->health = health;
    player->max_health = max_health;
    player->attack = attack;
    player->defense = defense;
    player->speed = speed;
    player->defending = defending;
    player->dodge_next = dodge_next;

    save_file >> potions;
    save_file >> floor;

    save_file.close();

    return player;
}

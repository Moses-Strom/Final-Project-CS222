#include <fstream>
#include <iostream>
#include <string>
#include "save.h"

// save class stats inventory and progress in dungeon

void save_game(Character* player, int potions, int current_floor, const std::string& encounter_order) {
    if (player == nullptr) {
        std::cout << "No game to save.\n";
        return;
    }

    std::ofstream save_file("save_file.txt");
    if (!save_file.is_open()) {
        std::cout << "No save file created.";
        return;
    }

    player->save(save_file);
    save_file << potions << "\n";
    save_file << current_floor << "\n";
    save_file << encounter_order << "\n";

    save_file.close();
}

Character* load_game(int& potions, int& current_floor, std::string& encounter_order)
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
        player = new Warrior();
    }
    else if (type == "Wizard") {
        player = new Wizard();
    }
    else if (type == "Archer") {
        player = new Archer();
    }
    else if (type == "Thief") {
        player = new Thief();
    }
    else {
        std::cout << "Corrupt save.\n";
        return nullptr;
    }

    player->load(save_file);

    save_file >> potions;
    save_file >> current_floor;
    encounter_order = "";
    save_file >> encounter_order;

    save_file.close();

    return player;
}

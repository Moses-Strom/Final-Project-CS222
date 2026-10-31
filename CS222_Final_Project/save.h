#pragma once
#include <string>
#include "classes.h"

Character* load_game(int& potions, int& current_floor, std::string& encounter_order);
void save_game(Character* player, int potions, int current_floor, const std::string& encounter_order);

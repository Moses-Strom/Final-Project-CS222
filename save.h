#pragma once
#include "classes.h"

Character* load_game(int& potions, int& floor);
void save_game(Character* player, int potions, int floor);

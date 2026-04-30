#include "classes.h"
#include <iostream>
#include <conio.h>
#include <chrono>
#include <thread>
#include <vector>
#include <cstdlib>
#include <string>

int player_potions = 3;
bool quit_to_menu = false;

void clear_screen()
{
    system("cls");
}

int pause_menu()
{
    clear_screen();

    std::cout << "==== PAUSED ====\n";
    std::cout << "[1] Resume\n";
    std::cout << "[2] Quit to Main Menu\n";

    while (true)
    {
        char input = _getch();               // instant key press

        if (input == '1') return 1; // resume
        if (input == '2') return 2; // quit
                                             // everything else is ignored
    }
}

int get_choice()
{
    while (true)
    {
        char input = _getch();               // instant key press

        if (input == 27) return 0;             // esc key
        if (input == '1') return 1;
        if (input == '2') return 2;
        if (input == '3') return 3;
        if (input == '4') return 4;

                                              // everything else is ignored
    }
}

std::vector<int> generate_sequence(int size)
{
    std::vector<int> seq;
    for (int i = 0; i < size; i++)
    {
        int r = rand() % 4;

        if (r == 0) seq.push_back(72); // up
        if (r == 1) seq.push_back(80); // down
        if (r == 2) seq.push_back(75); // left
        if (r == 3) seq.push_back(77); // right
    }
    return seq;
}

bool run_seq(const std::vector<int>& seq)
{

    std::cout << "\nPress the Arrow keys-\nRepeat the sequence!\n"; // was using endl at first, but it was a pain to type out

    for (int i = 0; i < seq.size(); i++)                    // prints arrows to screen
    {
        if (seq[i] == 72)    std::cout << "^ ";
        if (seq[i] == 80)  std::cout << "v ";
        if (seq[i] == 75)  std::cout << "< ";
        if (seq[i] == 77) std::cout << "> ";
    }

    std::cout << "\n";

    using clock = std::chrono::steady_clock;                    // clock
    const int time_limit_ms = 1200;                             // time limit per input

    std::string RED = "\033[31m";                               // color of bar
    std::string RESET = "\033[0m";                              // reset color

    for (int i = 0; i < seq.size(); i++)                        // loop for each arrow key that's supposed to be pressed
    {
        std::chrono::steady_clock::time_point start = clock::now();    // marking the time when player starts reacting

        while (true)                                               
        {
            std::cout << "\033[?25l";                                    // hide cursor cuz it was flashing all over

            std::chrono::steady_clock::time_point now = clock::now();                                   // what time is it now

            int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();   //time passed

            int remaining = time_limit_ms - elapsed;                //max time - elapsed

            if (remaining <= 0)
                return false;                                       // ran out of time

            // VISUAL BAR FOR TIMR
            int width = 20;
            int filled = (remaining * width) / time_limit_ms;

            std::cout << "\r" << RED << "[";

            for (int j = 0; j < width; j++)                         //prints loading bar
            {
                if (j < filled)
                    std::cout << "-";
                else
                    std::cout << " ";
            }

            std::cout << "]" << RESET << "   " << std::flush;

            if (_kbhit())
            {
                int input = _getch();       // instant key press that checks for special key press

                if (input == 224)           // arrow keys are special keys, which means they have 2 inputs, 224 and then the # for their code which u can see above
                {
                    int arrow = _getch();  // insteant key press to see what special key is pressed

                    if (arrow != seq[i])  // checks if input matches arrow key in vector
                        return false;
                        std::cout << "\033[?25h";           // show cursor
                    break;                      // move to next arrow key
                }
                else
                {
                    std::cout << "\033[?25l";            // show cursor
                    return false;                       // if not special key - false
                }
            }
        }
    }
    std::cout << "\033[?25h";           // show cursor
    return true;                        // true after passing 2 checks
}

std::string health_bar(int hp, int max_hp)
{
    std::string RESET = "\033[0m";
    std::string RED = "\033[31m";
    std::string GREEN = "\033[32m";

    int width = 20;
    int filled = (hp * width) / max_hp;

    std::string bar = "[";

    for (int i = 0; i < width; i++) // remaining health is green, health lost is red
    {
        if (i < filled)
            bar += GREEN + "#" + RESET;
        else
            bar += RED + "#" + RESET;
    }

    bar += "]";
    return bar;
}

void wait(int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void type_text(const std::string& text, int delay_ms) // prints at "delay_ms" so it doesn't appear all at once
{
    for (int i = 0; i < text.length(); i++)
    {
        std::cout << text[i] << std::flush;
        wait(delay_ms);
    }
}

void print_action(const std::string& text) //prints type_text in the context of combat
{
    type_text(text, 18);   
    std::cout << "\n";
    wait(400);             
}

void render_ui(Character* player, Enemy& enemy, const std::string& action_text) // the fighting ui
{
    clear_screen();

    std::cout << "==================================================\n";

    std::cout << "ENEMY\t\t\t|| PLAYER\n";                                      // I tried doing setw() but it was a pain so i just did tabs and newlines
    std::cout << enemy.name << "\t\t|| " << player->characterClass  << "\n";

    std::cout << "HP: "
        << health_bar(enemy.health, enemy.max_health)
        << " " << enemy.health << "\t|| ";

    std::cout << "HP: "
        << health_bar(player->health, player->max_health)
        << " " << player->health << "\n";

    std::cout << "--------------------------------------------------\n";

    std::cout << "ACTION: " << action_text << "\n";

    std::cout << "--------------------------------------------------\n";

    std::cout << "[1] Attack   [2] Defend   [3] Ability   [4] Item";
    std::cout << " (Potions: " << player_potions << ")\n";

    std::cout << "==================================================\n";
}

//ABILTIES  
template <typename T, typename E>
void wizard(T* player, E* enemy)
{
    if (run_seq(generate_sequence(6)))
    {
        int dmg = player->get_attack() * 2; // 2x dmg
        enemy->health -= enemy->take_damage(dmg);

        print_action("\nYou Arcane Surge!\n");
    }
    else
    {
        print_action("\nAbility failed!");
    }
}
template <typename T, typename E>
void warrior(T* player, E* enemy)
{
    if(run_seq(generate_sequence(5)))
    {
        int dmg = player->get_attack() + 10; // flat boost
        enemy->health -= enemy->take_damage(dmg);

        print_action("\nPower Strike lands!");
    }
    else
    {
        print_action("\nAbility failed!");
    }
}

template <typename T, typename E>
void archer(T* player, E* enemy)
{
    if (run_seq(generate_sequence(5)))
    {
        int dmg = player->get_attack() + (player->get_attack() / 2);    // 1.5x dmg
        enemy->health -= enemy->take_damage(dmg);

        print_action("\nPrecision Shot lands!");
    }
    else
    {
        print_action("\nAbility failed!");
    }
}

template <typename T>
void thief(T* player)
{
    if (run_seq(generate_sequence(6)))
    {
        player->dodge_next = true;
        print_action("\nYou prepare to dodge the next attack!");
    }
    else
    {
        print_action("\nAbility failed!");
    }
}

template <typename T>                  // template makes it so you can pass in any class for combat as "player"
combat_result combat(T* player)
{
    Enemy enemy;

    std::string action;
    print_action("A enemy appears!");

    //FIRST?
    bool player_first = (player->get_speed() >= enemy.get_speed());
    bool first_turn = true;

    if (player_first)
        print_action("You are faster and act first!");
    else
        print_action("The enemy is faster and acts first!");

    //MAKE SCREEN
    render_ui(player, enemy, action);
    wait(700);

    int input = 0;

    while (player->get_health() > 0 && enemy.get_health() > 0)
    {
        // PLAYER TURN
            if (first_turn && !player_first)                   //if player not first, it set's input to 11 which is impossible to type with get_choice
                input = 11;
            else
                input = get_choice();

                // PAUSE MENU
                if (input == 0)
                {
                    int result = pause_menu();

                    if (result == 1)
                    {
                        // resume 
                        render_ui(player, enemy, "Resuming...");
                        wait(300);
                        continue;
                    }
                    else if (result == 2)
                    {
                        // quit
                        quit_to_menu = true;
                        return combat_result::lose;
                        //////////////////////////////////////////////////////////////////////
                    }
                }
            //PLAYER ATTACK
                else if (input == 1)
                {
                    if (run_seq(generate_sequence(4)))
                    {
                        int dmg = player->get_attack();
                        enemy.health -= enemy.take_damage(dmg);
                        print_action("\nYou land a clean attack!");
                    }
                    else
                    {
                        print_action("\nYou missed your attack!");
                    }
                }
            //PLAYER DEFEND
                else if (input == 2)
                {
                    if (run_seq(generate_sequence(3)))
                    {
                        player->defending = true;
                        print_action("\nYou brace for incoming damage!");
                    }
                    else
                    {
                        print_action("\nYou failed to defend!");
                    }
                }
            //PLAYER ABILITY
                else if (input == 3)
                {
                    if (player->characterClass == "Wizard")
                    {
                        wizard(player, &enemy);
                    }
                    else if (player->characterClass == "Warrior")
                    {
                        warrior(player, &enemy);
                    }
                    else if (player->characterClass == "Archer")
                    {
                        archer(player, &enemy);
                    }
                    else if (player->characterClass == "Thief")
                    {
                        thief(player);
                    }
                }
            //PLAYER INVENTORY (potion)
                else if (input == 4)
                {
                    if (player_potions <= 0)
                    {
                        print_action("You have no potions!");
                    }
                    else
                    {
                        player_potions--;
                        player->health += 30;
                        if (player->health > player->max_health)
                        {
                            player->health = player->max_health;
                        }
                        print_action("You drink a potion and recover HP!");
                    }
                }
            //PLAYER NOT FIRST
                else if (input == 11) {}

            if (enemy.get_health() <= 0)
                return combat_result::win;

        //REMAKE SCREEN TO UPDATE COMBAT
        render_ui(player, enemy, "Enemy preparing response...");
        wait(500);

        // ENEMY TURN
            if (enemy.is_charging)
            {
                enemy.is_charging = false;

                int dmg = enemy.get_attack() * 2 + (rand() % 9 - 4);               // 2x then between +4, -4

                if (player->dodge_next)
                {
                    player->dodge_next = false;
                    print_action("\nYou dodged the charged attack!");
                }
                else
                {
                    player->health -= player->take_damage(dmg);
                    print_action("\nEnemy unleashes a CHARGED attack!");
                }
            }
            else if (rand() % 100 < 30)
            {
                enemy.is_charging = true;
                print_action("\nEnemy begins charging a powerful attack...");
            }
            else
            {
                int dmg = enemy.get_attack() + (rand() % 5 - 2);                 // between +2, -2

                if (player->dodge_next)
                {
                    player->dodge_next = false;
                    print_action("\nYou dodged the attack!");
                }
                else
                {
                    player->health -= player->take_damage(dmg);
                    print_action("\nEnemy attacks!");
                }
            }

            if (player->get_health() <= 0)
                return combat_result::lose;
            //REMAKE SCREEN TO UPDATE COMBAT
            render_ui(player, enemy, "Press a key to continue...");
        
        first_turn = false;
    }

    return combat_result::lose;                                             // for compiler reasons (never called)
}

//READ THIS FIRST - don't make changes to this file, just copy it and then change it

//MAIN MENU
//needs main menu system that u return to after u die or win
// ex: switch case in a while loop
// a. Start new game (make it so it rewrites save file after u select this
// b. Continue (loads info save file, if save file doesn't have a save/not full info, prompts to start a new game)
// c. Instructions/help (explain the premise of the game, explains each part of combat, explains puzzles/challeneges, btw dont explain theres a final boss)
// d. credits (just our names prob idk)
// e. quit (file save after u choose quit option, no choose quit = no save)

//INVENTORY
//needs inventory implementation (prob just potions cuz complexity increases a shi ton if u try to add more stuff, but up 2 u)
// ex: start with 3 potions, update the csv when one is used
// csv ex: ", potions, 3" 
// (implement check in combat function, if potions 0 then print "You have no potions!" using print_action)

//PROGRESSION
//needs loop of multiple battles/challenges (prob 6 battles and idk how many challenges, up 2 u)
//side note becareful of implementation of challenges, try not to change the player's health if u can bc it affects balancing (maybe make it pass/fail? idk)

//STRETCH GOALS
// I did the quick time events and the pause menu
// final boss needs to be implemented (you'll prob want to make a diff function for it other than combat, but alot of the stuff i used for it u can copy over to help, or just make it ur own)
// also for the final boss, try to make it as best as u can, if u wan disregard what i wrote and make ur own sick battle, go for it. just make sure it fits the proj criteria, which is pretty loose so you should be fine
// also also, for final boss make player heal to full hp, so easier to balance
// u can do more of the ones we had if u want more stuff to talk abt in the presentation

//if u have any suggestions or edits lmk

int main_menu()
{
    clear_screen();

    std::cout << "==== MAIN MENU ====\n";
    std::cout << "[1] Start New Game\n";
    std::cout << "[2] Instructions / Help\n";
    std::cout << "[3] Credits\n";
    std::cout << "[4] Quit\n";

    while (true)
    {
        char input = _getch();

        if (input == '1') return 1;
        if (input == '2') return 2;
        if (input == '3') return 3;
        if (input == '4' || input == 27) return 4;
    }
}

void wait_for_menu()
{
    std::cout << "\nPress any key to return to the main menu...";
    _getch();
}

void show_instructions()
{
    clear_screen();

    std::cout << "==== INSTRUCTIONS ====\n";
    std::cout << "You play through quick-time battles by matching the arrow key sequence on screen.\n";
    std::cout << "If you press the wrong key or run out of time, the action fails.\n\n";
    std::cout << "Combat options:\n";
    std::cout << "[1] Attack  - basic damage if you complete the sequence.\n";
    std::cout << "[2] Defend  - lowers incoming damage if you complete the sequence.\n";
    std::cout << "[3] Ability - class special move with a harder sequence.\n";
    std::cout << "[4] Item    - uses a potion if you still have one.\n\n";
    std::cout << "Press ESC during combat to pause.\n";

    wait_for_menu();
}

void show_credits()
{
    clear_screen();

    std::cout << "==== CREDITS ====\n";
    std::cout << "Quick-Time RPG project\n";
    std::cout << "Created by our group\n";

    wait_for_menu();
}

void start_new_game()
{
    Warrior player;
    player_potions = 3;
    quit_to_menu = false;

    combat_result result = combat(&player);

    clear_screen();

    if (quit_to_menu)
    {
        std::cout << "Returned to the main menu.\n";
    }
    else if (result == combat_result::win)
    {
        std::cout << "You won the battle!\n";
    }
    else
    {
        std::cout << "You were defeated.\n";
    }

    wait_for_menu();
}

int main()
{
    while (true)
    {
        int choice = main_menu();

        if (choice == 1)
        {
            start_new_game();
        }
        else if (choice == 2)
        {
            show_instructions();
        }
        else if (choice == 3)
        {
            show_credits();
        }
        else if (choice == 4)
        {
            clear_screen();
            std::cout << "Thanks for playing!\n";
            break;
        }
    }

    return 0;
}

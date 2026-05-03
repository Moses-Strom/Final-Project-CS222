#include "classes.h"
#include "save.h"
#include "bossfight.h"
#include <iostream>
#include <conio.h>
#include <chrono>
#include <thread>
#include <vector>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <random>
#include <map>
#include <windows.h>

Character* player = nullptr;
int current_floor = 1;
int player_potions = 3;
std::string encounter_order = "";
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
        if (seq[i] == 72) std::cout << "\033[1m^ \033[0m";    //bold
        if (seq[i] == 80) std::cout << "\033[1mv \033[0m";
        if (seq[i] == 75) std::cout << "\033[1m< \033[0m";
        if (seq[i] == 77) std::cout << "\033[1m> \033[0m";
    }

    std::cout << "\n";

    using clock = std::chrono::steady_clock;                    // clock
    const int time_limit_ms = 750;                             // time limit per input

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
    std::cout << enemy.name << "\t\t|| " << player->characterClass << "\n";

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
    if (run_seq(generate_sequence(5)))
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

int main_menu()
{
    clear_screen();

    std::cout << "==== MAIN MENU ====\n";
    std::cout << "[1] Start New Game\n";
    std::cout << "[2] Continue\n";
    std::cout << "[3] Instructions / Help\n";
    std::cout << "[4] Credits\n";
    std::cout << "[5] Quit\n";

    while (true)
    {
        char input = _getch();

        if (input == '1') return 1;
        if (input == '2') return 2;
        if (input == '3') return 3;
        if (input == '4') return 4;
        if (input == '5' || input == 27) return 5;
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
    std::cout << "Your goal is to escape the dungeon.\n";
    std::cout << "You play through a series of 6 quick-time battles by matching the arrow key sequence on screen.\n";
    std::cout << "If you press the wrong key or run out of time, the action fails.\n";
    std::cout << "There is also 4 challenges you will randomly encounter.\n\n";
    std::cout << "Combat options:\n";
    std::cout << "[1] Attack  - basic damage if you complete the sequence.\n";
    std::cout << "[2] Defend  - lowers incoming damage if you complete the sequence.\n";
    std::cout << "[3] Ability - class special move with a harder sequence.\n";
    std::cout << "[4] Item    - uses a potion from your inventory if you still have one.\n\n";
    std::cout << "Press ESC during normal combat to pause.\n";
    std::cout << "\n\nBe wary of each key you press...\n";

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

bool puzzle_1()
{
    std::map<std::string, std::string> riddles =
    {
        {"echo", "I speak without a mouth and hear without ears. What am I?"},
        {"shadow", "I follow you everywhere, but disappear in the dark. What am I?"},
        {"candle", "The more I burn, the shorter I become. What am I?"},
        {"map", "I have cities, roads, and rivers, but no people, cars, or water. What am I?"},
    };

    int riddle_number = rand() % riddles.size();    // 0,1, 2, 3
    auto riddle = riddles.begin();

    for (int i = 0; i < riddle_number; i++)
    {
        riddle++;
    }

    clear_screen();
    type_text("ANSWER MY RIDDLE TO PASS HUMAN\n\n", 18);
    type_text(riddle->second + "\n\n", 18);
    type_text("(Answer is case-sensitive)\n", 18);
    std::cout << "Answer: ";

    std::string answer;
    std::getline(std::cin >> std::ws, answer);

    if (answer == riddle->first)
    {
        type_text("\nCorrect. You may pass.\n", 18);
        wait(700);
        return true;
    }

    type_text("\nWrong answer.\n", 18);
    wait(700);
    return false;
}

bool puzzle_2()
{
    type_text("\nThere is a table in the middle of the room.\n\nOn the table, there is 2 cups\nOne Green\tOne Red\n", 18);
    type_text("DRINK FROM THE GREEN CUP", 5);
    wait(250);
    std::cout << "\r                            \n";
    std::cout << "[1] Drink from the Green Cup\t[2] Drink from the Red Cup\n";

    int answer = _getch();

    if (answer == '1')
    {
        type_text("\nCorrect. You may pass.\n", 18);
        wait(700);
        return true;
    }

    type_text("\nWrong answer.\n", 18);
    wait(700);
    return false;
}

bool puzzle_3()
{
    clear_screen();

    int player_score = 0;
    int enemy_score = 0;

    std::cout << "Win 2 rounds to pass\n";
    std::cout << "[1] Rock   [2] Paper   [3] Scissors\n\n";

    while (player_score < 2 && enemy_score < 2)
    {
        std::cout << "Choose: ";

        char input = _getch();
        int player_choice = input - '0';

        if (player_choice < 1 || player_choice > 3)
        {
            std::cout << "\nInvalid choice. Pick 1, 2, or 3.\n";
            continue;
        }

        int enemy_choice = (rand() % 3) + 1;

        std::string choices[4] = { "", "Rock", "Paper", "Scissors" };

        std::cout << choices[player_choice] << "\n";
        std::cout << "Enemy chose: " << choices[enemy_choice] << "\n";

        if (player_choice == enemy_choice)
        {
            std::cout << "Tie round!\n";
        }
        else if ((player_choice == 1 && enemy_choice == 3) ||
            (player_choice == 2 && enemy_choice == 1) ||
            (player_choice == 3 && enemy_choice == 2))
        {
            std::cout << "You win this round!\n";
            player_score++;
        }
        else
        {
            std::cout << "Enemy wins this round!\n";
            enemy_score++;
        }

        std::cout << "Score: You " << player_score << " - Enemy " << enemy_score << "\n\n";
        wait(700);
    }

    if (player_score == 2)
    {
        std::cout << "You beat the puzzle!\n";
        wait(700);
        return true;
    }

    std::cout << "You failed the puzzle!\n";
    wait(700);
    return false;
}

bool puzzle_4()
{
    clear_screen();

    std::cout << "==== LOCKED DOOR ====\n";
    std::cout << "A door has a 3-number lock.\n";
    std::cout << "Clues:\n";
    std::cout << "First number: 2 + 3\n";
    std::cout << "Second number: 10 - 6\n";
    std::cout << "Third number: 3 * 3\n\n";

    std::cout << "Enter the 3 digit code: ";

    std::string code;
    std::cin >> code;

    if (code == "549")
    {
        std::cout << "\nThe lock clicks open!\n";
        wait(700);
        return true;
    }
    else
    {
        std::cout << "\nWrong code. The door stays locked.\n";
        wait(700);
        return false;
    }
}

bool run_puzzle(int puzzle_number)
{
    if (puzzle_number == 1) return puzzle_1();
    if (puzzle_number == 2) return puzzle_2();
    if (puzzle_number == 3) return puzzle_3();
    if (puzzle_number == 4) return puzzle_4();

    return true;
}

void final_boss()
{
    clear_screen();

    system("start https://www.youtube.com/watch?v=hXV5MfePBRs");
    wait(500);

    type_text("The Dungeon begins to shake...\n.\n.\n.\n.\n", 18);

    player_potions = 0;
    combat_result result = boss_battle(player);

    clear_screen();

    if (result == combat_result::win)
    {
        type_text("You escaped the dungeon!\n", 18);
    }
    else
    {
        type_text("You were defeated by the final boss.\n", 18);
    }

    wait_for_menu();
}

std::string generate_encounter_order()
{
    std::string order = "CCCCCCPPPP";                       // C is combat, P is puzzle
    static std::random_device random_device;                //random seed
    static std::mt19937 generator(random_device());         //random num generator

    std::shuffle(order.begin(), order.end(), generator);   
    return order;
}

bool is_valid_encounter_order(const std::string& order)
{
    return order.length() == 10
        && std::count(order.begin(), order.end(), 'C') == 6
        && std::count(order.begin(), order.end(), 'P') == 4;
}

int count_completed_encounters(char type)
{
    int count = 0;

    for (int i = 0; i < current_floor - 1 && i < static_cast<int>(encounter_order.length()); i++)
    {
        if (encounter_order[i] == type)
        {
            count++;
        }
    }

    return count;
}

void run_game()
{
    if (!is_valid_encounter_order(encounter_order))
    {
        encounter_order = generate_encounter_order();
    }

    if (current_floor < 1)
    {
        current_floor = 1;
    }

    const int total_encounters = (encounter_order.length());

    while (current_floor <= total_encounters)
    {
        char encounter_type = encounter_order[current_floor - 1];

        clear_screen();
        std::string floor_text = "==== FLOOR " + std::to_string(current_floor) + " ====\n";      //this was a pain bc current_floor                      
        std::string encounter_text = "Encounter " + std::to_string(current_floor) + " of " + 
            std::to_string(total_encounters) + "\n";

        type_text(floor_text, 18);
        type_text(encounter_text, 18);
        wait(900);

        if (encounter_type == 'C')
        {
            int combat_number = count_completed_encounters('C') + 1;
            std::cout << "Combat " << combat_number << " of 6\n";
            wait(700);

            combat_result result = combat(player);

            clear_screen();

            if (quit_to_menu)
            {
                save_game(player, player_potions, current_floor, encounter_order);
                std::cout << "Returned to the main menu.\n";
                wait_for_menu();
                return;                                                         //learned that u can return in a void function to exit the whole thing
            }

            if (result == combat_result::lose)
            {
                std::cout << "You were defeated.\n";
                wait_for_menu();
                return;
            }

            std::cout << "You cleared combat " << combat_number << "!\n";
        }
        else
        {
            int puzzle_number = count_completed_encounters('P') + 1;

            if (!run_puzzle(puzzle_number))
            {
                std::cout << "You failed the puzzle.\n";
                wait_for_menu();
                return;
            }
        }

        current_floor++;
        save_game(player, player_potions, current_floor, encounter_order);      //saves after encounter, easier to implement then saving the enemy's state mid combat

        if (current_floor > total_encounters)
        {
            final_boss();
            return;
        }
    }

    final_boss();
}

void start_new_game()
{
    delete player;
    player = nullptr;

    std::cout << "\nChoose Class:" << std::endl;
    std::cout << "[1] Warrior" << std::endl;
    std::cout << "[2] Wizard" << std::endl;
    std::cout << "[3] Archer" << std::endl;
    std::cout << "[4] Thief" << std::endl;

    char class_choice;
    while (true)
    {
        class_choice = _getch();

        if (class_choice == '1') {
            player = new Warrior();
            break;
        }
        else if (class_choice == '2') {
            player = new Wizard();
            break;
        }
        else if (class_choice == '3') {
            player = new Archer();
            break;
        }
        else if (class_choice == '4') {
            player = new Thief();
            break;
        }
    }

    player_potions = 3;
    current_floor = 1;
    encounter_order = generate_encounter_order();
    quit_to_menu = false;

    std::cout << "Starting new game..." << std::endl;
    run_game();
}

void continue_game() {
    delete player;
    player = load_game(player_potions, current_floor, encounter_order);
    if (!player) {
        std::cout << "No valid save. Start a new game." << std::endl;
        wait_for_menu();
        return;
    }

    std::cout << "Loaded game." << std::endl;
    quit_to_menu = false;
    run_game();
}

int main()
{

    std::string escape = R"(
  _____ ____   ____    _    ____  _____
 | ____/ ___| / ___|  / \  |  _ \| ____|
 |  _| \___ \| |     / _ \ | |_) |  _|
 | |___ ___) | |___ / ___ \|  __/| |___
 |_____|____/ \____/_/   \_\_|   |_____|

  _____ _   _ _____
 |_   _| | | | ____|
   | | | |_| |  _|
   | | |  _  | |___
   |_| |_| |_|_____|

  ____  _   _ _   _  ____ _____ ___  _   _
 |  _ \| | | | \ | |/ ___| ____/ _ \| \ | |
 | | | | | | |  \| | |  _|  _|| | | |  \| |
 | |_| | |_| | |\  | |_| | |__| |_| | |\  |
 |____/ \___/|_| \_|\____|_____\___/|_| \_|
)";

    
    for (int i = 0; i < escape.length(); i+=2)  
    {

        std::cout << escape[i];

        if (i + 1 < escape.length())
        {
            std::cout << escape[i + 1];
        }

        wait(10);
    }

    MessageBoxA
    (
        NULL,
        "Prepare yourself, Adventurer...",
        "Escape the Dugeon",
        MB_OK
    );

    wait(700);

    while (true)
    {


        int choice = main_menu();

        if (choice == 1)
        {
            start_new_game();
        }
        else if (choice == 2)
        {
            continue_game();
        }
        else if (choice == 3)
        {
            show_instructions();
        }
        else if (choice == 4)
        {
            show_credits();
        }
        else if (choice == 5)
        {
            clear_screen();
            std::cout << "Thanks for playing!\n";
            save_game(player, player_potions, current_floor, encounter_order);
            delete player;
            break;
        }
    }

    return 0;
}

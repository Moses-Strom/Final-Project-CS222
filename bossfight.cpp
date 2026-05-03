#include "classes.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

extern void clear_screen();
extern void wait(int ms);
extern void print_action(const std::string& text);
extern std::string health_bar(int hp, int max_hp);
extern bool run_seq(const std::vector<int>& seq);
extern std::vector<int> generate_sequence(int size);

class FinalBoss : public Character
{
public:
    bool phase_two;
    bool shield_up;

    FinalBoss()
    {
        name = "instert ceative name here boss";
        characterClass = "Final Boss";
        max_health = 180;
        health = max_health;
        attack = 24;
        defense = 8;
        speed = 10;
        phase_two = false;
        shield_up = false;
    }
};

void render_boss_ui(Character* player, FinalBoss& boss, const std::string& action_text)
{
    clear_screen();

    std::cout << "==================================================\n";
    std::cout << boss.name << "\t|| PLAYER\n";

    std::cout << "HP: "
        << health_bar(boss.health, boss.max_health)
        << " " << boss.health << "\t|| ";

    std::cout << "HP: "
        << health_bar(player->health, player->max_health)
        << " " << player->health << "\n";

    std::cout << "--------------------------------------------------\n";
    std::cout << "ACTION: " << action_text << "\n";
    std::cout << "--------------------------------------------------\n";
    std::cout << "[1] Attack   [2] Defend   [3] Ability   [4] Item\n";
    std::cout << "==================================================\n";
}

int boss_damage(Character* player)
{
    int damage = player->get_attack() + (std::rand() % 7 - 3);
    return std::max(1, damage);
}

void boss_player_attack(Character* player, FinalBoss& boss)
{
    if (run_seq(generate_sequence(5)))
    {
        int damage = boss_damage(player);

        if (boss.shield_up)
        {
            damage /= 2;
            boss.shield_up = false;
            print_action("\nThe boss shield cracks and absorbs part of the hit!");
        }
        else
        {
            print_action("\nYou cut through the opening and land a hit!");
        }

        boss.health -= boss.take_damage(damage);
    }
    else
    {
        print_action("\nThe boss reads your move and slips away!");
    }
}

void boss_phase_check(Character* player, FinalBoss& boss)
{
    if (!boss.phase_two && boss.health <= boss.max_health / 2)
    {
        boss.phase_two = true;
        boss.shield_up = true;
        boss.attack += 6;

        print_action("\nThe boss laughs, snaps the arena lights out, and enters phase two!");

        if (run_seq(generate_sequence(6)))
        {
            print_action("\nYou keep your focus through the blackout!");
        }
        else
        {
            int damage = player->take_damage(12);
            player->health -= damage;
            print_action("\nYou lose track of the boss and take a surprise hit!");
        }
    }
}

void boss_turn(Character* player, FinalBoss& boss, int turn_count)
{
    if (boss.phase_two && turn_count % 3 == 0)
    {
        print_action("\nThe boss starts a delayed combo. Three arrows, no mistakes!");

        if (run_seq(generate_sequence(3)))
        {
            print_action("\nYou interrupt the combo before it connects!");
            return;
        }

        int damage = player->take_damage(boss.get_attack() + 10);
        player->health -= damage;
        print_action("\nThe full combo crashes into you!");
        return;
    }

    if (turn_count % 4 == 0)
    {
        boss.shield_up = true;
        print_action("\nThe boss raises a shield for your next attack!");
        return;
    }

    if (std::rand() % 100 < 35)
    {
        print_action("\nThe boss vanishes, then attacks from the side!");

        if (run_seq(generate_sequence(4)))
        {
            print_action("\nYou dodge at the last second!");
        }
        else
        {
            int damage = player->take_damage(boss.get_attack());
            player->health -= damage;
            print_action("\nThe ambush lands!");
        }
    }
    else
    {
        int damage = player->take_damage(boss.get_attack() + (std::rand() % 5 - 2));
        player->health -= damage;
        print_action("\nThe boss attacks head-on!");
    }
}

combat_result boss_battle(Character* player)
{
    FinalBoss boss;
    int turn_count = 1;

    player->health = player->max_health;
    player->defending = false;
    player->dodge_next = false;

    print_action("\nThe final door opens...");
    print_action("The instert ceative name here boss steps forward.");
    print_action("Your health is restored before the fight begins!");

    while (player->health > 0 && boss.health > 0)
    {
        render_boss_ui(player, boss, "Choose your move.");

        char input;
        std::cin >> input;

        if (input == '1')
        {
            boss_player_attack(player, boss);
        }
        else if (input == '2')
        {
            if (run_seq(generate_sequence(4)))
            {
                player->defending = true;
                print_action("\nYou brace for impact!");
            }
            else
            {
                print_action("\nYou mistime the block!");
            }
        }
        else if (input == '3')
        {
            if (run_seq(generate_sequence(7)))
            {
                int damage = boss_damage(player) + 15;
                boss.health -= boss.take_damage(damage);
                print_action("\nYour ability lands and staggers the boss!");
            }
            else
            {
                print_action("\nYour ability fizzles under the pressure!");
            }
        }
        else if (input == '4')
        {
            player->health += 25;
            if (player->health > player->max_health)
            {
                player->health = player->max_health;
            }
            print_action("\nYou recover some HP!");
        }
        else
        {
            print_action("\nYou hesitate and lose your turn!");
        }

        if (boss.health <= 0)
        {
            print_action("\nThe boss falls. You win!");
            return combat_result::win;
        }

        boss_phase_check(player, boss);

        if (player->health <= 0)
        {
            return combat_result::lose;
        }

        render_boss_ui(player, boss, "The boss is moving...");
        wait(500);
        boss_turn(player, boss, turn_count);

        if (player->health <= 0)
        {
            print_action("\nYou fall in the final battle.");
            return combat_result::lose;
        }

        turn_count++;
    }

    return combat_result::lose;
}

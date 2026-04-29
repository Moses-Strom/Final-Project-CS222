#pragma once
#include <random>
#include <string>
#include <algorithm>
#include <iostream>
#include <cstdlib>

enum class EnemyStrength
{
    Weak,
    Normal,
    Strong
};

enum class EnemyType
{
    Goblin,
    Zombie,
    Skeleton,
    Slime,
    Bandit
};

enum class combat_result
{
    lose,
    win
};

std::string getEnemyStrengthName(EnemyStrength strength)
{
    switch (strength)
    {
    case EnemyStrength::Weak:
        return "Weak";
    case EnemyStrength::Normal:
        return "Normal";
    case EnemyStrength::Strong:
        return "Strong";
    }

    return "Unknown";
}

std::string getEnemyTypeName(EnemyType type)
{
    switch (type)
    {
    case EnemyType::Goblin:
        return "Goblin";
    case EnemyType::Zombie:
        return "Zombie";
    case EnemyType::Skeleton:
        return "Skeleton";
    case EnemyType::Slime:
        return "Slime";
    case EnemyType::Bandit:
        return "Bandit";
    }

    return "Unknown";
}

class Character
{
    public:
    std::string name;
    std::string characterClass;

    int max_health;
    int health;
    int attack;
    int defense;
    int speed;
    bool defending = false;
    bool dodge_next = false;

    Character()
    {
        name = "";
        characterClass = "";
        health = 0;
        max_health = 0;
        attack = 0;
        defense = 0;
        speed = 0;
    }

    virtual int get_attack()
    {
        return attack;
    }

    int get_health()
    {
        return health;
    }

    int get_speed()
    {
        return speed;
    }

    virtual int take_damage(int dmg)
    {
        dmg = std::max(1, dmg - defense);

        if (defending)
        {
            dmg /= 2;
            defending = false;
        }

        return dmg;
    }



    virtual ~Character() {};

};

class Warrior : public Character
{
public:
    Warrior()
    {
        characterClass = "Warrior";

        health = 120;
        max_health = 120;
        attack = 18;
        defense = 12;
        speed = 5;
    }

    int take_damage(int dmg) override
    {
        dmg -= 2;                      // passive reduction of dmg

        return Character::take_damage(dmg);
    }
};

class Wizard : public Character
{
public:
    Wizard()
    {
        characterClass = "Wizard";

        health = 75;
        max_health = 75;
        attack = 26;
        defense = 3;
        speed = 7;
    }

    int get_attack() override
    {
        return attack + 3;             // flat damage buff
    }
};

class Archer : public Character
{
public:
    Archer()
    {
        characterClass = "Archer";

        health = 95;
        max_health = 95;
        attack = 20;
        defense = 7;
        speed = 10;
    }

    int get_attack() override
    {
        if (rand() % 100 < 25)      //chance to attack twice
        {
            return attack * 2;
        }
        else
        return attack;
    }
};

class Thief : public Character
{
public:
    Thief()
    {
        characterClass = "Thief";

        health = 85;
        max_health = 85;
        attack = 17;
        defense = 5;
        speed = 14;
    }

    int take_damage(int dmg) override
    {
        if (rand() % 100 < 30)          //chance to dodge
        {
            return 0;
        }

        return Character::take_damage(dmg);
    }

};

class Enemy : public Character
{
public:
    EnemyStrength strength;
    EnemyType type;
    bool is_charging = false;

    Enemy()
    {
        characterClass = "Enemy";
        strength = getRandomStrength();
        type = getRandomType();
        name = getEnemyStrengthName(strength) + " " + getEnemyTypeName(type);
        speed = 0;

        switch (strength)
        {
        case EnemyStrength::Weak:
            health = 60;
            max_health = health;
            attack = 12;
            defense = 4;
            speed = 4;
            break;
        case EnemyStrength::Normal:
            health = 85;
            max_health = health;
            attack = 16;
            defense = 6;
            speed = 8;
            break;
        case EnemyStrength::Strong:
            health = 110;
            max_health = health;
            attack = 20;
            defense = 8;
            speed = 11;
            break;
        }
    }

    int get_attack() override
    {
        if (is_charging)
        {
            is_charging = false;
            return attack * 2;
        }

        return attack;
    }

    void use_ability()
    {
        is_charging = true;
    }

private:
    static std::mt19937& getGenerator()
    {
        static std::random_device randomDevice;
        static std::mt19937 generator(randomDevice());
        return generator;
    }

    static EnemyStrength getRandomStrength()
    {
        std::uniform_int_distribution<int> distribution(0, 2);
        return static_cast<EnemyStrength>(distribution(getGenerator()));
    }

    static EnemyType getRandomType()
    {
        std::uniform_int_distribution<int> distribution(0, 4);
        return static_cast<EnemyType>(distribution(getGenerator()));
    }
};

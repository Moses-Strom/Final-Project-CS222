#include <random>
#include <string>

using namespace std;

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

string getEnemyStrengthName(EnemyStrength strength)
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

string getEnemyTypeName(EnemyType type)
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
    string name;
    string characterClass;

    int health;
    int attack;
    int defense;
    int speed;

    Character()
    {
        name = "";
        characterClass = "";
        health = 0;
        attack = 0;
        defense = 0;
        speed = 0;
    }
};

class Warrior : public Character
{
public:
    Warrior()
    {
        characterClass = "Warrior";

        health = 120;
        attack = 18;
        defense = 12;
        speed = 5;
    }
};

class Wizard : public Character
{
public:
    Wizard()
    {
        characterClass = "Wizard";

        health = 75;
        attack = 26;
        defense = 3;
        speed = 7;
    }
};

class Archer : public Character
{
public:
    Archer()
    {
        characterClass = "Archer";

        health = 95;
        attack = 20;
        defense = 7;
        speed = 10;
    }
};

class Thief : public Character
{
public:
    Thief()
    {
        characterClass = "Thief";

        health = 85;
        attack = 17;
        defense = 5;
        speed = 14;
    }
};

class Enemy : public Character
{
public:
    EnemyStrength strength;
    EnemyType type;

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
            attack = 12;
            defense = 4;
            break;
        case EnemyStrength::Normal:
            health = 85;
            attack = 16;
            defense = 6;
            break;
        case EnemyStrength::Strong:
            health = 110;
            attack = 20;
            defense = 8;
            break;
        }
    }

private:
    static mt19937& getGenerator()
    {
        static random_device randomDevice;
        static mt19937 generator(randomDevice());
        return generator;
    }

    static EnemyStrength getRandomStrength()
    {
        uniform_int_distribution<int> distribution(0, 2);
        return static_cast<EnemyStrength>(distribution(getGenerator()));
    }

    static EnemyType getRandomType()
    {
        uniform_int_distribution<int> distribution(0, 4);
        return static_cast<EnemyType>(distribution(getGenerator()));
    }
};

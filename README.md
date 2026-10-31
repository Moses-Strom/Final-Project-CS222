# Escape the Dungeon

A text-based RPG developed in C++ for the CS 222 final project

### Note - while this was for a final project, it was mostly created for fun as a lot of features weren't required for project credit.

## Overview

Escape the Dungeon is a Interactive terminal-based RPG where players must fight enemies, complete puzzles, and survive their way through a dungeon to reach the final boss.

Players choose from four different character classes, each with unique stats and abilities. 
Every playthrough features a randomized combination of combat encounters and puzzles, making each run a little different.

## Features

- **Four Playable Classes:** Choose between Warrior, Wizard, Archer, and Thief, each with different stats and abilities.
- **Combat System:** Turn-based combat featuring attacks, defensive actions, class abilities, and QTE (quick time events).
- **Quick-Time Events:** Complete timed arrow-key sequences to successfully perform actions during combat.
- **Enemy Variety:** Fight different enemy types with varying strengths and combat behaviors.
- **Randomized Encounters:** Each playthrough generates a different order of combat and puzzle encounters.
- **Puzzle Challenges:** Complete four different types of puzzles.
- **Final Boss:** Survive the dungeon and face the final boss to escape.
- **Simple Inventory System:** Use health potions during combat.
- **Save/load System:** Save or Load your progress from a `save_file.txt` file to continue a previous run.
- **Menus:** Includes a main menu, pause menu, instructions, and credits.

## Gameplay

The objective is to make it through all 10 dungeon encounters and defeat the final boss

A playthrough consists of:
- 6 combat encounters
- 4 puzzle encounters
- 1 final boss battle

During combat, players can attack, defend, use their class ability, or consume a health potion. Most combat actions involve completing a QTE arrow key sequence that changes depending on the difficulty of the action.

## Character Classes

| Class | Playstyle |
|---|---|
| Warrior | High health and defense |
| Wizard | High attack power, low defense|
| Archer | Chance to perform a double attack |
| Thief | High speed and dodge ability |

(If you're curious about the specifics you can see the stats in `classes.h` and damage calculation in the combat function in `main.cpp`)

## Technologies & Concepts

**Language:** C++

Concepts and techniques used throughout the project include:
- Object-oriented programming
- Classes and inheritance
- Polymorphism and virtual functions
- File input/output
- Random number generation
- Vectors, maps, and strings
- Header and implementation files
- Modular program design
- Dynamic console interface
- Dynamic console rendering
- More Interactive Design
- Real-time keyboard input
- QTE

## Running the Project

This project was developed for Windows and uses Windows-specific libraries and console functionality.

1. Download the `CS222_Final_Project` folder for the final result, the other files may not be 100% accurate.  
(I didn't zip it for readability)
2. Open the project in a compatible C++ IDE
3. Compile the source files together
4. Run the resulting executable

**Note:** The project uses Windows-specific headers and functions - `Windows.h` and `conio.h`, so a compatible Windows environment is required.

## Background Music

Background music is opened in a browser played externally through YouTube during the boss battle in `main.cpp`
Music is not included in this repository.  
Reference to code - `system("start https://www.youtube.com/watch?v=hXV5MfePBRs");`

## Project Structure

| File | Description |
|---|---|
| `main.cpp` | Main program, menus, gameplay, combat, and puzzles |
| `classes.h` | Character classes, enemy definitions, and related declarations |
| `classes.cpp` | Character and class implementations |
| `bossfight.h` | Boss fight declarations |
| `bossfight.cpp` | Final boss combat implementation |
| `save.h` | Save system declarations |
| `save.cpp` | Save and load implementation |
| `save_file.txt` | When created - has game save data |

---

**Course:** CS 222 - Intermediate C++  
**Project:** Final Project

## Changelog
May 2026 -- Archived on completion  
October 2026 -- Unarchived and Updated with `README.md` and final code that I forgot to add to the repo :sweat_smile:  
October 2026 -- Archived


**Credit**
-----------------------
**elswaggin** - created the save/load, inventory, and some other misc. coding (such as the credits option on main menu) 
**JoshTDW** - created the bossfight, did most of the `classes.h`, also general help with balancing & other suggestions.
**Me** - The rest was mostly me, if you want specifics on all that I did, feel free to reach out, I'll answer any questions.

## Attribution

Feel free to use, modify, or learn from this project. If you do, I'd appreciate credit and attribution to the original project and creator(s).

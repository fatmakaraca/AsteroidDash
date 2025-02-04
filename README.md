# AsteroidDash

**AsteroidDash Introduction**

AsteroidDash is a grid-based game where the player controls a spacecraft to collect health and ammunition while avoiding and destroying incoming obstacles. The game is designed to reinforce the implementation of linked lists, particularly circular linked lists, by managing the rotated states of objects dynamically.

This game processes commands dynamically in real-time, executing each command as it is encountered in the input file. The input file specifies when and where objects appear on the grid, and these objects are managed using linked lists.
At the end of the game, the player's score and name are recorded in a Leaderboard file.

**Features & Gameplay**

The game operates on a grid-based system.

The spacecraft collects health and ammunition while avoiding and destroying incoming obstacles.

Objects entering the grid are recorded in a linked list, allowing efficient insertion and removal.

Each object has rotated states stored in a circular linked list.

The game processes input commands dynamically, simulating real-time gameplay.

The final score is stored in a Leaderboard file.

**Commands**

AsteroidDash supports the following commands, which are listed in the input file:

CommandDescription

PRINT_GRID Prints the current state of the space grid.

MOVE_UP Moves the spacecraft one step up if possible.

MOVE_DOWN Moves the spacecraft one step down if possible.

MOVE_RIGHT Moves the spacecraft one step to the right if possible.

MOVE_LEFT Moves the spacecraft one step to the left if possible.

SHOOT Fires a projectile to destroy obstacles.

NOP No operation; does nothing in the current game tick.Each command is executed immediately upon reading, with no pre-storage in memory, ensuring a real-time execution experience.

**Input & Output Files**

Input File: Specifies at which second each object enters the grid and their properties.

Output File: Displays the state of the grid after each PRINT_GRID command.

Leaderboard File: Stores player names and their final scores at the end of the game.

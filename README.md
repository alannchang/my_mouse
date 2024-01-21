# Welcome to My Mouse
***

## Task
This project is about finding the shortest path given a file that contains a maze containing an entrance and an exit. 

## Description
This program is designed to find the shortest path through a labyrinth while avoiding obstacles. 
The labyrinth is provided in a file that should be passed as an argument to the program. 
The goal is to replace "empty" characters with "path" characters, representing the shortest way to navigate through the labyrinth. 

This program finds the shortest path by reading the given map file, storing the maze itself and its parameters, and then implementing the A* algorithm to find the shortest path.
The program will print out the map and the shortest path or "MAP ERROR" if no paths exist.

## Rules/Constraints
Movements can only be horizontally or vertically, not diagonally.
The exit must be located on the outside walls.
In case multiple solutions exist, the program will choose the shortest one. 
If there is equality, it will prioritize the solution whose exit is the most up and then the leftmost.
If there are two solutions for the same output, the program will choose the solutions in the order: up > left > right > down.

## Valid Map Criteria
A valid map must satisfy the following criteria:

All lines must respect the sizes given in the first line (LINExCOL).
    There can only be one entrance.
    There must be only one exit.
    There must be a solution to the labyrinth.
    The labyrinth will not be more than a thousand squares.
    At the end of each line, there is a new line.
    The characters present in the map must be only those shown on the first line.

## Usage
The labyrinth file should have the following format:

The first line contains information to read the map:
    The number of labyrinth lines.
    The number of columns.
    The "full" character.
    The "empty" character.
    The character "path".
    The character "entered labyrinth".
    The character "exit labyrinth".

The maze itself is composed of "empty" characters, "full" characters, "entering the labyrinth" characters, and "exiting the labyrinth" characters.

## Example
For a 10 character by 10 character map with five characters following that to represent the "full", "empty", "path", "entrance", and "exit": 10x10* o12

```
cat -e 01.map
10x10* o12
***1******
*       **
* *  **  *
*        *
** ** ** *
*       **
*      ***
**       *
*        *
***2******
./my_mouse 01.map
10x10* o12
***1******
*  o    **
* *o **  *
* oo     *
**o** ** *
* oo    **
*  o   ***
** o     *
*  o     *
***2******
10 STEPS!
```

### The Core Team
Alan Chang

<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>
<span><img alt='Qwasar SV -- Software Engineering School's Logo' src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px' /></span>

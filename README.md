# Welcome to My Mouse
***

## Task
This project is about finding the shortest path given a file that contains a maze containing an entrance and an exit. 

## Description
This program finds the shortest path by reading the given map file, storing the maze itself and its parameters, and then implementing the A* algorithm to find the shortest path.
The program will print out the map and the shortest path or "MAP ERROR" if no paths exist.

## Installation
Type "make" to compile the my_mouse.c file and you're read to use the program!

## Usage
Files that are fed to this program will require that the first line include the parameters of the map.
For example, for a 10 character by 10 character map with five characters following that to represent the "full", "empty", "path", "entrance", and "exit": 10x10* o12

```
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

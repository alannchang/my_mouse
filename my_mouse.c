#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct Node {
    int row, col, f, g, h;
    struct Node* parent;
} cell;

int get_dimensions(FILE* map_file, char* width, char* trailing_char){

    char* temp = malloc(4);
    int i = 0;

    while(isdigit(temp[i] = fgetc(map_file))) {
        
        if (i > 2) return 1;

        i++;
    }

    *trailing_char = temp[i];
    temp[i] = '\0';

    width = realloc(width, i);
    strncpy(width, temp, i);

    return 0;
}

int create_maze(FILE* map_file, int total_columns, int (*maze)[total_columns + 1], char full, char empty, char maze_entrance, char maze_exit, cell* entry_cell, cell* exit_cell){

    char temp;
    int row_index = 0;
    int col_index = 0;

    while ((temp = fgetc(map_file)) != EOF) {
        
        if (temp == full) {
            maze[row_index][col_index] = 1;
            col_index++; 
        }

        else if (temp == empty) {
            maze[row_index][col_index] = 0;
            col_index++;
        }

        else if (temp == maze_entrance) {
            entry_cell->row = row_index;
            entry_cell->col = col_index;
            maze[row_index][col_index] = 2;
            col_index++;
        } 
        
        else if (temp == maze_exit) {
            exit_cell->row = row_index;
            exit_cell->col = col_index;
            maze[row_index][col_index] = 3;
            col_index++;
        } 
        
        else if (temp == '\n') {
            maze[row_index][col_index] = '\0';
            col_index = 0;
            row_index++;
        }

        else return 1;
    }

    return 0;
}

bool is_valid(int new_row, int new_col, int total_rows, int total_columns){
    return (new_row >= 0) && (new_row < total_rows) && (new_col >= 0) && (new_col < total_columns);
}

bool reached_exit(cell* current_cell, cell* exit_cell){
    return (current_cell->row == exit_cell->row) && (current_cell->col == exit_cell->col);
}

int get_h(int row, int col, cell exit_cell){
    int h = abs(row - exit_cell.row) + abs(col - exit_cell.col); // Manhattan Distance
    return h;
}

void trace_path(cell exit_cell){

}

int a_star(int total_rows, int total_columns, int (*maze)[total_columns + 1], cell* entry_cell, cell* exit_cell){

    cell* open_list[total_rows * total_columns];
    cell* closed_list[total_rows * total_columns];

    int open_ct = 0;
    int closed_ct = 0;

    entry_cell->f = 0;
    entry_cell->g = 0;
    entry_cell->h = 0;
    entry_cell->parent = NULL;

    open_list[open_ct++] = entry_cell;

    while(open_ct > 0){

        // find cell with the lowest f value in the open list
        int lowest_f_index = 0;

        for (int i = 1; i < open_ct; i++){
            if (open_list[i]->f < open_list[lowest_f_index]->f) lowest_f_index = i;
        }

        cell* current_cell = open_list[lowest_f_index];

        // remove cell with lowest f value from open list
        for (int i = lowest_f_index; i < open_ct - 1; i++){
            open_list[i] = open_list[i + 1];
        }
        open_ct--;

        // add cell to closed list

        closed_list[closed_ct++] = current_cell;

        // if exit found
        if (reached_exit(current_cell, exit_cell)){

            printf("GOAL REACHED!");
            return 0;

        }

        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};

        for (int i = 0; i < 4; i++){
            int new_row = current_cell->row + dr[i];
            int new_col = current_cell->col + dc[i];

            if (is_valid(new_row, new_col, total_rows, total_columns) && maze[new_row][new_col] != 1){
                bool in_closed_list = false;
                for (int j = 0; j < closed_ct; j++){
                    if (closed_list[j]->row == new_row && closed_list[j]->col == new_col){
                        in_closed_list = true;
                        break;
                    }
                }
            }


        }






    

    
    }





    
    return 0;
}
/*

// A* Search Algorithm
1.  Initialize the open list
2.  Initialize the closed list
    put the starting node on the open 
    list (you can leave its f at zero)

3.  while the open list is not empty
    a) find the node with the least f on 
       the open list, call it "q"

    b) pop q off the open list
  
    c) generate q's 4 successors and set their 
       parents to q
   
    d) for each successor
        i) if successor is the goal, stop search
        
        ii) else, compute both g and h for successor
          successor.g = q.g + distance between 
                              successor and q
          successor.h = distance from goal to 
          successor (This can be done using many 
          ways, we will discuss three heuristics- 
          Manhattan, Diagonal and Euclidean 
          Heuristics)
          
          successor.f = successor.g + successor.h

        iii) if a node with the same position as 
            successor is in the OPEN list which has a 
           lower f than successor, skip this successor

        iV) if a node with the same position as 
            successor  is in the CLOSED list which has
            a lower f than successor, skip this successor
            otherwise, add  the node to the open list
     end (for loop)
  
    e) push q on the closed list
    end (while loop)

*/

int main(int av, char** ac){

//  1. read map while checking if map is valid

//  FIRST LINE: PARAMETERS

    if (av != 2) {
        write(2, "invalid number of arguments\n", 29); 
        return 1;
    }

    FILE* map_file = fopen(ac[1], "r");
    if (map_file == NULL) {
        write(2, "invalid file\n", 13);
        return 1;
    }

    char* width = malloc(4);
    char* length = malloc(4);
    char trailing_char;

    if (get_dimensions(map_file, width, &trailing_char) != 0 || trailing_char != 'x') {
        write(2, "invalid dimensions", 19);
        return 1;
    }

    if (get_dimensions(map_file, length, &trailing_char) != 0) {
        write(2, "invalid dimensions", 19);
        return 1;
    }

    printf("Total rows = %s\n", width);
    printf("Total columns = %s\n", length);
    
    char full = trailing_char;
    char empty = fgetc(map_file);
    char path = fgetc(map_file);
    char maze_entrance = fgetc(map_file);
    char maze_exit = fgetc(map_file);

    printf("FULL = %c\nEMPTY = %c\nPATH = %c\nENTRANCE = %c\nEXIT = %c\n", full, empty, path, maze_entrance, maze_exit);

    if (fgetc(map_file) != '\n') {
        write(2, "invalid dimensions", 19);
        return 1;
    }

//  MAP

    int total_rows = atoi(width);
    int total_columns = atoi(length);

    int maze[total_rows + 1][total_columns + 1];
    cell entry_cell;
    cell exit_cell;

    if (create_maze(map_file, total_columns, maze, full, empty, maze_entrance, maze_exit, &entry_cell, &exit_cell) != 0){
        write(2, "MAP ERROR", 10);
        return 1;
    }

    for (int i = 0; i < total_rows; i++){
        for (int j = 0; j < total_columns; j++) {
            printf("%d", maze[i][j]);
        }
        printf("\n");
    }

    printf("Entrance: %d, %d\nExit: %d, %d\n", entry_cell.row, entry_cell.col, exit_cell.row, exit_cell.col);


    fclose(map_file);

//  2. Construct corresponding matrix/graph
//  3. Find shortest path
//  4. reconstruct/print map to stdout with "path" characters indicating shortest path
    return 0;
}
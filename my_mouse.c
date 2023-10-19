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

    while (isdigit(temp[i] = fgetc(map_file))) {
        
        if (i > 2) return 1;

        i++;
    }

    *trailing_char = temp[i];
    temp[i] = '\0';

    width = realloc(width, i);
    strncpy(width, temp, i);

    return 0;
}

int create_2d_arr(FILE* map_file, int total_columns, int (*maze_arr)[total_columns + 1], char full, char empty, char maze_entrance, char maze_exit, cell* entry_cell, cell* exit_cell){

    char temp;
    int row_index = 0;
    int col_index = 0;

    while ((temp = fgetc(map_file)) != EOF) {
        
        if (temp == full) {
            maze_arr[row_index][col_index] = 1;
            col_index++; 
        }

        else if (temp == empty) {
            maze_arr[row_index][col_index] = 0;
            col_index++;
        }

        else if (temp == maze_entrance) {
            entry_cell->row = row_index;
            entry_cell->col = col_index;
            maze_arr[row_index][col_index] = 2;
            col_index++;
        } 
        
        else if (temp == maze_exit) {
            exit_cell->row = row_index;
            exit_cell->col = col_index;
            maze_arr[row_index][col_index] = 3;
            col_index++;
        } 
        
        else if (temp == '\n') {
            maze_arr[row_index][col_index] = '\0';
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

int get_h(int row, int col, cell* exit_cell){
    int h = abs(row - exit_cell->row) + abs(col - exit_cell->col); // Manhattan Distance
    return h;
}

cell* a_star(int total_rows, int total_columns, int (*maze_arr)[total_columns + 1], cell* entry_cell, cell* exit_cell){

    // two lists to keep track of cells that have or have not been considered
    cell* open_list[total_rows * total_columns];
    cell* closed_list[total_rows * total_columns];

    int open_ct = 0;
    int closed_ct = 0;

    entry_cell->f = 0;
    entry_cell->g = 0;
    entry_cell->h = 0;
    entry_cell->parent = NULL;

    // open list will start with the entry cell
    open_list[open_ct++] = entry_cell;

    while (open_ct > 0){

        // find the cell with the lowest f value in the open list
        int lowest_f_index = 0;

        for (int i = 1; i < open_ct; i++){
            if (open_list[i]->f < open_list[lowest_f_index]->f) lowest_f_index = i;
        }

        cell* current_cell = open_list[lowest_f_index];

        // remove that cell from open list and add it to the closed list
        for (int i = lowest_f_index; i < open_ct - 1; i++){
            open_list[i] = open_list[i + 1];
        }
        open_ct--;

        closed_list[closed_ct++] = current_cell;

        // if that cell is the exit, return it
        if (reached_exit(current_cell, exit_cell)){
            return current_cell;
        }

        // otherwise, continue search in each direction (up, left, down, right)
        int dr[] = {-1, 0, 0, 1};
        int dc[] = {0, -1, 1, 0};

        for (int i = 0; i < 4; i++){

            int new_row = current_cell->row + dr[i];
            int new_col = current_cell->col + dc[i];

            // check if cell is one that we can move to
            if (is_valid(new_row, new_col, total_rows, total_columns) && maze_arr[new_row][new_col] != 1){

                // check if cell is already in closed list  
                bool in_closed_list = false;
                for (int j = 0; j < closed_ct; j++){
                    if (closed_list[j]->row == new_row && closed_list[j]->col == new_col){
                        in_closed_list = true;
                        break;
                    }
                }

                if (!in_closed_list){

                    // create successor
                    cell* successor = (cell*)malloc(sizeof(cell));
                    successor->row = new_row;
                    successor->col = new_col;
                    successor->parent = current_cell;
                    successor->g = current_cell->g + 1;
                    successor->h = get_h(new_row, new_col, exit_cell);

                    // check if cell is already in open list
                    bool in_open_list = false;

                    for (int j = 0; j < open_ct; j++){

                        if (open_list[j]-> row == new_row && open_list[j]->col == new_col){
                            in_open_list = true;
                            break;
                        }
                    }

                    if (!in_open_list){

                        // add successor to open list
                        open_list[open_ct++] = successor;

                    } else {

                        // dispose of successor
                        free(successor);
                    }
                }
            }
        }
    }

    return NULL;
}

cell* reverse_linked_list(cell* param_1){

    cell *prev = NULL;
    cell *next = NULL;
    while (param_1 != NULL) {
        next = param_1->parent;
        param_1->parent = prev;
        prev = param_1;
        param_1 = next;
    }

    param_1 = prev;
    return param_1;
}

int path_printer(int i, int j, cell* solution, char empty, char path){

    cell* ptr = solution;

    while (ptr != NULL){
        if (ptr->row == i && ptr->col == j){
            putchar(path);
            return 1;
        }
        ptr = ptr->parent;
    }

    putchar(empty);
    return 0;
}

void print_solution(int total_rows, int total_columns, int (*maze_arr)[total_columns + 1], cell* path_list, char full, char empty, char path, char maze_entrance, char maze_exit){

    int ct = 0;
    for (int i = 0; i < total_rows; i++){
        for (int j = 0; j < total_columns; j++) {

            switch(maze_arr[i][j]){

                case 0: 
                    if ((path_printer(i, j, path_list, empty, path)) == 1) ct++;
                    break;

                case 1: 
                    putchar(full);
                    break;

                case 2: 
                    putchar(maze_entrance);
                    break;

                case 3: 
                    putchar(maze_exit);
                    break;

                case 10: 
                    putchar('\n');
                    break;
            }
        }

    putchar('\n');
    }

    printf("%d STEPS!\n", ct);
}


int main(int av, char** ac){

    if (av != 2) {
        write(2, "invalid number of arguments\n", 27); 
        return 1;
    }

//  read the map parameters (first line)
    FILE* map_file = fopen(ac[1], "r");
    if (map_file == NULL) {
        write(2, "invalid file\n", 13);
        return 1;
    }

    char* width = malloc(4);
    char* length = malloc(4);
    char trailing_char;

    char* map_parameters;
    fgets(map_parameters, 12, map_file);

    if (get_dimensions(map_file, width, &trailing_char) != 0 || trailing_char != 'x') {
        write(2, "invalid dimensions\n", 19);
        return 1;
    }

    if (get_dimensions(map_file, length, &trailing_char) != 0) {
        write(2, "invalid dimensions\n", 19);
        return 1;
    }
    
    char full = trailing_char;
    char empty = fgetc(map_file);
    char path = fgetc(map_file);
    char maze_entrance = fgetc(map_file);
    char maze_exit = fgetc(map_file);

    if (fgetc(map_file) != '\n') {
        write(2, "invalid dimensions\n", 19);
        return 1;
    }

//  read the map and store it as a 2D array representation
    int total_rows = atoi(width);
    int total_columns = atoi(length);

    int maze_arr[total_rows + 1][total_columns + 1];
    cell entry_cell;
    cell exit_cell;

    if (create_2d_arr(map_file, total_columns, maze_arr, full, empty, maze_entrance, maze_exit, &entry_cell, &exit_cell) != 0){
        write(2, "MAP ERROR", 9);
        return 1;
    }

// using a-star, find the shortest path or return NULL if no solution 
    cell* path_list;

    if ((path_list = a_star(total_rows, total_columns, maze_arr, &entry_cell, &exit_cell)) == NULL){
        write(2, "MAP ERROR", 9);
        return 1;

    } else path_list = reverse_linked_list(path_list);

//  reconstruct map using "path" character indicating shortest path
    printf("%dx%d%c%c%c%c%c\n", total_rows, total_columns, full, empty, path, maze_entrance, maze_exit);

    print_solution(total_rows, total_columns, maze_arr, path_list, full, empty, path, maze_entrance, maze_exit);

    fclose(map_file);

    return 0;
}
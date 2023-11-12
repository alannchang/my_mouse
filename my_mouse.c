#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct{
    int total_rows;
    int total_columns;
    int** arr;
} map;

typedef struct Node{
    int row, col, f, g, h;
    struct Node* parent;
} cell;

typedef struct {
    cell** open_list;
    cell** closed_list;
    int open_ct;
    int closed_ct;
} a_star;

typedef struct {
    int row[4];
    int col[4];
} Direction;

int get_dimensions(char** map_parameters, char* dimension, char* trailing_char){
    char* temp = malloc(4);
    int i = 0;
    while (isdigit(**map_parameters)) {
        if (i > 2) return 1;
        temp[i] = **map_parameters;
        (*map_parameters)++;
        i++;
    }
    *trailing_char = **map_parameters;
    (*map_parameters)++;
    temp[i] = '\0';
    dimension = realloc(dimension, i);
    strncpy(dimension, temp, i);
    return 0;
}

int get_chars(char* map_parameters, char* empty, char* path, char* maze_entrance, char* maze_exit){
    *empty = map_parameters[0];
    *path = map_parameters[1];
    *maze_entrance = map_parameters[2];
    *maze_exit = map_parameters[3];
    if (map_parameters[4] != '\n') return -1;
    return 0;
}

map init_map(const char* width, const char* length){
    map map;
    map.total_rows = atoi(width);
    map.total_columns = atoi(length);
    map.arr = (int**)malloc((map.total_rows + 1) * sizeof(int*));
    for (int i = 0; i < map.total_rows + 1; i++) map.arr[i] = (int*)malloc((map.total_columns + 1) * sizeof(int));
    return map;
}

int handle_full(int row_index, int col_index, map map) {
    map.arr[row_index][col_index] = 1;
    return col_index + 1;
}

int handle_empty(int row_index, int col_index, map map) {
    map.arr[row_index][col_index] = 0;
    return col_index + 1;
}

int handle_entrance(int row_index, int col_index, map map, cell* entry_cell) {
    entry_cell->row = row_index;
    entry_cell->col = col_index;
    map.arr[row_index][col_index] = 2;
    return col_index + 1;
}

int handle_exit(int row_index, int col_index, map map, cell* exit_cell) {
    exit_cell->row = row_index;
    exit_cell->col = col_index;
    map.arr[row_index][col_index] = 3;
    return col_index + 1;
}

int handle_newline(int row_index, int col_index, map map) {
    map.arr[row_index][col_index] = '\0';
    return row_index + 1;
}

int create_2d_arr(FILE* map_file, map map, char full, char empty, char maze_entrance, char maze_exit, cell* entry_cell, cell* exit_cell) {
    char line[map.total_columns + 1];
    int row_index = 0;
    int col_index = 0;
    while ((fgets(line, map.total_columns + 1, map_file)) != NULL) {
        int i = 0;
        while(line[i] != '\0'){
            if (line[i] == full) col_index = handle_full(row_index, col_index, map);
            else if (line[i] == empty) 
                col_index = handle_empty(row_index, col_index, map);
            else if (line[i] == maze_entrance) 
                col_index = handle_entrance(row_index, col_index, map, entry_cell);
            else if (line[i] == maze_exit) 
                col_index = handle_exit(row_index, col_index, map, exit_cell);
            else if (line[i] == '\n'){
                row_index = handle_newline(row_index, col_index, map);
                col_index = 0;
            }
            else return 1;
            i++;
        }
    }
    return 0;
}

bool is_valid(int new_row, int new_col, map map){
    return (new_row >= 0) && (new_row < map.total_rows) && (new_col >= 0) && (new_col < map.total_columns);
}

bool reached_exit(cell* current_cell, cell* exit_cell){
    return (current_cell->row == exit_cell->row) && (current_cell->col == exit_cell->col);
}

int get_h(int row, int col, cell* exit_cell){
    int h = abs(row - exit_cell->row) + abs(col - exit_cell->col);
    return h;
}

cell* find_lowest_f_cell(cell** open_list, int open_ct, int* lowest_f_index) {
    for (int i = 1; i < open_ct; i++) {
        if (open_list[i]->f < open_list[*lowest_f_index]->f) *lowest_f_index = i;
    }
    return open_list[*lowest_f_index];
}

bool is_in_list(cell** list, int count, int row, int col) {
    for (int i = 0; i < count; i++) {
        if (list[i]->row == row && list[i]->col == col) return true;
    }
    return false;
}

a_star init_a_star(map map, cell* entry_cell){
    a_star a_star;
    a_star.open_list = malloc(map.total_rows * map.total_columns * sizeof(cell*));
    a_star.closed_list = malloc(map.total_rows * map.total_columns * sizeof(cell*));
    a_star.open_ct = 0;
    a_star.closed_ct = 0;
    a_star.open_list[a_star.open_ct++] = entry_cell;
    return a_star;
}

void init_entry_cell(cell* entry_cell){
    entry_cell->f = 0;
    entry_cell->g = 0;
    entry_cell->h = 0;
    entry_cell->parent = NULL;
}

void init_successor(cell* successor, int new_row, int new_col, cell* current_cell, cell* exit_cell){
    successor->row = new_row;
    successor->col = new_col;
    successor->parent = current_cell;
    successor->g = current_cell->g + 1;
    successor->h = get_h(new_row, new_col, exit_cell);
}

cell* a_star_algo(map map, cell* entry_cell, cell* exit_cell){

    init_entry_cell(entry_cell);
    a_star a_star = init_a_star(map, entry_cell);
    
    while (a_star.open_ct > 0){
        int lowest_f_index = 0;
        cell* current_cell = find_lowest_f_cell(a_star.open_list, a_star.open_ct, &lowest_f_index);
        for (int i = lowest_f_index; i < a_star.open_ct - 1; i++) a_star.open_list[i] = a_star.open_list[i + 1];
        a_star.open_ct--;
        a_star.closed_list[a_star.closed_ct++] = current_cell;
        if (reached_exit(current_cell, exit_cell)) return current_cell;
        Direction direction = {{-1, 0, 0, 1}, {0, -1, 1, 0}};
        for (int i = 0; i < 4; i++){
            int new_row = current_cell->row + direction.row[i];
            int new_col = current_cell->col + direction.col[i];
            if (is_valid(new_row, new_col, map) && map.arr[new_row][new_col] != 1){
                bool in_closed_list = is_in_list(a_star.closed_list, a_star.closed_ct, new_row, new_col);
                if (!in_closed_list){
                    cell* successor = (cell*)malloc(sizeof(cell));
                    init_successor(successor, new_row, new_col, current_cell, exit_cell);
                    bool in_open_list = is_in_list(a_star.open_list, a_star.open_ct, new_row, new_col);
                    if (!in_open_list) a_star.open_list[a_star.open_ct++] = successor;
                    else free(successor);
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

void print_solution(map map, cell* path_list, char full, char empty, char path, char maze_entrance, char maze_exit){
    int ct = 0;
    for (int i = 0; i < map.total_rows; i++){
        for (int j = 0; j < map.total_columns; j++) {
            switch(map.arr[i][j]){
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
    FILE* map_file = fopen(ac[1], "r");
    char* width = malloc(4);
    char* length = malloc(4);
    char trailing_char;
    char* map_parameters = malloc(12);
    fgets(map_parameters, 15, map_file);
    get_dimensions(&map_parameters, width, &trailing_char);
    get_dimensions(&map_parameters, length, &trailing_char);
    char full = trailing_char;
    char empty, path, maze_entrance, maze_exit;
    get_chars(map_parameters, &empty, &path, &maze_entrance, &maze_exit);
    map map = init_map(width, length);
    cell entry_cell, exit_cell;

    if (create_2d_arr(map_file, map, full, empty, maze_entrance, maze_exit, &entry_cell, &exit_cell) != 0){
        write(2, "MAP ERROR", 9);
        return 1;
    }
    cell* path_list;

    if ((path_list = a_star_algo(map, &entry_cell, &exit_cell)) == NULL){
        write(2, "MAP ERROR", 9);
        return 1;

    } else path_list = reverse_linked_list(path_list);
    printf("%dx%d%c%c%c%c%c\n", map.total_rows, map.total_columns, full, empty, path, maze_entrance, maze_exit);
    print_solution(map, path_list, full, empty, path, maze_entrance, maze_exit);
    free(map.arr);
    return 0;
}
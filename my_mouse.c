#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
typedef struct{
    char full;
    char empty;
    char path; 
    char maze_entrance;
    char maze_exit;
} params;

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

typedef struct {
    int row;
    int col;
} new;

int get_dimensions(char** map_details, char* dimension, char* trailing_char){
    char* temp = malloc(4);
    int i = 0;
    while (isdigit(**map_details)) {
        if (i > 2) return 1;
        temp[i] = **map_details;
        (*map_details)++;
        i++;
    }
    *trailing_char = **map_details;
    (*map_details)++;
    temp[i] = '\0';
    dimension = realloc(dimension, i);
    strncpy(dimension, temp, i);
    return 0;
}

params set_params(char trailing_char, char* map_details){
    params params;
    params.full = trailing_char;
    params.empty = map_details[0];
    params.path = map_details[1];
    params.maze_entrance = map_details[2];
    params.maze_exit = map_details[3];
    return params;
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

int create_2d_arr(FILE* map_file, map map, params params, cell* entry_cell, cell* exit_cell) {
    char line[map.total_columns + 1];
    int row_index = 0;
    int col_index = 0;
    while ((fgets(line, map.total_columns + 1, map_file)) != NULL) {
        int i = 0;
        while(line[i] != '\0'){
            if (line[i] == params.full) col_index = handle_full(row_index, col_index, map);
            else if (line[i] == params.empty) 
                col_index = handle_empty(row_index, col_index, map);
            else if (line[i] == params.maze_entrance) 
                col_index = handle_entrance(row_index, col_index, map, entry_cell);
            else if (line[i] == params.maze_exit) 
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

bool is_valid(new new, map map){
    return (new.row >= 0) && (new.row < map.total_rows) && (new.col >= 0) && (new.col < map.total_columns);
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

bool is_in_list(cell** list, int count, new new) {
    for (int i = 0; i < count; i++) {
        if (list[i]->row == new.row && list[i]->col == new.col) return true;
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

new init_new(cell* current_cell, Direction direction, int i){
    new new;
    new.row = current_cell->row + direction.row[i];
    new.col = current_cell->col + direction.col[i];
    return new;
}

void init_successor(cell* successor, int new_row, int new_col, cell* current_cell, cell* exit_cell){
    successor->row = new_row;
    successor->col = new_col;
    successor->parent = current_cell;
    successor->g = current_cell->g + 1;
    successor->h = get_h(new_row, new_col, exit_cell);
}

void check_lists(cell* current_cell, Direction direction, map map, a_star* a_star, cell* exit_cell){
    for (int i = 0; i < 4; i++){
        new new = init_new(current_cell, direction, i);
        if (is_valid(new, map) && map.arr[new.row][new.col] != 1){
            bool in_closed_list = is_in_list(a_star->closed_list, a_star->closed_ct, new);
            if (!in_closed_list){
                cell* successor = (cell*)malloc(sizeof(cell));
                init_successor(successor, new.row, new.col, current_cell, exit_cell);
                bool in_open_list = is_in_list(a_star->open_list, a_star->open_ct, new);
                if (!in_open_list) a_star->open_list[a_star->open_ct++] = successor;
                else free(successor);
            }
        }
    }
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
        check_lists(current_cell, direction, map, &a_star, exit_cell);
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

cell* solve_map(FILE* map_file, map map, params params, cell* entry_cell, cell* exit_cell, cell** path_list){
    
    if (create_2d_arr(map_file, map, params, entry_cell, exit_cell) != 0) return NULL;
    if ((*path_list = a_star_algo(map, entry_cell, exit_cell)) == NULL) return NULL;
    else return reverse_linked_list(*path_list);
}

void print_solution(map map, cell* path_list, params params){
    int ct = 0;
    for (int i = 0; i < map.total_rows; i++){
        for (int j = 0; j < map.total_columns; j++) {
            switch(map.arr[i][j]){
                case 0: 
                    if ((path_printer(i, j, path_list, params.empty, params.path)) == 1) ct++;
                    break;
                case 1: 
                    putchar(params.full);
                    break;
                case 2: 
                    putchar(params.maze_entrance);
                    break;
                case 3: 
                    putchar(params.maze_exit);
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

void printer(map map, params params, cell* path_list){
    printf("%dx%d%c%c%c%c%c\n", map.total_rows, map.total_columns, params.full, params.empty, params.path, params.maze_entrance, params.maze_exit);
    print_solution(map, path_list, params);
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
    char* map_details = malloc(12);
    fgets(map_details, 15, map_file);
    get_dimensions(&map_details, width, &trailing_char);
    get_dimensions(&map_details, length, &trailing_char);
    params params = set_params(trailing_char, map_details);
    map map = init_map(width, length);
    cell entry_cell, exit_cell;

    cell* path_list;
    if ((path_list = solve_map(map_file, map, params, &entry_cell, &exit_cell, &path_list)) == NULL){
        write(2, "MAP ERROR", 9);
        return 1;     
    }   

    printer(map, params, path_list);
    free(map.arr);
    return 0;
}
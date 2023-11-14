#ifndef HELPERS_H
#define HELPERS_H

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
} symbol;

typedef struct{
    int total_rows;
    int total_columns;
    int** arr;
} map;

typedef struct{
    char* width;
    char* length;
    char trailing_char;
    char* first_line;
} parameter;

typedef struct Node{
    int row, col, f, g, h;
    struct Node* parent;
} cell;

typedef struct{
    cell** open_list;
    cell** closed_list;
    int open_ct;
    int closed_ct;
} a_star;

typedef struct{
    int row[4];
    int col[4];
} direction;

typedef struct{
    int row;
    int col;
} newx;

int get_dimensions(char** first_line, char* dimension, char* trailing_char);

parameter set_parameters(FILE* map_file);

symbol set_symbols(char trailing_char, char* first_line);

map init_map(const char* width, const char* length);

int handle_full(int row_index, int col_index, map map);

int handle_empty(int row_index, int col_index, map map);

int handle_entrance(int row_index, int col_index, map map, cell* entry_cell);

int handle_exit(int row_index, int col_index, map map, cell* exit_cell);

int handle_newline(int row_index, int col_index, map map);

int create_2d_arr(FILE* map_file, map map, symbol symbol, cell* entry_cell, cell* exit_cell);

bool is_valid(new new, map map);

bool reached_exit(cell* current_cell, cell* exit_cell);

int get_h(int row, int col, cell* exit_cell);

cell* find_lowest_f_cell(cell** open_list, int open_ct, int* lowest_f_index);

bool is_in_list(cell** list, int count, new new);

a_star init_a_star(map map, cell* entry_cell);

void init_entry_cell(cell* entry_cell);

new init_new(cell* current_cell, direction dir, int i);

void init_successor(cell* successor, int new_row, int new_col, cell* current_cell, cell* exit_cell);

void check_lists(cell* current_cell, direction dir, map map, a_star* a_star, cell* exit_cell);

cell* a_star_algo(map map, cell* entry_cell, cell* exit_cell);

cell* reverse_linked_list(cell* param_1);

int path_printer(int i, int j, cell* solution, char empty, char path);

cell* solve_map(FILE* map_file, map map, symbol symbol, cell* entry_cell, cell* exit_cell, cell** path_list);

void print_solution(map map, cell* path_list, symbol symbol);

void printer(map map, symbol symbol, cell* path_list);

#endif
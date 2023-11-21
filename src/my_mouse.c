#include "../include/my_mouse.h"

int main(int av, char** ac){
    if (av != 2) {
        write(2, "invalid number of arguments\n", 27); 
        return 1;
    }
    FILE* map_file = fopen(ac[1], "r");
    parameter parameter = set_parameters(map_file);
    symbol symbol = set_symbols(parameter.trailing_char, parameter.first_line);
    map map = init_map(parameter.width, parameter.length);
    cell entry_cell, exit_cell;
    cell* path_list;
    if ((path_list = solve_map(map_file, map, symbol, &entry_cell, &exit_cell, &path_list)) == NULL){
        write(2, "MAP ERROR", 9);
        return 1;     
    }   
    printer(map, symbol, path_list);
    free(map.arr);
    return 0;
}
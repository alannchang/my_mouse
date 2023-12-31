#include "../include/my_mouse.h"

int main(int ac, char** av){

    if(check_arg_ct(ac) != 0) return 1;
    
    FILE* map_file = fopen(av[1], "r");
    if (map_file == NULL){
        write(2, "invalid file", 12);
        return 1;
    }

    parameter param = set_parameters(map_file);
    symbol symbol = set_symbols(param.trailing_char, param.ptr);
    map map = init_map(param.width, param.length);
    free(param.first_line);
    free(param.width);
    free(param.length);
    cell entry_cell, exit_cell;
    cell* path_list;
    if ((path_list = solve_map(map_file, map, symbol, &entry_cell, &exit_cell, &path_list)) == NULL){
        write(2, "MAP ERROR", 9);
        return 1;     
    }
    printer(map, symbol, path_list);
    for (int i = 0; i < map.total_rows + 1; i++) free(map.arr[i]);
    free(map.arr);
    return 0;
}
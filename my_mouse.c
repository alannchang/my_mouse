#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <cstddef>


int get_dimensions(FILE* map_file, char stopchar, char* width, size_t i){

    char* temp = malloc(4);
    i = 0;

    while((temp[i] = fgetc(map_file)) != stopchar) {

        if (temp[i] == EOF || temp[i] == '\n') return 1;
        
        if (i > 3) return 1;

        i++;
    }
    temp[i] = '\0';

    char* width = malloc(i);
    return 0;
}


int main(int av, char** ac){

//  1. read map while checking if map is valid

    if (av != 2) {
        write(2, "invalid number of arguments\n", 29); 
        return 1;
    }

    FILE* map_file = fopen(ac[1], "r");
    if (map_file == NULL) {
        write(2, "invalid file\n", 13);
        return 1;
    }

    char* width;
    size_t size = 0;
    get_dimensions(map_file, 'x', width, size);
    strncpy(width, temp, i);

    printf("width = %s\n", width);


    fclose(map_file);

//  2. Construct corresponding matrix/graph

//  3. Find shortest path

//  4. reconstruct/print map to stdout with "path" characters indicating shortest path

    return 0;
}
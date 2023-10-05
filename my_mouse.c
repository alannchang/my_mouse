#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int get_dimensions(FILE* map_file, char* width){

    char* temp = malloc(4);
    int i = 0;

    while(isdigit(temp[i] = fgetc(map_file))) {
        
        if (i > 2) return 1;

        i++;
    }

    temp[i] = '\0';

    width = realloc(width, i);
    strncpy(width, temp, i);

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

    char* width = malloc(4);
    char* height = malloc(4);
    get_dimensions(map_file, width);
    get_dimensions(map_file, height);

    printf("width = %s\n", width);
    printf("length = %s\n", height);

    fclose(map_file);

//  2. Construct corresponding matrix/graph

//  3. Find shortest path

//  4. reconstruct/print map to stdout with "path" characters indicating shortest path

    return 0;
}
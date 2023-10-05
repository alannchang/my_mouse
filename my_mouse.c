#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    char trailing_char;

    if (get_dimensions(map_file, width, &trailing_char) != 0 || trailing_char != 'x') {
        write(2, "invalid dimensions", 19);
        return 1;
    }

    if (get_dimensions(map_file, height, &trailing_char) != 0) {
        write(2, "invalid dimensions", 19);
        return 1;
    }

    printf("width = %s\n", width);
    printf("length = %s\n", height);
    
    char full = trailing_char;
    printf("FULL = %c\n", full);



    fclose(map_file);

//  2. Construct corresponding matrix/graph

//  3. Find shortest path

//  4. reconstruct/print map to stdout with "path" characters indicating shortest path

    return 0;
}
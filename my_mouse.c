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

    int total_rows = atoi(width);
    int total_columns = atoi(length);
    int row_index = 0;
    int col_index = 0;

    int maze[total_rows + 1][total_columns + 1];

    char temp;
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
            maze[row_index][col_index] = 2;
            col_index++;
        } 
        
        else if (temp == maze_exit) {
            maze[row_index][col_index] = 3;
            col_index++;
        } 
        
        else if (temp == '\n') {
            maze[row_index][col_index] = '\0';
            col_index = 0;
            row_index++;
        }

        else {
            write(2, "MAP ERROR", 10);
            return 1;
        }

    }

    for (int i = 0; i < total_rows; i++){
        for (int j = 0; j < total_columns; j++) {
            printf("%d", maze[i][j]);
        }
        printf("\n");
    }

    fclose(map_file);

//  2. Construct corresponding matrix/graph

//  3. Find shortest path

//  4. reconstruct/print map to stdout with "path" characters indicating shortest path

    return 0;
}
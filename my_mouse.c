#include <stdio.h>
#include <unistd.h>



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

    char line[1000];

    while (fgets(line, sizeof(line), map_file) != NULL) {
        printf("%s", line);
    }

    fclose(map_file);

//  2. Construct corresponding matrix/graph

//  3. Find shortest path

//  4. reconstruct/print map to stdout with "path" characters indicating shortest path

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#define MATRIX_SIZE 50
#define clear() printf("\033[H\033[J")

int matrix[MATRIX_SIZE][MATRIX_SIZE] = {{0}};

void print_matrix() {
    for (short int i = 0; i < MATRIX_SIZE; ++i) {
        for (short int j = 0; j < MATRIX_SIZE; ++j) {
            if (matrix[i][j] == 1) {
                printf("%c ", '+');
            }
            else {
                printf("%c ", ' ');
            }
        }
        printf("\n");
    }
}

int get_living_cell_count() {
    int living_cells = 0;
    for (short int i = 0; i < MATRIX_SIZE; ++i) {
        for (short int j = 0; j < MATRIX_SIZE; ++j) {
            if (matrix[i][j] == 1) {
                living_cells++;
            }
        }
    }
    return living_cells;
}

int get_all_neighbours(int x, int y) {
    int ret = 0;
    int directions[8][2] = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0},   // cardinal
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}  // diagonal
    };
    
    for (int i = 0; i < 8; i++) {
        int nx = x + directions[i][0];
        int ny = y + directions[i][1];
        if (nx >= 0 && nx < MATRIX_SIZE && ny >= 0 && ny < MATRIX_SIZE) {
            if (matrix[nx][ny] == 1) {
                ret++;
            }
        }
    }
    return ret;
}

void kill_cell(int x, int y) {
    matrix[x][y] = 0;
}

void revive_cell(int x, int y) {
    matrix[x][y] = 1;
}

bool is_cell_alive(int x, int y) {
    if (matrix[x][y] == 1) {
        return true;
    }
    else {
        return false;
    }
}

//TODO fix this. Currently reading the file messes up the matrix due to \n characters.
bool read_matrix_from_file() {
    FILE* f;
    f = fopen("grid.txt", "r");
    if (!f) { //Check if file exists and opened successfully.
        return false;
    }
    
    const size_t FILE_MATRIX_SIZE = MATRIX_SIZE * MATRIX_SIZE;

    char file_matrix[FILE_MATRIX_SIZE];
    size_t used = 0;
    
    // Reads the whole file into the 800 sized buffer.
    while (fgets(file_matrix + used, FILE_MATRIX_SIZE - used, f)) {
        // Remove Newline at the end of string.
        char* p = strchr(file_matrix + used, '\n');
        if (p) {
            *p = '\0';
        }

        used = strlen(file_matrix);
        if (used >= FILE_MATRIX_SIZE - 1) {
            break;
        }
    }
    
    printf("%s\n", file_matrix);
    
    fclose(f);

    for (size_t k = 0; k < FILE_MATRIX_SIZE; k++) {
        if (file_matrix[k] == '1') {
            int row = k / MATRIX_SIZE;
            int col = k % MATRIX_SIZE;
            matrix[row][col] = 1;
        }
    }
    return true;
}

int main() {
    if (!read_matrix_from_file()) {
        printf("Something went terribly wrong when opening the file! Please try again.\n");
    }
    print_matrix();
    
    while (get_living_cell_count() > 0) {
        clear();
        print_matrix();
        for (short int i = 0; i < MATRIX_SIZE; ++i) {
            for (short int j = 0; j < MATRIX_SIZE; ++j) {
                if (is_cell_alive(i, j)) {
                    if (get_all_neighbours(i, j) <= 1) {
                        kill_cell(i, j);
                    }
                    if (get_all_neighbours(i, j) >= 4) {
                        kill_cell(i, j);
                    }
                }
                if (!is_cell_alive(i, j)) {
                    if (get_all_neighbours(i, j) == 3) {
                        revive_cell(i, j);
                    }    
                }
            }
        }
        sleep(1);
    }
    
    printf("All cells dead!\n");
    return 0;
}
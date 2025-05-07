#include <stdio.h>
#include <stdbool.h>

#define N 9
#define NA 0

void print_grid(int grid[N][N]);
bool pos_safe(int grid[N][N], int row, int col, int num);   
bool solve(int grid[N][N]);
void modify_grid(int grid[N][N]);
void clear_grid(int grid[N][N]);

int main(){
    int grid[N][N] = {0};
    int choice;
    printf("--- Sudoku Solver ---\n");
    while (true){
        printf("1. Modify grid\n");
        printf("2. Solve grid\n");
        printf("3. Print grid\n");
        printf("4. Clear grid\n");
        printf("5. Exit\n");
        printf("Choice (1/2/3/4/5): ");
        scanf("%d", &choice);
        if (choice == 1){
            print_grid(grid);
            modify_grid(grid);
        }
        else if(choice == 2){
            printf("\nAttempting to Solve the puzzle...\n");
            if(solve(grid)){
                print_grid(grid);
            }
            else{
                printf("There is no solution to the puzzle\n"); 
            }
        }
        else if(choice == 3){
            print_grid(grid);
        }
        else if(choice == 4){
            clear_grid(grid);
            print_grid(grid);
        }
        else{
            break;
        }
    }
}

void modify_grid(int grid[N][N]){
    int row; int col;
    printf("Enter the row: ");
    scanf("%d", &row);
    printf("Enter the column: ");
    scanf("%d", &col);
    printf("Enter the number you want to change in row %d and column %d: ", row, col);
    scanf("%d", &grid[row][col]);
    print_grid(grid);
}

void print_grid(int grid[N][N]) {
    printf("    "); // adds padding to top left corner
    for (int i = 0; i < N; i++) {
        printf("%-3d", i);
        if ((i + 1) % 3 == 0 && i != N - 1) printf(" "); // adds padding between columns
    }
    printf("\n");

    for (int i = 0; i < N; i++) {
        printf("%-3d", i);
        printf("| ");
        for (int j = 0; j < N; j++) {
            if (grid[i][j] == 0)
                printf(".  ");
            else
                printf("%-3d", grid[i][j]);

            if ((j + 1) % 3 == 0 && j != N - 1) printf(" ");
        }
        printf("\n");

        if ((i + 1) % 3 == 0 && i != N - 1) {
            printf("    ");
            for (int k = 0; k < N + 2; k++) printf("--");
            printf("------\n");
        }
    }
}

void clear_grid(int grid[N][N]) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            grid[i][j] = 0;
}

bool pos_safe(int grid[N][N], int row, int col, int num){
    //check if the number is safe to place in the given position
    int r_start = row - (row % 3);
    int c_start = col - (col % 3);
    int rlim = row - (row % 3) + 3;
    int clim = col - (col % 3) + 3;
    // Checking the sub grid
    for(int i = r_start; i < rlim; i ++){
        for(int j = c_start; j < clim; j ++){
            if (num == grid[i][j]){
                return false;
            }
        }
    }
    // Checking rows and columns simultaneously
    for(int i = 0; i < N; i ++){
        if (num == grid[i][col] || num == grid[row][i]){
            return false;
        }
    }
    return true;
}



bool solve(int grid[N][N]){
    for(int i = 0; i < N; i ++){
        for (int j = 0; j < N; j++){
            if (grid[i][j] == 0){ // Checking whether the position is empty
                for (int num = 1; num < 10; num ++){
                    if (pos_safe(grid, i, j, num))// Checking if the position is safe
                    {
                        grid[i][j] = num; // Placing the number here and attempting to solve the grid
                        if (solve(grid)){
                            return true; // If grid is solved 
                        }
                        
                        grid[i][j] = 0; // Otherwise reset that position back to 0, and attempt to solve the sudoku once again
                    }
                }
                return false; 
            }
            
        }
    }
    return true; // In case the sudoku provided is already solved
}

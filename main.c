#include <stdio.h>
#include <stdbool.h>

#define N 9
#define NA 0

void print_grid(int grid[N][N]);
bool pos_safe(int grid[N][N], int row, int col, int num);
bool locate_filled(int grid[N][N], int *row, int *col);
bool solve(int grid[N][N]);


int main(){
    int grid[N][N] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };
    printf("--- Sudoku Solver ---\n");
    
    printf("The Sudoku grid you entered is:\n");
    print_grid(grid);
    printf("\nAttempting to Solve the puzzle...\n");
    if(solve(grid)){
        print_grid(grid);
    }
    else{
        printf("There is no solution to the puzzle\n"); 
    }
}


void print_grid(int grid[N][N]){
    for(int i = 0; i < N; i ++){
        for(int j = 0; j < N; j ++){
            printf("%d   ", grid[i][j]);
        }
        printf("\n");
    }
}


bool pos_safe(int grid[N][N], int row, int col, int num){
    //check if the number is safe to place in the given position
    bool flag = true; // setting flag to true as the default case
    int r_start = row - (row % 3);
    int c_start = col - (col % 3);
    int rlim = row - (row % 3) + 3;
    int clim = col - (col % 3) + 3;
    // Checking the sub grid
    for(int i = r_start; i < rlim; i ++){
        for(int j = c_start; j < clim; j ++){
            if (num == grid[i][j]){
                flag = false;
            }
        }
    }
    // Checking rows and columns simultaneously
    for(int i = 0; i < N; i ++){
        if (num == grid[i][col] || num == grid[row][i]){
            flag = false;
        }
    }
    return flag;
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

#include <stdio.h>
#include <stdbool.h>

#define N 9
#define NA 0

void print_grid(int grid[N][N]);
bool pos_safe(int grid[N][N], int row, int col, int num);   
bool solve(int grid[N][N]);


int main(){
    int grid[N][N];
    for(int i = 0; i < N; i ++){
        for(int j = 0; j < N; j ++){
            printf("Enter the number in row %d and column %d (0 for blank): ", i, j);
            scanf("%d", &grid[i][j]);
        }
    }
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

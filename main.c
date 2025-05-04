#include <stdio.h>
#include <stdbool.h>

#define N 9
#define NA 0

void print_grid(int grid[N][N]);
bool pos_safe(int grid[N][N], int row, int col, int num);
bool locate_filled(int grid[N][N], int *row, int *col);
bool solve(int grid[N][N],int,int);


int main(){
    int grid[N][N];
    printf("--- Sudoku Solver ---\n");
    printf("Enter the Sudoku grid (0 for empty cells):, row by row, separated by a space.\n");
    for (int i = 0; i < N; i++) {
        printf("Row %d: \nElements:-\n1 2 3 4 5 6 7 8 9\n", i + 1);
        scanf("%d %d %d %d %d %d %d %d %d", &grid[i][0], &grid[i][1], &grid[i][2], &grid[i][3], &grid[i][4], &grid[i][5], &grid[i][6], &grid[i][7], &grid[i][8]);
    }
    printf("The Sudoku grid you entered is:\n");
    print_grid(grid);
    printf("\nAttempting to Solve the puzzle...\n");
    //core logic here
}


void print_grid(int grid[N][N]){
    //print logic
}


bool pos_safe(int grid[N][N], int row, int col, int num){
    //check if the number is safe to place in the given position
    bool flag = true; // setting flag to true as the default case
    int r_start = row - (row % 3);
    int c_start = col - (col % 3);
    int rlim = row - (row % 3) + 3;
    int clim = col - (col % 3) + 3;
    for(int i = r_start; i < rlim; i ++){
        for(int j = c_start; j < clim; j ++){
            if (num == grid[i][j]){
                flag = false;
            }
        }
    }
    for(int i = 0; i < N; i ++){
        if (num == grid[i][col] || num == grid[row][i]){
            flag = false;
        }
    }
    return flag;
}


bool locate_filled(int grid[N][N], int *row, int *col){
    //find a filled position in the grid
    for (*row = 0; *row < N; (*row)++) {
        for (*col = 0; *col < N; (*col)++) {
            if (grid[*row][*col] == 0) {
                return false;
            }
        }
    }
    return true;
}//instead of locating empty, i located the filled, and when filled cell is found, idea is to just go to next cell


bool solve(int grid[N][N],int row, int col){
    //solve logic
    if (row == 9)//solving row after row, rows--> 0 to 8, i.e. 9 means out of bounds
    print_grid(grid);

    if (col == 9) 
    return recurr(grid, row + 1, 0);
    
    if (locate_filled(grid,&row,&col))
    return recurr(grid, row, col + 1);//i.e. since it is filled, you skip this cell and go to the next one
}

#include <stdio.h>
#include <stdbool.h>

#define N 9
#define NA 0

void print_grid(int grid[N][N]);
bool pos_safe(int grid[N][N], int row, int col, int num);
bool locate_empty(int grid[N][N], int *row, int *col);
bool solve(int grid[N][N]);

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
}
bool locate_empty(int grid[N][N], int *row, int *col){
    //find an empty position in the grid
}
bool solve(int grid[N][N]){
    //solve logic
}

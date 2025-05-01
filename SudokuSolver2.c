#include <stdio.h>
#include <stdbool.h>

bool check(int board[9][9], int row, int col, int num) 
{
    // checks for the num in row or column
    for (int i = 0; i < 9; i++)
    {
        if (board[row][i] == num ||board[i][col] == num) 
            return false;
    }


    // checks each 3/3 box
    int smolrow = (row / 3) * 3; //rows 0,1,2 --> smolrow= 0*3     3,4,5--> smolrow=1*3     6,7,8--> 2*3
    int smolcol = (col / 3) * 3;
    for (int i = smolrow; i < smolrow + 3; i++) 
    {
        for (int j = smolcol; j < smolcol + 3; j++) 
        {
            if (board[i][j] == num) 
                return false;
        }
    }

    return true;
}

bool recurr(int board[9][9], int row, int col) 
{
    if (row == 9) //this is the end of board, printing
    {
        for (int i = 0; i < 9; i++) 
        {
            for (int j = 0; j < 9; j++) 
            printf("%d ", board[i][j]);
            printf("\n");
        }
        return true; //returning true or false doesn't make a diff, just a terminating return,
            // i did return true to stop the warning msg that i'm returning no value in non-void fn 
    }

    if (col == 9) 
        return recurr(board, row + 1, 0);

    if (board[row][col]) 
        return recurr(board, row, col + 1);


    for (int num = 1; num <= 9; num++) 
    {
        if (check(board, row, col, num)) 
        {
            board[row][col] = num;
            if (recurr(board, row, col + 1)) 
            return true;
            else
            board[row][col] = 0;
        }
    }
    return false;
}

int main()
{
    int board[9][9] = {
        {0, 7, 0, 0, 0, 3, 0, 5, 0},
        {0, 0, 0, 0, 2, 0, 7, 0, 0},
        {8, 9, 0, 6, 0, 0, 0, 0, 2},
        {0, 1, 0, 0, 0, 9, 3, 0, 5},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {2, 0, 5, 8, 0, 0, 0, 1, 0},
        {5, 0, 0, 0, 0, 2, 0, 4, 1},
        {0, 0, 1, 0, 9, 0, 0, 0, 0},
        {0, 3, 0, 7, 0, 0, 0, 8, 0}
        };

    recurr(board, 0, 0);
    
}

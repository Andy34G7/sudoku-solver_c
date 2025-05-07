#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>
#include <string.h>


#define N 9
#define NA 0
void init_ncurses();
void end_ncurses();
void print_grid(int grid[N][N]);
bool pos_safe(int grid[N][N], int row, int col, int num);
bool locate_filled(int grid[N][N], int *row, int *col);
bool solve(int grid[N][N],int,int);
void get_input(int grid[N][N]);
void display_message(const char *message);

int main(){
    init_ncurses();
    int grid[N][N];
    memset(grid, 0, sizeof(grid));
    print_grid(grid);
    display_message("Welcome to Sudoku Solver!\nPress 'q' to quit, 's' to solve, and arrow keys to navigate.\nPress '0' to clear a cell. \nPress 'r' to reset the grid.");
    int row = 0, col = 0;
    int ch;
    get_input(grid);
    end_ncurses();
    return 0;
}

void init_ncurses() {
    initscr();
    cbreak(); //Disable Line buffering
    noecho();
    keypad(stdscr, TRUE); // Enable function keys
    curs_set(1);          // Show the cursor
}

void end_ncurses() {
    endwin();
}

void print_grid(int grid[N][N]){
    // grid display
    int start_row = 6;
    int start_col = 2; // Adjust starting column for better centering

    // Clear area for grid printing
    // We need to clear enough rows and columns to encompass the entire grid including borders
    int grid_height = N * 2 + (N/3)*2 + 3; // Height including top/bottom borders and spacing
    int grid_width = N * 4 + (N/3)*2 + 3; // Width including left/right borders and spacing

    for (int i = 0; i < grid_height; i++) {
        mvhline(start_row - 1 + i, start_col - 1, ' ', grid_width);
    }

    // Draw the grid with box drawing characters
    for (int i = 0; i <= N; i++) {
        // Calculate the row for drawing lines. Extra space every 3 rows for thicker lines.
        int line_row = start_row + i * 2 + (i / 3);

        if (i < N) {
            // Draw horizontal lines within the grid
            chtype h_line = (i % 3 == 0) ? ACS_HLINE : ACS_HLINE; // Use ACS_HLINE for all horizontal lines for simplicity
            mvhline(line_row, start_col, h_line, N * 4 + (N/3)*2);
        }

        for (int j = 0; j <= N; j++) {
            // Calculate the column for drawing lines. Extra space every 3 columns for thicker lines.
            int line_col = start_col + j * 4 + (j / 3) * 2;

            if (j < N) {
                // Draw vertical lines within the grid
                 chtype v_line = (j % 3 == 0) ? ACS_VLINE : ACS_VLINE; // Use ACS_VLINE for all vertical lines
                 // Adjust the height of vertical lines to stop at the bottom border
                 // The height should be the number of rows used by the grid cells plus the horizontal separators
                 int vline_height = N * 2 + (N/3)*2 + 1; // N rows * 2 lines/row + spacing lines + 1 for bottom border
                 mvvline(start_row, line_col, v_line, vline_height);
            }

            // Draw intersection characters
            chtype intersection = ACS_PLUS;
            if (i % 3 == 0 && j % 3 == 0) intersection = ACS_BSSB; // Box Separator Single from Bottom to Single to Side
            else if (i % 3 == 0) intersection = ACS_TTEE; // Tee pointing up
            else if (j % 3 == 0) intersection = ACS_LTEE; // Tee pointing left
            else intersection = ACS_PLUS;

            // Draw intersection characters at appropriate positions
            if (i < N && j < N) {
                 // Draw numbers
                int num_display_row = start_row + i*2 + (i/3) + 1;
                int num_display_col = start_col + j*4 + (j/3)*2 + 2; // Center number in cell

                if (grid[i][j] != NA) {
                    mvprintw(num_display_row, num_display_col, "%d", grid[i][j]);
                } else {
                    mvprintw(num_display_row, num_display_col, " ");
                }
            } else if (i < N && j == N) {
                 // Draw right border intersections
                 if (i % 3 == 0) mvaddch(line_row, line_col, ACS_RTEE);
                 else mvaddch(line_row, line_col, ACS_RTEE); // Use RTEE for all right border intersections
            } else if (i == N && j < N) {
                 // Draw bottom border intersections
                 if (j % 3 == 0) mvaddch(line_row, line_col, ACS_BTEE);
                 else mvaddch(line_row, line_col, ACS_BTEE); // Use BTEE for all bottom border intersections
            }
        }
    }

    // Draw the outer box corners
    mvaddch(start_row, start_col, ACS_ULCORNER);
    mvaddch(start_row, start_col + N * 4 + (N/3)*2, ACS_URCORNER);
    mvaddch(start_row + N * 2 + (N/3)*2 + 2, start_col, ACS_LLCORNER);
    mvaddch(start_row + N * 2 + (N/3)*2 + 2, start_col + N * 4 + (N/3)*2, ACS_LRCORNER);

    // Draw the outer horizontal and vertical lines to connect the corners
    mvhline(start_row, start_col + 1, ACS_HLINE, N * 4 + (N/3)*2 - 1);
    mvhline(start_row + N * 2 + (N/3)*2 + 2, start_col + 1, ACS_HLINE, N * 4 + (N/3)*2 - 1);
    mvvline(start_row + 1, start_col, ACS_VLINE, N * 2 + (N/3)*2 + 1);
    mvvline(start_row + 1, start_col + N * 4 + (N/3)*2, ACS_VLINE, N * 2 + (N/3)*2 + 1);


    refresh();
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
    return solve(grid, row + 1, 0);
    
    if (locate_filled(grid,&row,&col))
    return solve(grid, row, col + 1);//i.e. since it is filled, you skip this cell and go to the next one
}

void get_input(int grid[N][N]){
    int row = 0, col = 0;
    int ch;

    // Adjust cursor positioning to match the new grid layout
    move(6 + row * 2 + (row/3) + 1, col * 4 + (col/3)*2 + 2); // Adjusted for new grid layout
    refresh();

    while ((ch = getch()) != 'q') { // Loop until 'q' is pressed
        switch (ch) {
            case KEY_UP:
                if (row > 0) row--;
                break;
            case KEY_DOWN:
                if (row < N - 1) row++;
                break;
            case KEY_LEFT:
                if (col > 0) col--;
                break;
            case KEY_RIGHT:
                if (col < N - 1) col++;
                break;
            case '1': case '2': case '3': case '4': case '5':
            case '6': case '7': case '8': case '9':
                grid[row][col] = ch - '0'; // Convert character to integer
                print_grid(grid); // Redraw grid after input
                break;
            case '0':
                grid[row][col] = NA; // Clear the cell
                print_grid(grid); // Redraw grid after clearing
                break;
            case 's':
                display_message("Solving...");
                // Create a copy of the grid to solve, so the original user input is preserved
                int solve_grid[N][N];
                memcpy(solve_grid, grid, sizeof(int[N][N]));

                if (solve(solve_grid,0,0)) { // Call the solve function with the copy
                    display_message("Puzzle Solved!");
                    // Copy the solved grid back to the main grid for display
                    memcpy(grid, solve_grid, sizeof(int[N][N]));
                    print_grid(grid); // Display the solved grid
                } else {
                    display_message("No solution exists for the entered puzzle.");
                }
                // After solving or failing, keep the grid displayed and allow quitting
                // The main loop will continue to wait for 'q'
                break;
             case 'r':
                display_message("Resetting...");
                memset(grid, 0, sizeof(grid)); // Reset the grid
                print_grid(grid); // Clear the grid display
                display_message("Grid reset. Enter a new puzzle."); // Update message
                break;
        }
        // Adjust cursor positioning to match the new grid layout
        move(6 + row * 2 + (row/3) + 1, col * 4 + (col/3)*2 + 2); // Adjusted for new grid layout
        refresh();
    }
}

void display_message(const char *message) {
    // Adjust the starting row for the message area to be below the grid
    int message_row = 6 + N * 2 + (N/3)*2 + 4; // Calculate row based on grid height
    mvhline(message_row, 0, ' ', COLS); // Clear the entire line before printing
    mvprintw(message_row, 0,"%s", message);
    refresh();
}
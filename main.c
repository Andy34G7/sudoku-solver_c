#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h> // For napms, if used for debugging

#define N 9
#define NA 0 // Not Assigned

// Function prototypes
void init_ncurses();
void end_ncurses();
chtype get_intersection_char(int r_line_idx, int c_line_idx, int max_idx);
void print_grid(int grid[N][N]);
bool pos_safe(int grid[N][N], int row, int col, int num);
bool solve(int grid[N][N], int row, int col);
void get_input(int grid[N][N]);
void display_message(const char *message);

int main() {
    init_ncurses();
    int grid[N][N];
    memset(grid, NA, sizeof(grid)); // Initialize grid with NA (0)

    // Initial welcome message and grid display
    display_message("Welcome! Arrows to move, 1-9 to enter, 0 to clear. 's' to solve, 'r' to reset, 'q' to quit.");
    print_grid(grid); // Display the empty grid initially

    get_input(grid); // Start the input loop

    end_ncurses();
    return 0;
}

void init_ncurses() {
    initscr();            // Start curses mode
    cbreak();             // Line buffering disabled, Pass on everty thing to me
    noecho();             // Don't echo() while we do getch
    keypad(stdscr, TRUE); // Enable function keys (like arrow keys)
    curs_set(1);          // Show the cursor (0 for invisible, 1 for normal, 2 for very visible)
    start_color();        // Start color functionality (optional, but good practice)
    // You can define color pairs here if needed, e.g., init_pair(1, COLOR_RED, COLOR_BLACK);
}

void end_ncurses() {
    endwin(); // End curses mode
}

// Helper function to determine the correct box drawing character for an intersection
chtype get_intersection_char(int r_line_idx, int c_line_idx, int max_idx) {
    // r_line_idx: 0 for top border, max_idx for bottom border
    // c_line_idx: 0 for left border, max_idx for right border
    // max_idx is N for Sudoku (N+1 lines)

    bool is_major_h_line = (r_line_idx == 0 || r_line_idx == max_idx || r_line_idx % 3 == 0);
    bool is_major_c_line = (c_line_idx == 0 || c_line_idx == max_idx || c_line_idx % 3 == 0);

    if (r_line_idx == 0) { // Top border
        if (c_line_idx == 0) return ACS_ULCORNER;
        if (c_line_idx == max_idx) return ACS_URCORNER;
        return is_major_c_line ? ACS_TTEE : ACS_HLINE; // Top Tee for major vertical, HLINE for minor
    }
    if (r_line_idx == max_idx) { // Bottom border
        if (c_line_idx == 0) return ACS_LLCORNER;
        if (c_line_idx == max_idx) return ACS_LRCORNER;
        return is_major_c_line ? ACS_BTEE : ACS_HLINE; // Bottom Tee for major vertical, HLINE for minor
    }
    if (c_line_idx == 0) { // Left border (and not top/bottom corner)
        return is_major_h_line ? ACS_LTEE : ACS_VLINE; // Left Tee for major horizontal, VLINE for minor
    }
    if (c_line_idx == max_idx) { // Right border (and not top/bottom corner)
        return is_major_h_line ? ACS_RTEE : ACS_VLINE; // Right Tee for major horizontal, VLINE for minor
    }

    // Internal intersections
    if (is_major_h_line && is_major_c_line) return ACS_PLUS; // Major H / Major V
    if (is_major_h_line && !is_major_c_line) return ACS_HLINE; // Major H / Minor V (part of H line)
    if (!is_major_h_line && is_major_c_line) return ACS_VLINE; // Minor H / Major V (part of V line)
    
    return ACS_PLUS; // Minor H / Minor V
}


void print_grid(int grid[N][N]) {
    int start_row_offset = 6; // Screen row to start drawing the grid
    int start_col_offset = 2; // Screen column to start drawing the grid

    // Calculate actual grid dimensions based on N
    // Height: N cells * 2 lines/cell (number + separator) + N/3 lines for 3x3 block spacing
    int grid_content_height_lines = N * 2 + N / 3;
    // Width: N cells * 4 chars/cell (" X |") + (N/3)*2 chars for 3x3 block spacing (double space)
    int grid_content_width_chars = N * 4 + (N / 3) * 2;

    // Clear area for grid printing (add some padding)
    int clear_height = grid_content_height_lines + 3;
    int clear_width = grid_content_width_chars + 3;
    for (int i = 0; i < clear_height; i++) {
        mvhline(start_row_offset - 1 + i, start_col_offset - 1, ' ', clear_width);
    }

    // Draw all horizontal lines
    for (int i = 0; i <= N; i++) { // N+1 horizontal lines
        int screen_y = start_row_offset + i * 2 + (i / 3);
        // Use ACS_HLINE, could be different for major/minor if desired
        chtype h_line_char = (i == 0 || i == N || i % 3 == 0) ? ACS_HLINE : ACS_HLINE; 
        mvhline(screen_y, start_col_offset, h_line_char, grid_content_width_chars);
    }

    // Draw all vertical lines
    for (int j = 0; j <= N; j++) { // N+1 vertical lines
        int screen_x = start_col_offset + j * 4 + (j / 3) * 2;
        // Use ACS_VLINE, could be different for major/minor if desired
        chtype v_line_char = (j == 0 || j == N || j % 3 == 0) ? ACS_VLINE : ACS_VLINE;
        mvvline(start_row_offset, screen_x, v_line_char, grid_content_height_lines);
    }
    
    // Draw all intersection characters (corners, tees, crosses)
    // This will overwrite parts of the lines drawn above, which is intended.
    for (int i = 0; i <= N; i++) { // Iterate over N+1 line indices
        int screen_y = start_row_offset + i * 2 + (i / 3);
        for (int j = 0; j <= N; j++) {
            int screen_x = start_col_offset + j * 4 + (j / 3) * 2;
            mvaddch(screen_y, screen_x, get_intersection_char(i, j, N));
        }
    }

    // Print numbers in the grid cells
    for (int r = 0; r < N; r++) { // Grid row index
        for (int c = 0; c < N; c++) { // Grid column index
            // Calculate screen position for the number within cell (r,c)
            int num_display_row = start_row_offset + r * 2 + (r / 3) + 1; // +1 to be below the horizontal line
            int num_display_col = start_col_offset + c * 4 + (c / 3) * 2 + 2; // +2 to center in " X "

            if (grid[r][c] != NA) {
                mvprintw(num_display_row, num_display_col, "%d", grid[r][c]);
            } else {
                mvprintw(num_display_row, num_display_col, " "); // Clear cell if NA
            }
        }
    }
    refresh(); // Refresh the screen to show changes
}


bool pos_safe(int grid[N][N], int row, int col, int num) {
    // Check row for the number
    for (int c_check = 0; c_check < N; c_check++) {
        if (grid[row][c_check] == num) {
            return false;
        }
    }

    // Check column for the number
    for (int r_check = 0; r_check < N; r_check++) {
        if (grid[r_check][col] == num) {
            return false;
        }
    }

    // Check 3x3 subgrid for the number
    int box_start_row = row - row % 3;
    int box_start_col = col - col % 3;

    for (int r_check = 0; r_check < 3; r_check++) {
        for (int c_check = 0; c_check < 3; c_check++) {
            if (grid[box_start_row + r_check][box_start_col + c_check] == num) {
                return false;
            }
        }
    }
    return true; // Number is safe to place
}

bool solve(int grid[N][N], int row, int col) {
    // If we've reached beyond the last cell (row 8, col 8), the puzzle is solved.
    // Base case: if col is N, move to next row, col 0.
    if (col == N) {
        row++;
        col = 0;
    }
    // Base case: if row is N, all cells have been processed.
    if (row == N) {
        return true; // Puzzle solved
    }

    // If current cell is already filled, move to the next cell.
    if (grid[row][col] != NA) {
        return solve(grid, row, col + 1);
    }

    // Try placing numbers 1 through N (1-9 for Sudoku)
    for (int num = 1; num <= N; num++) {
        if (pos_safe(grid, row, col, num)) {
            grid[row][col] = num; // Tentatively place the number
            // print_grid(grid); napms(10); // Optional: show solving steps

            // Recursively try to solve the rest of the puzzle
            if (solve(grid, row, col + 1)) {
                return true; // Solution found
            }

            grid[row][col] = NA; // Backtrack: undo the placement if it didn't lead to a solution
            // print_grid(grid); napms(10); // Optional: show backtracking
        }
    }
    return false; // No number from 1-9 worked for this cell, trigger backtracking
}


void get_input(int grid[N][N]) {
    int current_row = 0; // Cursor's current row in the grid
    int current_col = 0; // Cursor's current column in the grid
    int ch;

    // Initial cursor position
    int start_row_offset = 6;
    int start_col_offset = 2;
    move(start_row_offset + current_row * 2 + (current_row / 3) + 1,
         start_col_offset + current_col * 4 + (current_col / 3) * 2 + 2);
    refresh();

    while ((ch = getch()) != 'q') { // Loop until 'q' (quit) is pressed
        switch (ch) {
            case KEY_UP:
                if (current_row > 0) current_row--;
                break;
            case KEY_DOWN:
                if (current_row < N - 1) current_row++;
                break;
            case KEY_LEFT:
                if (current_col > 0) current_col--;
                break;
            case KEY_RIGHT:
                if (current_col < N - 1) current_col++;
                break;
            case '1': case '2': case '3': case '4': case '5':
            case '6': case '7': case '8': case '9':
                // Check if placing the number is valid according to Sudoku rules (optional, but good for user experience)
                // For now, just place it. The solver will determine overall validity.
                grid[current_row][current_col] = ch - '0'; // Convert char '1'-'9' to int 1-9
                print_grid(grid);
                break;
            case '0': // Use '0' or backspace/delete to clear a cell
            case KEY_BACKSPACE:
            case KEY_DC: // Delete character
                grid[current_row][current_col] = NA; // Clear the cell
                print_grid(grid);
                break;
            case 's': // Solve the puzzle
                display_message("Solving... Please wait.");
                int solve_grid_copy[N][N];
                memcpy(solve_grid_copy, grid, sizeof(int[N][N])); // Work on a copy

                if (solve(solve_grid_copy, 0, 0)) {
                    memcpy(grid, solve_grid_copy, sizeof(int[N][N])); // Copy solution back
                    print_grid(grid);
                    display_message("Puzzle Solved! Press 'q' to quit or 'r' to reset.");
                } else {
                    print_grid(grid); // Show original grid if no solution
                    display_message("No solution exists for this puzzle. Press 'q' or 'r'.");
                }
                break;
            case 'r': // Reset the grid
                display_message("Resetting grid...");
                memset(grid, NA, sizeof(grid));
                print_grid(grid);
                display_message("Grid reset. Enter new puzzle or 's' to solve empty, 'q' to quit.");
                current_row = 0; // Reset cursor position
                current_col = 0;
                break;
            default:
                // display_message("Unknown key. Arrows, 1-9, 0, s, r, q."); // Optional: feedback for other keys
                break;
        }
        // Update cursor position after each action
        move(start_row_offset + current_row * 2 + (current_row / 3) + 1,
             start_col_offset + current_col * 4 + (current_col / 3) * 2 + 2);
        refresh();
    }
}

void display_message(const char *message) {
    int start_row_offset = 6;
    // Calculate row for messages, ensuring it's below the grid
    int grid_total_height_lines = N * 2 + N / 3; // Number of lines spanned by grid borders
    int message_screen_row = start_row_offset + grid_total_height_lines + 2; // 2 lines below grid

    move(message_screen_row, 0);   // Move to the start of the message line
    clrtoeol();                    // Clear the line from cursor to end of line
    mvprintw(message_screen_row, 0, "%s", message); // Print the new message
    refresh();                     // Refresh to show the message
}

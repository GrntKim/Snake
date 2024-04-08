#include <ncurses.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#define WINROW 20
#define WINCOL 40
#define SPEED  3.5
#define ROW    0
#define COL    1
#define HEAD   0

int main(void)
{
    int i, key;
    int row = WINROW / 2;
    int col = WINCOL / 2;
    int food_row = rand() % (WINROW - 5) + 3;
    int food_col = rand() % (WINCOL - 5) + 3;
    int next_row = 0;
    int next_col = 0;
    int tail = HEAD;
    int score = 0;
    bool game_over = false;

    int body[WINROW * WINCOL][2] = {0};
    body[HEAD][ROW] = row;
    body[HEAD][COL] = col;

    initscr();
    noecho();
    curs_set(false);

    WINDOW *game_screen = newwin(WINROW, WINCOL, 0, 0);
    keypad(game_screen, true);
    nodelay(game_screen, true);
    srand((unsigned int)time(NULL));

    do {
        key = wgetch(game_screen);
        switch (key) {
            case KEY_LEFT:
                next_row = 0; next_col = -1;
                break;
            case KEY_RIGHT:
                next_row = 0; next_col = 1;
                break;
            case KEY_UP:
                next_row = -1; next_col = 0;
                break;
            case KEY_DOWN:
                next_row = 1; next_col = 0;
                break;
        }
        if (row < 0 || col < 0 || row > WINROW || col > WINCOL) 
            break;

        row += next_row;
        col += next_col;

        if (row == food_row && col == food_col) {
            food_row = rand() % (WINROW - 5) + 3;
            food_col = rand() % (WINCOL - 5) + 3;
            tail++;
        }
        for (i = tail; i > HEAD; i--) {
            body[i][ROW] = body[i-1][ROW];
            body[i][COL] = body[i-1][COL];
        }
        body[HEAD][ROW] = row;
        body[HEAD][COL] = col;

        for (i = HEAD+4; i <= tail; i++) {
            if (body[HEAD][ROW] == body[i][ROW] && 
                body[HEAD][COL] == body[i][COL])
                    game_over = true;
        }
        score = tail;
        wrefresh(game_screen);
        werase(game_screen);
        box(game_screen, 0, 0);
        for (i = HEAD; i <= tail; i++)
            mvwaddch(game_screen, body[i][ROW], body[i][COL], 'S');
        mvwaddch(game_screen, food_row, food_col, 'F');
		mvprintw(WINROW + 1, 0, "Score: %d", score);
        refresh();
        usleep((10 - SPEED) * 10000);
    } while (!game_over);

    werase(game_screen);
    mvwprintw(game_screen, WINROW/2, WINCOL/2 - 5, "GAME OVER");
    wrefresh(game_screen);
    sleep(3);
	endwin();
    printf("GAME OVER\nTotal score: %d\n", score);

	return 0;
}
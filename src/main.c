
#include <ncurses.h>
#include <stdbool.h>
#include <unistd.h>

int maxX;
int maxY;

void toggle_cell(int x, int y, bool field[maxX + 2][maxY + 2])
{
    field[x + 1][y + 1] = !field[x + 1][y + 1];
    attron(COLOR_PAIR(field[x + 1][y + 1] + 1));
    mvprintw(y, x * 2, "  ");
}
void copy_field(bool field[maxX + 2][maxY + 2], bool copy[maxX + 2][maxY + 2])
{

    for (int i = 0; i < maxX + 2; i++) {
        copy[i][0] = 0;
        copy[i][maxY + 1] = 0;
    }
    for (int i = 0; i < maxY + 2; i++) {
        copy[0][i] = 0;
        copy[maxX + 1][i] = 0;
    }

    for (int i = 1; i < maxX + 1; i++) {
        for (int j = 1; j < maxY + 1; j++) {
            copy[i][j] = field[i][j];
        }
    }
}

int get_around(bool c[maxX + 2][maxY + 2], int x, int y)
{
    return //
        c[x + 1][y + 1] + //
        c[x + 1][y - 1] + //
        c[x + 1][y] + //
        c[x - 1][y + 1] + //
        c[x - 1][y - 1] + //
        c[x - 1][y] + //
        c[x][y + 1] + //
        c[x][y - 1] //
        ; //
}
void apply_rules(bool field[maxX + 2][maxY + 2], bool copy[maxX + 2][maxY + 2], int x, int y)

{

    // mvprintw(0, 0, "x: %i y: %i", x, y);
    int around = get_around(copy, x, y);
    if (copy[x][y] && around < 2) { // underpopulation
        toggle_cell(x - 1, y - 1, field);
    } else if (copy[x][y] && around > 3) { // overpopulation
        toggle_cell(x - 1, y - 1, field);
    } else if (!copy[x][y] && around == 3) { // reproduction
        toggle_cell(x - 1, y - 1, field);
    }
}
void play_life(bool field[maxX + 2][maxY + 2])
{

    bool copy[maxX + 2][maxY + 2];
    for (;;) {
        copy_field(field, copy);
        for (int i = 1; i < maxX + 1; i++) {
            for (int j = 1; j < maxY + 1; j++) {
                apply_rules(field, copy, i, j);
            }
        }
        // // debug
        // attron(COLOR_PAIR(1));
        // for (int i = 0; i < maxX + 2; i++) {
        //     for (int j = 0; j < maxY + 2; j++) {
        //         mvprintw(j, i * 2, "%i", copy[i][j]);
        //     }
        // }
        // //

        refresh();
        usleep(500000);
    }
}
void clear_field(bool field[maxX + 2][maxY + 2])
{
    for (int i = 0; i < maxX + 2; i++) {
        for (int j = 0; j < maxY + 2; j++) {
            field[i][j] = false;
        }
    }
}
void handle_input(bool field[maxX + 2][maxY + 2])
{
    int c;
    MEVENT event;
    for (;;) {

        c = getch();
        if (c == 10) { // run life
            play_life(field);
            return;
        }
        if (c == 113) { // quit condiiton
            return;
        }
        if (getmouse(&event) == OK && event.bstate) {
            toggle_cell(event.x / 2, event.y, field);
        }
    }
}
int main()
{
    WINDOW* win = initscr(); /* initialize window */
    keypad(win, TRUE); /* some mouse boilerplate */
    mousemask(BUTTON1_PRESSED | BUTTON3_PRESSED, NULL);
    curs_set(0); // hide cursor
    noecho(); // disable keyboadr
    //
    if (has_colors() == FALSE) { // color boilerplate
        endwin();
        printf("Your terminal does not support color\n");
        return 1;
    }
    start_color();
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(1, COLOR_WHITE, COLOR_BLACK);

    getmaxyx(win, maxY, maxX); /* initialize field to store state*/
    maxX = maxX / 2;
    bool field[maxX + 2][maxY + 2];
    clear_field(field); /* set all cells to false*/

    handle_input(field);
    endwin();
    return 0;
}

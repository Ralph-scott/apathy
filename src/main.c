#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>
#include "apathy.h"

#define ctrl(key) ((key)&0x1f)

int main(int argc, char **argv)
{
    if(argc < 2) return 1; // TODO
    FILE *f = fopen(argv[1], "r");
    char *text = "";
    size_t size = 0;
    if(f) {
        fseek(f, 0l, SEEK_END);
        size = ftell(f);
        fseek(f, 0l, SEEK_SET);
        text = malloc(size);
        PTR_ASSERT(text);
        fread(text, size, 1, f);
        fclose(f);
    }

    GapBuf b = buf_create(text, size);
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    int c = 0;
    while(c != ctrl('c')) {
        erase();
        switch (c) {
            case KEY_LEFT:
                buf_left(&b);
            break;

            case KEY_RIGHT:
                buf_right(&b);
            break;

            case KEY_UP:
                buf_up(&b);
            break;

            case KEY_DOWN:
                buf_down(&b);
            break;

            case KEY_BACKSPACE:
                buf_delete(&b);
            break;

            default:
                if(c >= 10 && c <= 127)
                    buf_append_char(&b, c);
            break;
        }
        size_t size = b.size - b.gap_size > 1000 ? 1000 : b.size - b.gap_size;
        char *display = malloc(size + 1);
        memmove(display, buf_to_string(&b), size);
        display[size] = 0;
        printw("%s", display);
        move(b.y, b.x);
        refresh();
        c = getch();
    }

    f = fopen(argv[1], "w");
    if(f == NULL) return 1; // TODO
    fwrite(b.text, 1, b.gap_start, f);
    fwrite(b.text + b.gap_start + b.gap_size, 1, b.size - b.gap_start - b.gap_size, f);
    fclose(f);

    buf_free(&b);
    endwin();

    return 0;
}

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "gap.h"

#define DEFAULT_SIZE 8

GapBuf buf_create(char *text, size_t size)
{
    PTR_ASSERT(text);
    GapBuf b = (GapBuf) {
        .text = (char *) malloc(size + DEFAULT_SIZE),
        .size = size + DEFAULT_SIZE,
        .gap_start = 0,
        .gap_size = DEFAULT_SIZE,
        .pos = 0,
        .x = 0,
        .y = 0
    };
    PTR_ASSERT(b.text);
    memmove(
        b.text + DEFAULT_SIZE,
        text,
        size
    );
    return b;
}

int buf_char_at(GapBuf *b, size_t pos)
{
    if(pos < b->gap_start)
        return b->text[pos];
    else if(pos > b->size - b->gap_size)
        return b->text[b->gap_size + pos];
    else
        return -1;
}

void buf_free(GapBuf *b)
{
    PTR_ASSERT(b);
    free(b->text);
}

char *buf_to_string(GapBuf *b)
{
    PTR_ASSERT(b);
    char *text;
    text = (char *) malloc(b->size - b->gap_size + 1);
    PTR_ASSERT(text);
    memmove(
        text,
        b->text,
        b->gap_start
    );
    memmove(
        text + b->gap_start,
        b->text + b->gap_start + b->gap_size,
        b->size - b->gap_start - b->gap_size
    );
    text[b->size - b->gap_size] = 0;
    return text;
}

void buf_append_char(GapBuf *b, char ch)
{
    PTR_ASSERT(b);
    buf_move(b);
    if(!b->gap_size) { // gap is empty
        size_t after_cursor = b->size - b->gap_start;
        b->gap_size = b->size;
        b->size *= 2;
        b->text = realloc(b->text, b->size);
        PTR_ASSERT(b->text);
        memmove(
            b->text + b->gap_size + b->gap_start,
            b->text + b->gap_start,
            after_cursor
        );
    }
    b->text[b->gap_start++] = ch;
    --b->gap_size;
    if(ch == '\n') {
        b->x = 0;
        ++b->y;
    } else ++b->x;
    ++b->pos;
}

void buf_left(GapBuf *b)
{
    PTR_ASSERT(b);
    if(b->pos) {
        --b->x;
        --b->pos;
        if(b->text[b->pos + ((b->pos > b->gap_start) ? b->gap_size : 0)] == '\n') {
            --b->y;
            b->x = 0;
            while(b->pos - b->x && b->text[b->pos + ((b->pos > b->gap_start) ? b->gap_size : 0) - b->x - 1] != '\n') ++b->x;
        }
    }
}

void buf_right(GapBuf *b)
{
    PTR_ASSERT(b);
    if(b->pos < b->size - b->gap_size) {
        ++b->x;
        ++b->pos;
        if(b->text[b->pos + ((b->pos > b->gap_start) ? b->gap_size : 0) - 1] == '\n') {
            ++b->y;
            b->x = 0;
        }
    }
}

void buf_up(GapBuf *b)
{
    (void) b;
    // TODO
}

void buf_down(GapBuf *b)
{
    (void) b;
    // TODO
}

void buf_delete(GapBuf *b)
{
    PTR_ASSERT(b);
    if(b->pos != b->gap_start) buf_move(b);
    if(b->gap_start) {
        --b->x;
        --b->pos;
        if(b->text[--b->gap_start] == '\n') {
            --b->y;
            b->x = 0;
            while(b->gap_start - b->x && b->text[b->gap_start - b->x - 1] != '\n') ++b->x;
        }
        ++b->gap_size;
    }
}

void buf_move(GapBuf *b)
{
    PTR_ASSERT(b);

    if(b->pos > b->size - b->gap_size) b->pos = b->size - b->gap_size;

    if(b->pos < b->gap_start) {
        memmove(
            b->text + b->gap_start + b->gap_size - (b->gap_start - b->pos),
            b->text + b->pos,
            b->gap_start + b->pos
        );
    } else {
        memmove(
            b->text + b->gap_start,
            b->text + b->gap_start + b->gap_size,
            b->pos - b->gap_start
        );
    }
    b->gap_start = b->pos;
}

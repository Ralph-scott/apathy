#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "gap.h"

GapBuf buf_create(char *text, size_t size)
{
    PTR_ASSERT(text);
    GapBuf b = (GapBuf) {
        .text = (char *) malloc(size + 8),
        .size = size + 8,
        .gap_start = 0,
        .gap_size = 8,
        .x = 0,
        .y = 0
    };
    PTR_ASSERT(b.text);
    memmove(
        b.text + 8,
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
}

void buf_left(GapBuf *b)
{
    PTR_ASSERT(b);
    if(b->gap_start) {
        --b->gap_start;
        --b->x;
        if(b->text[b->gap_start] == '\n') {
            --b->y;
            b->x = 0;
            while(b->gap_start - b->x && b->text[b->gap_start - b->x - 1] != '\n') ++b->x;
        }
        b->text[b->gap_start + b->gap_size] = b->text[b->gap_start];
    }
}

void buf_right(GapBuf *b)
{
    PTR_ASSERT(b);
    if(b->gap_start + b->gap_size < b->size) {
        ++b->gap_start;
        ++b->x;
        if(b->text[b->gap_start + b->gap_size - 1] == '\n') {
            ++b->y;
            b->x = 0;
        }
        b->text[b->gap_start - 1] = b->text[b->gap_start + b->gap_size - 1];
    }
}

void buf_up(GapBuf *b)
{
    size_t pos = b->gap_start;
    while(b->gap_start && b->text[b->gap_start - 1] != '\n')
        buf_left(b);
    pos -= b->gap_start;
    do buf_left(b);
    while(b->gap_start && b->text[b->gap_start - 1] != '\n');
    for(size_t i = 0; i < pos && b->text[b->gap_start + b->gap_size] != '\n'; ++i) {
        buf_right(b);
    }
}

void buf_down(GapBuf *b)
{
    size_t pos = b->gap_start;
    while(b->gap_start && b->text[b->gap_start - 1] != '\n')
        buf_left(b);
    pos -= b->gap_start;
    do buf_right(b);
    while(b->gap_start + b->gap_size < b->size && b->text[b->gap_start - 1] != '\n');
    for(size_t i = 0; i < pos && b->text[b->gap_start + b->gap_size] != '\n'; ++i) {
        buf_right(b);
    }
}

void buf_delete(GapBuf *b)
{
    PTR_ASSERT(b);
    if(b->gap_start) {
        --b->x;
        if(b->text[--b->gap_start] == '\n') {
            --b->y;
            b->x = 0;
            while(b->gap_start - b->x && b->text[b->gap_start - b->x - 1] != '\n') ++b->x;
        }
        ++b->gap_size;
    }
}

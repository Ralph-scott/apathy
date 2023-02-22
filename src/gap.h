#include <stdio.h>

#ifndef GAPBUF_H_
#define GAPBUF_H_

#define PTR_ASSERT(p) if((p) == NULL) { fprintf(stderr, "NULL Pointer\n"); exit(1); }

typedef struct {
    char *text;
    size_t size;
    size_t gap_start;
    size_t gap_size;
    size_t pos;
    size_t x;
    size_t y;
} GapBuf;

GapBuf buf_create(char *text, size_t size);
void buf_free(GapBuf *b);
char *buf_to_string(GapBuf *b);
void buf_append_char(GapBuf *b, char ch);
void buf_left(GapBuf *b);
void buf_right(GapBuf *b);
void buf_up(GapBuf *b);
void buf_down(GapBuf *b);
void buf_delete(GapBuf *b);
void buf_move(GapBuf *b);

#endif // GAPBUF_H_

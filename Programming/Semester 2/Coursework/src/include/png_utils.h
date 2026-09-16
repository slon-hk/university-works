#ifndef PNG_UTILS_H
#define PNG_UTILS_H

#include <png.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int width, height;
    png_byte color_type;
    png_byte bit_depth;
    int channels;
    int interlace_type;
    int compression_type;
    int filter_type;
    png_bytep *row_pointers;
} Png;

FILE *open_file(char *filename);
int open_png(char *filename, Png *img);
int save_png(const char *filename, Png *img);
void free_png(Png *img);

void print_png_info(Png *img);

void put_pixel(Png *img, int x, int y, int color[3]);

void line(Png *img, int x1, int y1, int x2, int y2, int color[3], int thickness);
void circle(Png *img, int cx, int cy, int radius, int color[3], int thickness);

void mirror(Png *img, char axis, int x1, int y1, int x2, int y2);
void pentagram(Png *img, int cx, int cy, int radius, int color[3], int thickness);

void contrast(Png *img, float alpha, int beta);
#endif
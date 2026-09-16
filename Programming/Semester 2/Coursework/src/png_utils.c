#include "png_utils.h"
#include <math.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define DEG_TO_RAD(x) ((x)*M_PI / 180.0)
#define CLAMP(v, min, max) ((v) < (min) ? (min) : ((v) > (max) ? (max) : (v)))

#define ALPHA_OPAQUE           255
#define PENTAGRAM_VERTICES     5
#define PENTAGRAM_START_ANGLE  (-90.0)
#define PENTAGRAM_ANGLE_STEP   (360.0 / PENTAGRAM_VERTICES)

FILE *open_file(char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Ошибка: Не удалось открыть файл '%s'\n", filename);
    }
    return fp;
}

int check_png(FILE *fp)
{
    unsigned char header[PNG_SIG_SIZE];
    if (fread(header, 1, PNG_SIG_SIZE, fp) != PNG_SIG_SIZE)
        return 0;
    return !png_sig_cmp(header, 0, PNG_SIG_SIZE);
}

int open_png(char *filename, Png *image)
{
    *image = (Png){0};

    FILE *fp = open_file(filename);
    if (!fp)
        return -1;

    if (!check_png(fp)) {
        fprintf(stderr, "Ошибка: Файл '%s' не является PNG\n", filename);
        fclose(fp);
        return -1;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return -1;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return -1;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "Ошибка при чтении PNG структуры\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return -1;
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, PNG_SIG_SIZE);
    png_read_info(png_ptr, info_ptr);

    image->width = png_get_image_width(png_ptr, info_ptr);
    image->height = png_get_image_height(png_ptr, info_ptr);
    image->color_type = png_get_color_type(png_ptr, info_ptr);
    image->bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    image->interlace_type = png_get_interlace_type(png_ptr, info_ptr);
    image->compression_type = png_get_compression_type(png_ptr, info_ptr);
    image->filter_type = png_get_filter_type(png_ptr, info_ptr);

    if (image->bit_depth == 16)
        png_set_strip_16(png_ptr);
    if (image->color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_ptr);
    if (image->color_type == PNG_COLOR_TYPE_GRAY && image->bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png_ptr);
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_ptr);
    if (image->color_type == PNG_COLOR_TYPE_GRAY || image->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png_ptr);

    png_read_update_info(png_ptr, info_ptr);

    image->channels = png_get_channels(png_ptr, info_ptr);
    image->color_type = png_get_color_type(png_ptr, info_ptr);

    image->row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * image->height);
    if (!image->row_pointers) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return -1;
    }

    for (int y = 0; y < image->height; y++)
        image->row_pointers[y] = NULL;

    for (int y = 0; y < image->height; y++) {
        image->row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png_ptr, info_ptr));
        if (!image->row_pointers[y]) {
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            fclose(fp);
            free_png(image);
            return -1;
        }
    }

    png_read_image(png_ptr, image->row_pointers);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);

    return 0;
}

int save_png(const char *filename, Png *img)
{
    if (!img || !img->row_pointers)
        return 1;

    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Ошибка: не удалось открыть файл %s для записи\n", filename);
        return 1;
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return 1;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(fp);
        return 1;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "Ошибка записи PNG\n");
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return 1;
    }

    png_init_io(png_ptr, fp);
    png_set_IHDR(png_ptr, info_ptr, img->width, img->height, img->bit_depth, img->color_type,
                 img->interlace_type, img->compression_type, img->filter_type);
    png_write_info(png_ptr, info_ptr);
    png_write_image(png_ptr, img->row_pointers);
    png_write_end(png_ptr, NULL);

    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
    return 0;
}

void free_png(Png *img)
{
    if (!img || !img->row_pointers)
        return;
    for (int y = 0; y < img->height; y++) {
        free(img->row_pointers[y]);
    }
    free(img->row_pointers);
    img->row_pointers = NULL;
}

void print_png_info(Png *img)
{
    if (!img || !img->row_pointers)
        return;
    printf("Width: %d\n", img->width);
    printf("Height: %d\n", img->height);
    printf("Bit depth: %d\n", img->bit_depth);
    printf("Color type: %d\n", img->color_type);
    printf("Channels: %d\n", img->channels);
}

void put_pixel(Png *img, int x, int y, int color[3])
{
    if (x < 0 || x >= img->width || y < 0 || y >= img->height)
        return;

    png_bytep row = img->row_pointers[y];
    png_bytep pixel = &row[x * img->channels];

    pixel[0] = color[0];
    pixel[1] = color[1];
    pixel[2] = color[2];

    if (img->channels == 4) {
        pixel[3] = ALPHA_OPAQUE;
    }
}

static void draw_thick_point(Png *img, int x, int y, int color[3], int thickness)
{
    int r = thickness / 2;
    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx++) {
            put_pixel(img, x + dx, y + dy, color);
        }
    }
}

void line(Png *img, int x1, int y1, int x2, int y2, int color[3], int thickness)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        draw_thick_point(img, x1, y1, color, thickness);
        if (x1 == x2 && y1 == y2)
            break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void circle(Png *img, int cx, int cy, int radius, int color[3], int thickness)
{
    float t = thickness / 2.0f;
    int minX = cx - radius - thickness;
    int maxX = cx + radius + thickness;
    int minY = cy - radius - thickness;
    int maxY = cy + radius + thickness;

    if (minX < 0)
        minX = 0;
    if (minY < 0)
        minY = 0;
    if (maxX >= img->width)
        maxX = img->width - 1;
    if (maxY >= img->height)
        maxY = img->height - 1;

    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            float dx = (float)(x - cx);
            float dy = (float)(y - cy);
            float d = sqrtf(dx * dx + dy * dy);
            if (fabsf(d - radius) <= t) {
                put_pixel(img, x, y, color);
            }
        }
    }
}

void swap_pixels(png_bytep a, png_bytep b, int channels)
{
    png_byte tmp;
    for (int i = 0; i < channels; i++) {
        tmp = a[i];
        a[i] = b[i];
        b[i] = tmp;
    }
}

void mirror(Png *img, char axis, int x1, int y1, int x2, int y2)
{
    int channels = img->channels;
    if (x1 > x2) {
        int t = x1;
        x1 = x2;
        x2 = t;
    }
    if (y1 > y2) {
        int t = y1;
        y1 = y2;
        y2 = t;
    }
    x1 = CLAMP(x1, 0, img->width - 1);
    x2 = CLAMP(x2, 0, img->width - 1);
    y1 = CLAMP(y1, 0, img->height - 1);
    y2 = CLAMP(y2, 0, img->height - 1);

    if (axis == 'x') {
        for (int y = y1; y <= y2; y++) {
            png_bytep row = img->row_pointers[y];
            int left = x1;
            int right = x2;
            while (left < right) {
                swap_pixels(&row[left * channels], &row[right * channels], channels);
                left++;
                right--;
            }
        }
    } else if (axis == 'y') {
        int top = y1;
        int bottom = y2;
        while (top < bottom) {
            png_bytep row_top = img->row_pointers[top];
            png_bytep row_bottom = img->row_pointers[bottom];
            for (int x = x1; x <= x2; x++) {
                swap_pixels(&row_top[x * channels], &row_bottom[x * channels], channels);
            }
            top++;
            bottom--;
        }
    }
}

void pentagram(Png *img, int cx, int cy, int radius, int color[3], int thickness)
{
    circle(img, cx, cy, radius, color, thickness);
    int px[PENTAGRAM_VERTICES], py[PENTAGRAM_VERTICES];
    double start_angle = PENTAGRAM_START_ANGLE;

    for (int i = 0; i < PENTAGRAM_VERTICES; i++) {
        double angle = DEG_TO_RAD(start_angle + i * PENTAGRAM_ANGLE_STEP);
        px[i] = (int)round(cx + radius * cos(angle));
        py[i] = (int)round(cy + radius * sin(angle));
    }

    int current = 0;
    for (int i = 0; i < PENTAGRAM_VERTICES; i++) {
        int next = (current + 2) % PENTAGRAM_VERTICES;
        line(img, px[current], py[current], px[next], py[next], color, thickness);
        current = next;
    }
}
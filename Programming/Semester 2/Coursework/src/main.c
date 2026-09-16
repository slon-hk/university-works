#include "png_utils.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERR_CODE          42
#define ACTION_INFO       1
#define ACTION_LINE       2
#define ACTION_MIRROR     3
#define ACTION_PENTAGRAM  4
#define DEFAULT_THICKNESS 1
#define DEFAULT_COLOR     255
#define MAX_COLOR_VALUE   255
#define OPT_COLOR         1000
#define OPT_INPUT         1001

void print_help()
{
    printf("Course work for option 4.20, created by Leonid Stepanov.\n");
    printf("Usage: cw [OPTIONS] [FILE]\n\n");
    printf("Действия:\n");
    printf("  -i, --info                 Показать информацию о PNG\n");
    printf("  -l, --line                 Нарисовать отрезок\n");
    printf("  -m, --mirror               Отразить область\n");
    printf("  -p, --pentagram            Нарисовать пентаграмму в круге\n\n");
    printf("Параметры:\n");
    printf("  -s, --start x.y            Координаты начала\n");
    printf("  -e, --end x.y              Координаты конца\n");
    printf("  -c, --center x.y           Центр окружности\n");
    printf("  -r, --radius N             Радиус\n");
    printf("  -a, --axis x|y             Ось отражения\n");
    printf("  -u, --left_up x.y          Левый верхний угол\n");
    printf("  -d, --right_down x.y       Правый нижний угол\n");
    printf("  -t, --thickness N          Толщина линии\n");
    printf("  --color r.g.b              Цвет в формате R.G.B\n");
    printf("  -o, --output FILE          Выходной файл (по умолчанию out.png)\n");
    printf("  --input FILE               Входной файл (или последний аргумент)\n");
}

int main(int argc, char **argv)
{
    if (argc == 1) {
        print_help();
        return 0;
    }

    opterr = 0;
    char *input_file = NULL;
    char *output_file = "out.png";
    int action = 0;

    int start_x = 0, start_y = 0, end_x = 0, end_y = 0;
    int center_x = 0, center_y = 0, radius = -1;
    char axis = 0;
    int lu_x = -1, lu_y = -1, rd_x = -1, rd_y = -1;
    int color_r = -1, color_g = -1, color_b = -1;
    int thickness = -1;

    struct option long_options[] = {{"help", no_argument, NULL, 'h'},
                                    {"info", no_argument, NULL, 'i'},
                                    {"line", no_argument, NULL, 'l'},
                                    {"mirror", no_argument, NULL, 'm'},
                                    {"pentagram", no_argument, NULL, 'p'},
                                    {"start", required_argument, NULL, 's'},
                                    {"end", required_argument, NULL, 'e'},
                                    {"center", required_argument, NULL, 'c'},
                                    {"radius", required_argument, NULL, 'r'},
                                    {"axis", required_argument, NULL, 'a'},
                                    {"left_up", required_argument, NULL, 'u'},
                                    {"right_down", required_argument, NULL, 'd'},
                                    {"thickness", required_argument, NULL, 't'},
                                    {"output", required_argument, NULL, 'o'},
                                    {"color", required_argument, NULL, OPT_COLOR},
                                    {"input", required_argument, NULL, OPT_INPUT},
                                    {NULL, 0, NULL, 0}};

    int opt;
    while ((opt = getopt_long(argc, argv, "hilmps:e:c:r:a:u:d:t:o:", long_options, NULL)) != -1) {
        switch (opt) {
            case 'h':
                print_help();
                return 0;
            case 'i':
                action = ACTION_INFO;
                break;
            case 'l':
                action = ACTION_LINE;
                break;
            case 'm':
                action = ACTION_MIRROR;
                break;
            case 'p':
                action = ACTION_PENTAGRAM;
                break;
            case 'o':
                output_file = optarg;
                break;
            case OPT_INPUT:
                input_file = optarg;
                break;

            case 's':
                if (sscanf(optarg, "%d.%d", &start_x, &start_y) != 2) {
                    fprintf(stderr, "Ошибка: Неверный формат --start\n");
                    return ERR_CODE;
                }
                break;
            case 'e':
                if (sscanf(optarg, "%d.%d", &end_x, &end_y) != 2) {
                    fprintf(stderr, "Ошибка: Неверный формат --end\n");
                    return ERR_CODE;
                }
                break;
            case 'c':
                if (sscanf(optarg, "%d.%d", &center_x, &center_y) != 2) {
                    fprintf(stderr, "Ошибка: Неверный формат --center\n");
                    return ERR_CODE;
                }
                break;
            case 'u':
                if (sscanf(optarg, "%d.%d", &lu_x, &lu_y) != 2) {
                    fprintf(stderr, "Ошибка: Неверный формат --left_up\n");
                    return ERR_CODE;
                }
                break;
            case 'd':
                if (sscanf(optarg, "%d.%d", &rd_x, &rd_y) != 2) {
                    fprintf(stderr, "Ошибка: Неверный формат --right_down\n");
                    return ERR_CODE;
                }
                break;
            case 'r':
                radius = atoi(optarg);
                if (radius <= 0) {
                    fprintf(stderr, "Ошибка: Радиус должен быть > 0\n");
                    return ERR_CODE;
                }
                break;
            case 't':
                thickness = atoi(optarg);
                if (thickness <= 0) {
                    fprintf(stderr, "Ошибка: Толщина должна быть > 0\n");
                    return ERR_CODE;
                }
                break;
            case 'a':
                if (strcmp(optarg, "x") == 0)
                    axis = 'x';
                else if (strcmp(optarg, "y") == 0)
                    axis = 'y';
                else {
                    fprintf(stderr, "Ошибка: Неверная ось (используйте x или y)\n");
                    return ERR_CODE;
                }
                break;
            case OPT_COLOR:
                if (sscanf(optarg, "%d.%d.%d", &color_r, &color_g, &color_b) != 3) {
                    fprintf(stderr, "Ошибка: Неверный формат --color\n");
                    return ERR_CODE;
                }
                if (color_r < 0 || color_r > MAX_COLOR_VALUE || color_g < 0 ||
                    color_g > MAX_COLOR_VALUE || color_b < 0 || color_b > MAX_COLOR_VALUE) {
                    fprintf(stderr, "Ошибка: Значения цвета должны быть 0-255\n");
                    return ERR_CODE;
                }
                break;
            case '?':
                fprintf(stderr, "Ошибка: Неизвестная опция\n");
                return ERR_CODE;
        }
    }

    if (!input_file) {
        if (optind < argc)
            input_file = argv[optind];
        else {
            fprintf(stderr, "Ошибка: Не указан входной файл\n");
            return ERR_CODE;
        }
    }

    if (action == 0) {
        fprintf(stderr, "Ошибка: Не указано действие (например, -i, -l, -m, -p)\n");
        return ERR_CODE;
    }

    Png img;
    if (open_png(input_file, &img) != 0) {
        return ERR_CODE;
    }

    if (thickness == -1)
        thickness = DEFAULT_THICKNESS;
    if (color_r == -1) {
        color_r = DEFAULT_COLOR;
        color_g = DEFAULT_COLOR;
        color_b = DEFAULT_COLOR;
    }
    int color[3] = {color_r, color_g, color_b};

    switch (action) {
        case ACTION_INFO:
            print_png_info(&img);
            break;
        case ACTION_LINE:
            line(&img, start_x, start_y, end_x, end_y, color, thickness);
            if (save_png(output_file, &img) != 0) {
                free_png(&img);
                return ERR_CODE;
            }
            break;
        case ACTION_MIRROR:
            if (!axis || lu_x == -1 || rd_x == -1) {
                fprintf(stderr, "Ошибка: Для отражения требуются --axis, "
                                "--left_up, --right_down\n");
                free_png(&img);
                return ERR_CODE;
            }
            mirror(&img, axis, lu_x, lu_y, rd_x - 1, rd_y - 1);
            if (save_png(output_file, &img) != 0) {
                free_png(&img);
                return ERR_CODE;
            }
            break;
        case ACTION_PENTAGRAM:
            if (radius <= 0) {
                fprintf(stderr, "Ошибка: Для пентаграммы требуется указать --radius > 0\n");
                free_png(&img);
                return ERR_CODE;
            }
            pentagram(&img, center_x, center_y, radius, color, thickness);
            if (save_png(output_file, &img) != 0) {
                free_png(&img);
                return ERR_CODE;
            }
            break;
    }

    free_png(&img);
    return 0;
}
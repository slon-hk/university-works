#include <stdio.h>
#include <stdlib.h>

#include "slon_tools.h"
#include "pred_obrabotka.h"
#include "one.h"
#include "two.h"
#include "three.h"
#include "four.h"

int zero();
int one();
int two();
int three();
int four();
int five();

int main(){
    printf("Course work for option 4.11, created by Stepanov Leonid.\n");

    int option;
    if (scanf("%d", &option) != 1) {
        printf("Error: Не удалось считать номер команды\n");
        return 0;
    }

    getchar();

    int res = 0;
    switch(option){
        case 0:
            res = zero();
            break;
        case 1:
            res = one();
            break;
        case 2:
            res = two();
            break;
        case 3:
            res = three();
            break;
        case 4:
            res = four();
            break;
        case 5:
            res = five();
            break;
        default:
            printf("Error: Введите номер от 0 до 5\n");
            return 0;
    }

    if (res != 0) {
        printf("Error: Ошибка при выполнении функции или чтении данных\n");
    }

    return 0;
}

int zero(){
    char *text = read_arr();
    if (!text) return 1;
    text = pred_obrabotka(text);
    if (!text) return 1;

    printf("%s\n", text);

    free(text);
    return 0;
}

int one(){
    char *text = read_arr();
    print_sum_time(text);

    free(text);
    return 0;
}

int two(){
    char *text = read_arr();

    text = del_first_sentence(text);
    if (text) printf("%s\n", text);

    free(text);
    return 0;
}

int three(){
    char *text = read_arr();

    text = remove_equal_length_sentences(text);
    if (text) printf("%s\n", text);

    free(text);
    return 0;
}

int four(){
    char *text = read_arr();

    text = sort_by_islower(text); 
    if (text) printf("%s\n", text);

    free(text);
    return 0;
}

int five(){
    printf("0. Вывод текста после первичной обработки.\n");
    printf("1. Найти предложения с <число>sec, вывести сумму времени.\n");
    printf("2. Удалить первое слово в каждом предложении.\n");
    printf("3. Удалить предложения с одинаковой длиной слов.\n");
    printf("4. Сортировать по убыванию строчных букв (qsort).\n");
    printf("5. Справка.\n");
    return 0;
}


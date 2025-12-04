#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

void swap(int *a, int *b);
void bubbleSort(int *arr, int size);
int compare(const void* a, const void* b);
    
int main(){
    clock_t start, end;
    double bubble_time, quick_time;

    int size = 1000;
    int arr[size];
    int arr_copy[size];
    memcpy(arr_copy, arr, size*sizeof(int));
    for(int i = 0; i < size; i++){
        scanf("%d", &arr[i]);
    }

    // Bubble sort
    start = clock();
    bubbleSort(arr, size);
    end = clock();
    bubble_time = ((double) (end-start)) / CLOCKS_PER_SEC;

    //Quick sort
    start = clock();
    qsort(arr_copy, size, sizeof(int), compare);
    end = clock();
    quick_time = ((double) (end-start)) / CLOCKS_PER_SEC;

    for(int i = 0; i<size; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");
    printf("Время выполнения: %f секунд\n", bubble_time);
    printf("Время выполнения: %f секунд\n", quick_time);
    return 0;
}

void swap(int *a, int *b){
    int buffer = *a;
    *a = *b;
    *b = buffer;
}


void bubbleSort(int *arr, int size){
    for(int i = 0; i<size-1; i++){
        for(int j = 0; j<size-1; j++){
            if(arr[j]>arr[j+1]) swap(&arr[j], &arr[j+1]);
        }
    }
}

int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

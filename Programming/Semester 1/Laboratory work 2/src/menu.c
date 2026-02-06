#include <stdio.h>

#include "abs_max.h"
#include "abs_min.h"
#include "diff.h"
#include "sum.h"

int main(){
	int f; 
	int n = 0;
	int arr[100];
	scanf("%d", &f);
	if (f<0 || f>3){
		printf("Данные некорректны\n");
		return 0;
	}
	while (n<100){
		int x = scanf("%d", &arr[n]);
		if (x == 1){
			n++;
		} else{
			break;
		}
	}
	int val, in;
	max_abs(arr, n, &val, &in);
	switch(f){
		case 0:
			printf("%d\n", val);
			break;
		case 1:
			printf("%d\n", min_abs(arr, n));
			break;
		case 2:
			printf("%d\n", diff_abs(arr, n));
			break;
		case 3:
			printf("%d\n", sum_arr(arr, n));
			break;
		
		default:
			printf("Данные некорректны\n");
	}
	return 0;
}

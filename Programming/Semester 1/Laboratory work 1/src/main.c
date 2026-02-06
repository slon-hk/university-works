#include <stdio.h>
#include <stdlib.h>
int max_abs(int arr[], int s);
int min_abs(int arr[], int s);
int diff_abs(int arr[], int s);
int sum_arr(int arr[], int s);
int gl_max_index;

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
	switch(f){
		case 0:
			printf("%d\n", max_abs(arr, n));
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

int max_abs(int arr[], int s){
	int max_abs = abs(arr[0]);
	int max_el = arr[0];
	int max_index = 0;
	
	for(int i = 0; i < s; i++){
		if (abs(arr[i]) > max_abs){
			max_abs = abs(arr[i]);
			max_el = arr[i];
			max_index = i;
		}
	}
	
	gl_max_index = max_index;
	return max_el; 
}

int min_abs(int arr[], int s){
	int min_abs = abs(arr[0]);
	int min_el = arr[0];
	
	for(int i = 0; i < s; i++){
		if (abs(arr[i]) < min_abs){
			min_abs = abs(arr[i]);
			min_el = arr[i];
		}
	}

	return min_el; 
}

int diff_abs(int arr[], int s){
	return (max_abs(arr, s) - min_abs(arr, s));
}

int sum_arr(int arr[], int s){
	int sum = 0;
	max_abs(arr, s); 
	
	for (int i = gl_max_index; i < s; i++){
		sum += arr[i];
	}
	
	return sum;
}
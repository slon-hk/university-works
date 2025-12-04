#include <stdlib.h>

#include "abs_max.h"

void max_abs(int arr[], int s, int* val, int* in){
	*val = arr[0];
	*in = 0;

	for (int i = 1; i<s; i++){
		if(abs(arr[i])>abs(*val)){
			*val = arr[i];
			*in = i; 
		}
	}
}

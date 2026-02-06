#include <stdlib.h>

#include "abs_min.h"

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

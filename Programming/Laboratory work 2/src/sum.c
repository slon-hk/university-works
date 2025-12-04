#include "sum.h"
#include "abs_max.h"

int sum_arr(int arr[], int s){
	int sum = 0;
	int val, in;
	max_abs(arr, s, &val, &in);

	for (int i = in; i<s; i++){
		sum += arr[i];
	}

	return sum;
}

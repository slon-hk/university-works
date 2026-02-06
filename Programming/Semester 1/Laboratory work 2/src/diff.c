#include "diff.h"
#include "abs_max.h"
#include "abs_min.h"

int diff_abs(int arr[], int s){
	int val, in;
	max_abs(arr, s, &val, &in);
	return (val - min_abs(arr, s));
}

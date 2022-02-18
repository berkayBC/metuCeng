#include "the1.h"

//You can add your own helper functions

void sillySort_helper(int *arr, int start, int end, int size, long &comparison, long &swap, int &num_of_calls)
{
	if (size < 4)
	{
		if (size == 2) //size 2
		{
			comparison++;
			if (*(arr + start) > *(arr + end)) // swap needed
			{
				int temp = *(arr + start);
				*(arr + start) = *(arr + end);
				*(arr + end) = temp;
				swap++;
			}
			else // 2-no swap
			{
				return;
			}
		}
	}
	else
	{ // 4- call again
		num_of_calls += 6;
		sillySort_helper(arr + start, 0, size / 2 - 1, size / 2, comparison, swap, num_of_calls);
		sillySort_helper(arr + start, size / 4, 3 * size / 4 - 1, size / 2, comparison, swap, num_of_calls);
		sillySort_helper(arr + start, size / 2, size - 1, size / 2, comparison, swap, num_of_calls);
		sillySort_helper(arr + start, 0, size / 2 - 1, size / 2, comparison, swap, num_of_calls);
		sillySort_helper(arr + start, size / 4, 3 * size / 4 - 1, size / 2, comparison, swap, num_of_calls);
		sillySort_helper(arr + start, 0, size / 2 - 1, size / 2, comparison, swap, num_of_calls);
	}
}

int sillySort(int *arr, long &comparison, long &swap, int size)
{
	int num_of_calls = 1;

	//Your code here
	if (size <= 1) //size 1
	{
		return num_of_calls;
	}
	else
	{
		sillySort_helper(arr, 0, size - 1, size, comparison, swap, num_of_calls);
	}
	return num_of_calls;
}

int crossMergeSort(int *arr, long &comparison, int size)
{
	int num_of_calls = 1;
	return num_of_calls;
}
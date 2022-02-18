#include "the0.h"

void insertionSort(int* arr, long &comparison, long & swap, int size) 
{
    int i, key, j;
    comparison = 0;
    swap = 0;
    for (i = 1; i < size; i++)
    {
        key = arr[i];
        j = i - 1;
        comparison++;
        while (j >= 0 && arr[j] > key)
        {
            swap++;
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
    
}
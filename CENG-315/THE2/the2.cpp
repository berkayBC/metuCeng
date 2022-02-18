#include "the2.h"

//You may write your own helper functions here
//HELPERS

void swap_help(unsigned short *a, unsigned short *b)
{
    unsigned short temp = *a;
    *a = *b;
    *b = temp;
}

int partition(unsigned short *arr, int start, int end, long &swap, double &avg_dist, double &max_dist)
{
    int pivot = arr[end];
    int i = start - 1;

    for (int j = start; j <= end - 1; j++)
    {
        if (arr[j] >= pivot)
        {
            i++;
            swap_help(&arr[i], &arr[j]);
            swap++;
            if (j > i)
            {
                avg_dist += j - i;
                if (max_dist < j - i)
                {
                    max_dist = j - i;
                }
            }
            else
            {
                avg_dist += i - j;
                if (max_dist < i - j)
                {
                    max_dist = i - j;
                }
            }
        }
    }
    swap_help(&arr[i + 1], &arr[end]);
    swap++;
    if (end > i + 1)
    {
        avg_dist += end - (i + 1);
        if (max_dist < end - (i + 1))
        {
            max_dist = end - (i + 1);
        }
    }
    else
    {
        avg_dist += (i + 1) - end;
        if (max_dist < (i + 1) - end)
        {
            max_dist = (i + 1) - end;
        }
    }
    return (i + 1);
}

void quickSort1_helper(unsigned short *arr, long &swap, double &avg_dist, double &max_dist, int start, int end)
{
    if (end - start >= 1)
    {
        int p = partition(arr, start, end, swap, avg_dist, max_dist);
        quickSort1_helper(arr, swap, avg_dist, max_dist, start, p - 1);
        quickSort1_helper(arr, swap, avg_dist, max_dist, p + 1, end);
    }
}

int hoare_partition(unsigned short *arr, long &swap, double &avg_dist, double &max_dist, int size)
{
    unsigned short pivot = arr[(size - 1) / 2];
    int i = -1;
    int j = size;

    while (true)
    {
        do
        {
            i++;
        } while (
            arr[i] > pivot);
        do
        {
            j--;
        } while (
            arr[j] < pivot);

        if (i < j)
        {
            swap_help(&arr[i], &arr[j]);
            swap++;
            avg_dist += (j - i);
            if (max_dist < j - i)
            {
                max_dist = j - i;
            }
        }
        else
            return j;
    }
}

void quickSortHoare_helper(unsigned short *arr, long &swap, double &avg_dist, double &max_dist, int size)
{
    if (size > 1)
    {
        int p = hoare_partition(arr, swap, avg_dist, max_dist, size);
        quickSortHoare_helper(arr, swap, avg_dist, max_dist, p + 1);
        quickSortHoare_helper(arr + p + 1, swap, avg_dist, max_dist, size - (p + 1));
    }
}

void quickSort3_partition(unsigned short *arr, long &swap, long &comparison, int size, int &l, int &r)
{
    int i = 0;
    int j = 0;
    int p = size - 1;
    while (i < p)
    {
        comparison++;
        if (arr[i] > arr[size - 1])
        {
            swap_help(&arr[i], &arr[j]);
            swap++;
            i += 1;
            j += 1;
        }
        else if (arr[i] == arr[size - 1])
        {
            comparison++;
            p -= 1;
            swap_help(&arr[i], &arr[p]);
            swap++;
        }
        else
        {
            comparison++;
            i += 1;
        }
    }
    int m = (p - j < size - p) ? p - j : size - p;
    for (int k = 0; k < m; k++)
    {
        swap_help(&arr[j + k], &arr[size - m + k]);
        swap++;
    }
    l = j;
    r = p - j;
}

void quickSort3_helper(unsigned short *arr, long &swap, long &comparison, int size)
{
    if (size > 1)
    {
        int l;
        int r;

        quickSort3_partition(arr, swap, comparison, size, l, r);

        quickSort3_helper(arr, swap, comparison, l);
        quickSort3_helper(arr + size - r, swap, comparison, r);
    }
}
// MAIN FUNCTIONS

void quickSort(unsigned short *arr, long &swap, double &avg_dist, double &max_dist, bool hoare, int size)
{
    if (hoare == false)
    {
        quickSort1_helper(arr, swap, avg_dist, max_dist, 0, size - 1);
    }
    else
    {
        quickSortHoare_helper(arr, swap, avg_dist, max_dist, size);
    }
    if (swap != 0)
        avg_dist /= swap;
}

void quickSort3(unsigned short *arr, long &swap, long &comparison, int size)
{
    quickSort3_helper(arr, swap, comparison, size);
}

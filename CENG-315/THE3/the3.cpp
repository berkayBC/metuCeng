#include "the3.h"

// do not add extra libraries here

/*
    arr       : array to be sorted, in order to get points this array should contain be in sorted state before returning
    ascending : true for ascending, false for descending 
    n         : number of elements in the array
    l         : the number of characters used in counting sort at each time
    
    you can use ceil function from cmath
    
*/
int FindIndex(std::string k, int s, int l)
{

    if (l == 1)
    {
        return (int)k[s] - 65;
    }
    else
    {
        int final_index = 26;
        int begin = s * l;
        int end = s * l + l;
        std::string sub_string = k.substr(begin, end - 1);
        for (int i = 0; i < l; i++)
        {
            final_index += ((int)sub_string[i] - 65);
        }
        return final_index;
    }
}
int CountingSort(std::string arr[], bool ascending, int n, int s, int l, int k)
{
    int result = 0;
    std::string *B = new std::string[n];
    int *C = new int[k];

    for (int i = 0; i < k; i++)
    {
        C[i] = 0;
        result++;
    }

    for (int j = 0; j < n; j++)
    {
        int index = FindIndex(arr[j], s, l);
        C[index] = C[index] + 1;
        result++;
    }

    for (int index = 1; index < k; index++)
    {
        C[index] = C[index] + C[index - 1];

        result++;
    }
    
    if (ascending)
    {
        for (int i = n - 1; i >= 0; i--)
        {
            int index = FindIndex(arr[i], s, l);

            if (C[index] > 0)
            {
                B[C[index] - 1] = arr[i];
                C[index]--;
            }
            arr[i] = B[i];
            result++;
        }
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            int index = FindIndex(arr[i], s, l);

            if (C[index] > 0)
            {
                B[n - C[index]] = arr[i];
                C[index]--;
            }
            arr[i] = B[i];
            result++;
        }
    }
    
    delete[] B;

    return result;
}

int radixSort(std::string arr[], bool ascending, int n, int l)
{

    int result = 0;
    int k = pow(26, l);
    int loop_count = n / l;
    for (int s = 0; s < loop_count; s += 1)
    {
        result += CountingSort(arr, ascending, n, s, l, k);
        if ((loop_count < s + 1) && (n % l > 0))
        {
            loop_count++;
            l = n % l;
        }
    }
    return result;
}
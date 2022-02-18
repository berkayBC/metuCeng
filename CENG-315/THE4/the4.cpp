#include "the4.h"

int recursive_sln(int i, int *&arr, int &number_of_calls)
{ // direct recursive
    number_of_calls += 1;

    // your code here
    int x, y;
    if (i == 0)
    {
        return arr[0];
    }
    if (i == 1)
    {
        int max = (arr[1] > arr[0] ? arr[1] : arr[0]);
        return max;
    }
    if (i == 2)
    {
        int max1 = (arr[1] > arr[0] ? arr[1] : arr[0]);
        int max2 = (arr[2] > max1 ? arr[2] : max1);
        return max2;
    }
    x = recursive_sln(i - 3, arr, number_of_calls) + arr[i];
    y = recursive_sln(i - 1, arr, number_of_calls);

    return (x > y ? x : y); // this is a dummy return value. YOU SHOULD CHANGE THIS!
}

int memoization_sln(int i, int *&arr, int *&mem)
{ // memoization

    // your code here
    if (i == 0)
    {
        mem[0] = (arr[0] > 0 ? arr[0] : 0);
    }
    else if (i == 1)
    {
        mem[0] = (arr[0] > 0 ? arr[0] : 0);
        mem[1] = (arr[1] > mem[0] ? arr[1] : mem[0]);
    }
    else if (i >= 2)
    {
        mem[0] = (arr[0] > 0 ? arr[0] : 0);
        mem[1] = (arr[1] > mem[0] ? arr[1] : mem[0]);

        int max1, max2, max3;
        max1 = arr[0] > 0 ? arr[0] : 0;
        max2 = arr[1] > max1 ? arr[1] : max1;
        max3 = arr[2] > max2 ? arr[2] : max2;
        mem[2] = max3;

        int k = 3;
        while (k < i + 1)
        {
            mem[k] = (mem[k - 1] > (arr[k] + mem[k - 3]) ? mem[k - 1] : (arr[k] + mem[k - 3]));
            k++;
        }
    }
    return mem[i]; // this is a dummy return value. YOU SHOULD CHANGE THIS!
}

int dp_sln(int size, int *&arr, int *&mem)
{ // dynamic programming

    // your code here
    if (size == 1)
    {
        mem[0] = (arr[0] > 0 ? arr[0] : 0);
    }
    else if (size == 2)
    {
        mem[0] = (arr[0] > 0 ? arr[0] : 0);
        mem[1] = (arr[1] > mem[0] ? arr[1] : mem[0]);
    }
    else if (size >= 3)
    {
        mem[0] = (arr[0] > 0 ? arr[0] : 0);
        mem[1] = (arr[1] > mem[0] ? arr[1] : mem[0]);

        int max1, max2, max3;
        max1 = arr[0] > 0 ? arr[0] : 0;
        max2 = arr[1] > max1 ? arr[1] : max1;
        max3 = arr[2] > max2 ? arr[2] : max2;
        mem[2] = max3;

        int i = 3;
        while (i < size)
        {
            mem[i] = (mem[i - 1] > (arr[i] + mem[i - 3]) ? mem[i - 1] : (arr[i] + mem[i - 3]));
            i++;
        }
    }
    return mem[size - 1]; // this is a dummy return value. YOU SHOULD CHANGE THIS!
}

#include "the5.h"
// do not add extra libraries here

/*
W: the width of the field
L: the length of the field
partitions: 2D Boolean array for partitions, if a partition of w x l exists then partitions[x][y] is true.
numberOfCalls/numberOfIterations: method specific control parameter
*/

int recursiveMethod(int W, int L, bool **partitions, int *numberOfCalls)
{
    *numberOfCalls += 1;
    int wasted_area = 0;

    if (partitions[W][L] == true)
    {
        return wasted_area;
    }
    else
    {
        wasted_area = W * L;
        int wasted_area2 = W * L;
        int wasted_area3 = W * L;
        for (int M = 1; M <= W / 2; M++)
        {
            int min_check = recursiveMethod(W - M, L, partitions, numberOfCalls) + recursiveMethod(M, L, partitions, numberOfCalls);
            wasted_area2 = min_check < wasted_area2 ? min_check : wasted_area2;
        }
        for (int N = 1; N <= L / 2; N++)
        {
            int min_check = recursiveMethod(W, L - N, partitions, numberOfCalls) + recursiveMethod(W, N, partitions, numberOfCalls);
            wasted_area3 = min_check < wasted_area3 ? min_check : wasted_area3;
        }
        return (wasted_area < (wasted_area2 < wasted_area3 ? wasted_area2 : wasted_area3) ? wasted_area : (wasted_area2 < wasted_area3 ? wasted_area2 : wasted_area3));
    }
}

int memoizationMethodHelper(int W, int L, bool **partitions, int *numberOfCalls, int **memoization)
{
    *numberOfCalls += 1;

    if (memoization[W][L] != -10)
        return memoization[W][L];
    else
    {
        if (partitions[W][L])
        {
            memoization[W][L] = 0;
            return memoization[W][L];
        }
        else
        {
            int wasted_area = W * L;
            int wasted_area2 = W * L;
            int wasted_area3 = W * L;
            for (int M = 1; M <= W / 2; M++)
            {
                int min_check = memoizationMethodHelper(W - M, L, partitions, numberOfCalls, memoization) + memoizationMethodHelper(M, L, partitions, numberOfCalls, memoization);
                wasted_area2 = min_check < wasted_area2 ? min_check : wasted_area2;
            }
            for (int N = 1; N <= L / 2; N++)
            {
                int min_check = memoizationMethodHelper(W, L - N, partitions, numberOfCalls, memoization) + memoizationMethodHelper(W, N, partitions, numberOfCalls, memoization);
                wasted_area3 = min_check < wasted_area3 ? min_check : wasted_area3;
            }
            memoization[W][L] = (wasted_area < (wasted_area2 < wasted_area3 ? wasted_area2 : wasted_area3) ? wasted_area : (wasted_area2 < wasted_area3 ? wasted_area2 : wasted_area3));
        }
        return memoization[W][L];
    }
}

int memoizationMethod(int W, int L, bool **partitions, int *numberOfCalls)
{
    *numberOfCalls += 1;
    int **memoization = new int *[W + 1];
    for (int index_1 = 0; index_1 < W + 1; index_1++)
    {
        memoization[index_1] = new int[L + 1];
        for (int index_2 = 0; index_2 < L + 1; index_2++)
        {
            memoization[index_1][index_2] = -10;
        }
    }

    return memoizationMethodHelper(W, L, partitions, numberOfCalls, memoization);
}

int bottomUpMethod(int W, int L, bool **partitions, int *numberOfIterations)
{
    int dummy;
    return dummy;
}

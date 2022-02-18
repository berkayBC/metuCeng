#include "the8.h"

// DO NOT ADD OTHER LIBRARIES

using namespace std;

void search(string text, const vector<string> &patterns, vector<int> &shifts)
{
    long M = patterns[0].length();
    long N = text.length();
    int i, j;
    // long p = 0; // hash value for pattern
    long t = 0; // hash value for text
    long h = 1;

    long q = 1000003; // prime number greater than 1000*1000
    int d = 26;
    long patterns_size = patterns.size();
    long p[patterns_size] = {};

    // The value of h would be "pow(d, M-1)%q"
    for (i = 0; i < M - 1; i++)
    {
        h = (h * d) % q;
    }

    // Calculate the hash value of pattern and first
    // window of text
    for (i = 0; i < M; i++)
    {
        for (j = 0; j < patterns_size; j++)
        {
            p[j] = (d * p[j] + patterns[j][i]) % q;
        }
        t = (d * t + text[i]) % q;
    }
    int k;
    // Slide the pattern over text one by one
    for (i = 0; i <= N - M; i++)
    {
        for (k = 0; k < patterns_size; k++)
        {
            if (p[k] == t)
            {
                // Check for characters one by one
                for (j = 0; j < M; j++)
                {
                    if (text[i + j] != patterns[k][j])
                    {
                        break;
                    }
                }

                // if p == t and pat[0...M-1] = text[i, i+1, ...i+M-1]
                if (j == M)
                {
                    shifts.push_back(i);
                }
            }

            // Calculate hash value for next window of text: Remove
            // leading digit, add trailing digit
        }
        if (i < N - M)
        {
            t = (d * (t - text[i] * h) + text[i + M]) % q;

            // We might get negative value of t, converting it
            // to positive
            if (t < 0)
            {
                t = (t + q);
            }
        }
    }
}

vector<int> the8(const string &text, const vector<string> &patterns)
{
    vector<int> shifts; // DO NOT CHANGE THIS

    search(text, patterns, shifts);

    return shifts; // DO NOT CHANGE THIS
}

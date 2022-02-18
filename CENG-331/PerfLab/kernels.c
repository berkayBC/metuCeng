/********************************************************
 * Kernels to be optimized for the Metu Ceng Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "blackLivesMatter", /* Team name */

    "Anil Berdan Ceylan", /* First member full name */
    "2304277",     /* First member id */

    "Berkay Bartug Cetin", /* Second member full name (leave blank if none) */
    "2309839", /* Second member id (leave blank if none) */

    "Fahri Numan Eskici", /* Third member full name (leave blank if none) */
    "2309995"  /* Third member id (leave blank if none) */
};

/****************
 * EXPOSURE FUSION KERNEL *
 ****************/

/*******************************************************
 * Your different versions of the exposure fusion kernel go here 
 *******************************************************/

/* 
 * naive_fusion - The naive baseline version of exposure fusion
 */
char naive_fusion_descr[] = "naive_fusion: Naive baseline exposure fusion";
void naive_fusion(int dim, int *img, int *w, int *dst)
{

    int i, j, k;
    for (k = 0; k < 4; k++)
    {
        for (i = 0; i < dim; i++)
        {
            for (j = 0; j < dim; j++)
            {
                dst[i * dim + j] += w[k * dim * dim + i * dim + j] * img[k * dim * dim + i * dim + j];
            }
        }
    }
}

// my_fusion(dim, img, w, dst);
/* 
 * fusion - Your current working version of fusion
 * IMPORTANT: This is the version you will be graded on
 */
char fusion_descr[] = "fusion: Current working version";
void fusion(int dim, int *img, int *w, int *dst)
{
    int i, k;
    int dimS = dim * dim;
    int kdimS = 0;

    for (k = 0; k < 4; k++)
    {
        for (i = 0; i < dimS; i += 32)
        {
            dst[i + 0] += w[kdimS + i + 0] * img[kdimS + i + 0];
            dst[i + 1] += w[kdimS + i + 1] * img[kdimS + i + 1];
            dst[i + 2] += w[kdimS + i + 2] * img[kdimS + i + 2];
            dst[i + 3] += w[kdimS + i + 3] * img[kdimS + i + 3];
            dst[i + 4] += w[kdimS + i + 4] * img[kdimS + i + 4];
            dst[i + 5] += w[kdimS + i + 5] * img[kdimS + i + 5];
            dst[i + 6] += w[kdimS + i + 6] * img[kdimS + i + 6];
            dst[i + 7] += w[kdimS + i + 7] * img[kdimS + i + 7];
            dst[i + 8] += w[kdimS + i + 8] * img[kdimS + i + 8];
            dst[i + 9] += w[kdimS + i + 9] * img[kdimS + i + 9];
            dst[i + 10] += w[kdimS + i + 10] * img[kdimS + i + 10];
            dst[i + 11] += w[kdimS + i + 11] * img[kdimS + i + 11];
            dst[i + 12] += w[kdimS + i + 12] * img[kdimS + i + 12];
            dst[i + 13] += w[kdimS + i + 13] * img[kdimS + i + 13];
            dst[i + 14] += w[kdimS + i + 14] * img[kdimS + i + 14];
            dst[i + 15] += w[kdimS + i + 15] * img[kdimS + i + 15];
            dst[i + 16] += w[kdimS + i + 16] * img[kdimS + i + 16];
            dst[i + 17] += w[kdimS + i + 17] * img[kdimS + i + 17];
            dst[i + 18] += w[kdimS + i + 18] * img[kdimS + i + 18];
            dst[i + 19] += w[kdimS + i + 19] * img[kdimS + i + 19];
            dst[i + 20] += w[kdimS + i + 20] * img[kdimS + i + 20];
            dst[i + 21] += w[kdimS + i + 21] * img[kdimS + i + 21];
            dst[i + 22] += w[kdimS + i + 22] * img[kdimS + i + 22];
            dst[i + 23] += w[kdimS + i + 23] * img[kdimS + i + 23];
            dst[i + 24] += w[kdimS + i + 24] * img[kdimS + i + 24];
            dst[i + 25] += w[kdimS + i + 25] * img[kdimS + i + 25];
            dst[i + 26] += w[kdimS + i + 26] * img[kdimS + i + 26];
            dst[i + 27] += w[kdimS + i + 27] * img[kdimS + i + 27];
            dst[i + 28] += w[kdimS + i + 28] * img[kdimS + i + 28];
            dst[i + 29] += w[kdimS + i + 29] * img[kdimS + i + 29];
            dst[i + 30] += w[kdimS + i + 30] * img[kdimS + i + 30];
            dst[i + 31] += w[kdimS + i + 31] * img[kdimS + i + 31];
        }
        kdimS += dimS;
    }
}


/*********************************************************************
 * register_fusion_functions - Register all of your different versions
 *     of the fusion kernel with the driver by calling the
 *     add_fusion_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_fusion_functions()
{
    add_fusion_function(&fusion, fusion_descr);
    /* ... Register additional test functions here */
}

/***************************
 * GAUSSIAN BLUR KERNEL *
 ***************************/

/******************************************************
 * Your different versions of the Gaussian blur functions go here
 ******************************************************/

/* 
 * naive_blur - The naive baseline version of Gussian blur
 */
char naive_blur_descr[] = "naive_blur The naive baseline version of Gaussian blur";
void naive_blur(int dim, float *img, float *flt, float *dst)
{

    int i, j, k, l;

    for (i = 0; i < dim - 5 + 1; i++)
    {
        for (j = 0; j < dim - 5 + 1; j++)
        {
            for (k = 0; k < 5; k++)
            {
                for (l = 0; l < 5; l++)
                {
                    dst[i * dim + j] +=  img[(i + k) * dim + j + l] * flt[k * dim + l];
                }
            }
        }
    }
}

/* 
 * blur - Your current working version of Gaussian blur
 * IMPORTANT: This is the version you will be graded on
 */
char blur_descr[] = "blur: Current working version";
void blur(int dim, float *img, float *flt, float *dst)
{
    int i,j,initIndex = dim - 4;
    float fltValues[25];
    float res;
    float *tmpImg, *rwDst, *rwImg;

    j = 0;
    for (i = 0; i < 25; i++)
    {
        fltValues[i] = flt[j++];
        if(j == 5){
            j = 0;
            flt += dim;
        }
    }

    float *fltVal = fltValues;

    for(i = initIndex; i > 0; i--) 
    {
        tmpImg = img; 
        rwDst = dst;
        for(j = initIndex; j > 0; j--) 
        {
            rwImg = tmpImg;
            res = 0;

            res += (*(rwImg++) * *(fltVal++)) + (*(rwImg++) * *(fltVal++)) + (*(rwImg++) * *(fltVal++)) + (*(rwImg++) * *(fltVal++)) + (*(rwImg) * *(fltVal++));
            rwImg += initIndex;
            res += (*(rwImg++) * *(fltVal++)) + (*(rwImg++) * *(fltVal++)) + (*(rwImg++) * *(fltVal++)) + (*(rwImg++) * *(fltVal++)) + (*(rwImg) * *(fltVal++));
            rwImg += initIndex;
            res += (*(rwImg++) * *(fltVal++)) + (*(rwImg++) * *(fltVal++)) + (*(rwImg++) * *(fltVal++)) + (*(rwImg++) * *(fltVal++)) + (*(rwImg) * *(fltVal++));
            rwImg += initIndex;
            res += (*(rwImg++) * *(fltVal++)) + (*(rwImg++) * *(fltVal++)) + (*(rwImg++) * *(fltVal++)) + (*(rwImg++) * *(fltVal++)) + (*(rwImg) * *(fltVal++));
            rwImg += initIndex;
            res += (*(rwImg++) * *(fltVal++)) + (*(rwImg++) * *(fltVal++)) + (*(rwImg++) * *(fltVal++)) + (*(rwImg++) * *(fltVal++)) + (*(rwImg) * *(fltVal++));
            
            fltVal = fltValues;
            tmpImg++;
            *rwDst++ += res;
        }
        img += dim;
        dst += dim;
    }
}


/*********************************************************************
 * register_blur_functions - Register all of your different versions
 *     of the gaussian blur kernel with the driver by calling the
 *     add_blur_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_blur_functions()
{
    add_blur_function(&blur, blur_descr);
    /* ... Register additional test functions here */
}


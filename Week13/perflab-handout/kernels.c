/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/*
 * Please fill in the following team struct
 */
team_t team = {
    "",              /* name */

    "2019011449",     /* student ID */
    "",  /* student email */

    "",                   /* Second member full name (leave blank if none) */
    ""                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/*
 * naive_rotate - The naive baseline version of rotate
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst)
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/*
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst) {
    int h_dim, v_dim, j;
    int h_end = dim >> 5;
    int index_src, index_dst;

    index_src = dim - 1;
    index_dst = 0;

    for (h_dim = 0; h_dim < h_end; h_dim++) {
        for (v_dim = 0; v_dim < dim; v_dim++) {
            for (j = 0; j < 32; j++) {
                dst[index_dst++] = src[index_src];
                index_src += dim;
            }
            index_src -= dim * 32 + 1;
            index_dst += dim - 32;
        }
        index_src += dim * (32 + 1);
        index_dst -= dim * dim - 32;
    }
}

char rotate_1[] = "no macro";
void rotate_func_1 (int dim, pixel *src, pixel *dst) {
    int i, j;

    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            dst[(dim - 1 - j) * dim + i] = src[i * dim + j];
}

char rotate_2[] = "blocking 16";
void rotate_func_2 (int dim, pixel *src, pixel *dst) {
    int i, j;
    int h_dim, v_dim;

    for (h_dim = 0; h_dim < dim; h_dim += 16)
        for (v_dim = 0; v_dim < dim; v_dim += 16)
            for (i = h_dim; (i < dim) && (i < h_dim + 16); i++)
                for (j = v_dim; (j < dim) && (j < v_dim + 16); j++)
                    dst[RIDX(dim - 1 - j, i, dim)] = src[RIDX(i, j, dim)];
}

char rotate_3[] = "blocking 32";
void rotate_func_3 (int dim, pixel *src, pixel *dst) {
    int i, j;
    int h_dim, v_dim;

    for (h_dim = 0; h_dim < dim; h_dim += 32)
        for (v_dim = 0; v_dim < dim; v_dim += 32)
            for (i = h_dim; (i < dim) && (i < h_dim + 32); i++)
                for (j = v_dim; (j < dim) && (j < v_dim + 32); j++)
                    dst[RIDX(dim - 1 - j, i, dim)] = src[RIDX(i, j, dim)];
}


char rotate_4[] = "blocking 16 + unrolling 16";
void rotate_func_4 (int dim, pixel *src, pixel *dst) {
    int i, j;
    int h_dim, v_dim;
    int index_dst, index_src;
    for (h_dim = 0; h_dim < dim; h_dim += 16)
        for (v_dim = 0; v_dim < dim; v_dim += 16) {
            for (i = h_dim; (i < dim) && (i < h_dim + 16); i++) {

                index_dst = RIDX(dim - 1 - v_dim, i, dim);
                index_src = RIDX(i, v_dim, dim);

                for (j = 0; j < 16; j++) {
                    dst[index_dst] = src[index_src++];
                    index_dst -= dim;
                }
            }
        }
}

char rotate_5[] = "unrolling 16 + vertical increasing";
void rotate_func_5 (int dim, pixel *src, pixel *dst) {
    int j;
    int h_dim, v_dim;
    int index_dst, index_src;
    int unrolling;
    int h_end;

    unrolling = 16;
    h_end = dim >> 4;

    index_src = 0;
    index_dst = (dim - 1) * dim;

    for (h_dim = 0; h_dim < h_end; h_dim++) {
        for (v_dim = 0; v_dim < dim; v_dim++) {

            for (j = 0; j < unrolling; j++) {
                dst[index_dst] = src[index_src++];
                index_dst -= dim;
            }
            index_src += dim - unrolling;
            index_dst += dim * unrolling + 1;
        }
        index_src += unrolling - dim * dim;
        index_dst -= dim * unrolling + dim;
    }
}

char rotate_6[] = "unrolling 32 + vertical increasing";
void rotate_func_6 (int dim, pixel *src, pixel *dst) {
    int j;
    int h_dim, v_dim;
    int index_dst, index_src;
    int unrolling;
    int h_end;

    unrolling = 32;
    h_end = dim >> 5;

    index_src = 0;
    index_dst = (dim - 1) * dim;

    for (h_dim = 0; h_dim < h_end; h_dim++) {
        for (v_dim = 0; v_dim < dim; v_dim++) {
            for (j = 0; j < unrolling; j++) {
                dst[index_dst] = src[index_src++];
                index_dst -= dim;
            }
            index_src += dim - unrolling;
            index_dst += dim * unrolling + 1;
        }
        index_src -= dim * dim - unrolling;
        index_dst -= dim * unrolling + dim;
    }
}

char rotate_7[] = "rotate from pi/2 to 0 + unrolling 16";
void rotate_func_7 (int dim, pixel *src, pixel *dst) {
    int h_dim, v_dim, j;
    int unrolling = 16;
    int h_end = dim >> 4;
    int index_src, index_dst;

    index_src = dim - 1;
    index_dst = 0;

    for (h_dim = 0; h_dim < h_end; h_dim++) {
        for (v_dim = 0; v_dim < dim; v_dim++) {
            for (j = 0; j < unrolling; j++) {
                dst[index_dst++] = src[index_src];
                index_src += dim;
            }
            index_src -= dim * unrolling + 1;
            index_dst += dim - unrolling;
        }
        index_src += dim * (unrolling + 1);
        index_dst -= dim * dim - unrolling;
    }
}

char rotate_8[] = "rotate form pi/2 to 0 + unrolling 32";
void rotate_func_8 (int dim, pixel *src, pixel *dst) {
    int h_dim, v_dim, j;
    int unrolling = 32;
    int h_end = dim >> 5;
    int index_src, index_dst;

    index_src = dim - 1;
    index_dst = 0;

    for (h_dim = 0; h_dim < h_end; h_dim++) {
        for (v_dim = 0; v_dim < dim; v_dim++) {
            for (j = 0; j < unrolling; j++) {
                dst[index_dst++] = src[index_src];
                index_src += dim;
            }
            index_src -= dim * unrolling + 1;
            index_dst += dim - unrolling;
        }
        index_src += dim * (unrolling + 1);
        index_dst -= dim * dim - unrolling;
    }
}

char rotate_9[] = "(2) rotate form pi/2 to 0 + unrolling 32";
void rotate_func_9 (int dim, pixel *src, pixel *dst) {
    int h_dim, v_dim, j;
    int h_end = dim >> 5;
    int index_src, index_dst;

    index_src = dim - 1;
    index_dst = 0;

    for (h_dim = 0; h_dim < h_end; h_dim++) {
        for (v_dim = 0; v_dim < dim; v_dim++) {
            for (j = 0; j < 32; j++) {
                dst[index_dst++] = src[index_src];
                index_src += dim;
            }
            index_src -= dim * 32 + 1;
            index_dst += dim - 32;
        }
        index_src += dim * (32 + 1);
        index_dst -= dim * dim - 32;
    }
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_rotate_functions()
{
#if 1 // TODO change 1 to 0
    // add_rotate_function(&naive_rotate, naive_rotate_descr);
    add_rotate_function(&rotate, rotate_descr);
    /*
    add_rotate_function(&rotate_func_1, rotate_1);
    add_rotate_function(&rotate_func_2, rotate_2);
    add_rotate_function(&rotate_func_3, rotate_3);
    add_rotate_function(&rotate_func_4, rotate_4);
    add_rotate_function(&rotate_func_5, rotate_5);
    add_rotate_function(&rotate_func_6, rotate_6);
    add_rotate_function(&rotate_func_7, rotate_7);
    add_rotate_function(&rotate_func_8, rotate_8);
    add_rotate_function(&rotate_func_9, rotate_9);*/
#endif
    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/*
 * initialize_pixel_sum - Initializes all fields of sum to 0
 */
static void initialize_pixel_sum(pixel_sum *sum)
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/*
 * accumulate_sum - Accumulates field values of p in corresponding
 * fields of sum
 */

static void accumulate_sum(pixel_sum *sum, pixel p)
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/*
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum)
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/*
 * avg - Returns averaged pixel value at (i,j)
 */
static pixel avg(int dim, int i, int j, pixel *src) {
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for (ii = max(i - 1, 0); ii <= min(i + 1, dim - 1); ii++)
        for (jj = max(j - 1, 0); jj <= min(j + 1, dim - 1); jj++)
            accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

#define MIN(A, B) (((A) < (B)) ? (A) : (B))
#define MAX(A, B) (((A) > (B)) ? (A) : (B))

pixel avg_with_macro (int dim, int i, int j, pixel *src)
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = (MAX(i-1, 0)); ii <= (MIN(i+1, dim-1)); ii++)
        for(jj = (MAX(j-1, 0)); jj <= (MIN(j+1, dim-1)); jj++)
            accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

pixel avg_no_procedure (int dim, int i, int j, pixel *src) {

    int ii, jj;
    int max_i, min_i, max_j, min_j;
    pixel_sum sum;
    pixel current_pixel;


    sum.red = sum.green = sum.blue = 0;
    sum.num = 0;


    max_i = (((i - 1) > 0) ? (i - 1) : 0);
    min_i = (((i + 1) < (dim - 1)) ? (i + 1) : (dim - 1));
    max_j = (((j - 1) > 0) ? (j - 1) : 0);
    min_j = (((j + 1) < (dim - 1)) ? (j + 1) : (dim - 1));

    for(ii = max_i; ii <= min_i; ii++)
        for(jj = max_j; jj <= min_j; jj++)
            accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}
/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) {
    int i, j;

    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth.
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) {
    int i, j;

    // I. row 0
    j = 0;
    // I-1. col 1
    i = 0;
    dst[j * dim + i].red = (src[j * dim + i].red + src[j * dim + i + 1].red + src[j * dim + i + dim].red + src[j * dim + i + dim + 1].red) / 4;
    dst[j * dim + i].green = (src[j * dim + i].green + src[j * dim + i + 1].green + src[j * dim + i + dim].green + src[j * dim + i + dim + 1].green) / 4;
    dst[j * dim + i].blue = (src[j * dim + i].blue + src[j * dim + i + 1].blue + src[j * dim + i + dim].blue + src[j * dim + i + dim + 1].blue) / 4;
    // I-2. col 1 - dim-2
    for (i = 1; i < dim - 1; i++) {
        dst[j * dim + i].red = (src[j * dim + i - 1].red + src[j * dim + i].red + src[j * dim + i + 1].red +
                                src[j * dim + i + dim - 1].red + src[j * dim + i + dim].red + src[j * dim + i + dim + 1].red) / 6;
        dst[j * dim + i].green = (src[j * dim + i - 1].green + src[j * dim + i].green + src[j * dim + i + 1].green +
                                  src[j * dim + i + dim - 1].green + src[j * dim + i + dim].green + src[j * dim + i + dim + 1].green) / 6;
        dst[j * dim + i].blue = (src[j * dim + i - 1].blue + src[j * dim + i].blue + src[j * dim + i + 1].blue +
                                 src[j * dim + i + dim - 1].blue + src[j * dim + i + dim].blue + src[j * dim + i + dim + 1].blue) / 6;
    }
    // I-3. col dim-1
    dst[j * dim + i].red = (src[j * dim + i - 1].red + src[j * dim + i].red + src[j * dim + i + dim - 1].red + src[j * dim + i + dim].red) / 4;
    dst[j * dim + i].green = (src[j * dim + i - 1].green + src[j * dim + i].green + src[j * dim + i + dim - 1].green + src[j * dim + i + dim].green) / 4;
    dst[j * dim + i].blue = (src[j * dim + i - 1].blue + src[j * dim + i].blue + src[j * dim + i + dim - 1].blue + src[j * dim + i + dim].blue) / 4;

    //II. row 1 - dim-2
    for (j = 1; j < dim - 1; j++) {
        // II-1. col 1
        i = 0;
        dst[j * dim + i].red = (src[j * dim + i - dim].red + src[j * dim + i - dim + 1].red +
                                src[j * dim + i].red + src[j * dim + i + 1].red +
                                src[j * dim + i + dim].red + src[j * dim + i + dim + 1].red) / 6;
        dst[j * dim + i].green = (src[j * dim + i - dim].green + src[j * dim + i - dim + 1].green +
                                  src[j * dim + i].green + src[j * dim + i + 1].green +
                                  src[j * dim + i + dim].green + src[j * dim + i + dim + 1].green) / 6;
        dst[j * dim + i].blue = (src[j * dim + i - dim].blue + src[j * dim + i - dim + 1].blue +
                                 src[j * dim + i].blue + src[j * dim + i + 1].blue +
                                 src[j * dim + i + dim].blue + src[j * dim + i + dim + 1].blue) / 6;
        // II-2. col 1 - dim-2
        for (i = 1; i < dim - 1; i++) {
            dst[j * dim + i].red = (src[j * dim + i - dim - 1].red + src[j * dim + i - dim].red + src[j * dim + i - dim + 1].red +
                                    src[j * dim + i - 1].red + src[j * dim + i].red + src[j * dim + i + 1].red +
                                    src[j * dim + i + dim - 1].red + src[j * dim + i + dim].red + src[j * dim + i + dim + 1].red) / 9;
            dst[j * dim + i].green = (src[j * dim + i - dim - 1].green + src[j * dim + i - dim].green + src[j * dim + i - dim + 1].green +
                                      src[j * dim + i - 1].green + src[j * dim + i].green + src[j * dim + i + 1].green +
                                      src[j * dim + i + dim - 1].green + src[j * dim + i + dim].green + src[j * dim + i + dim + 1].green) / 9;
            dst[j * dim + i].blue = (src[j * dim + i - dim - 1].blue + src[j * dim + i - dim].blue + src[j * dim + i - dim + 1].blue +
                                     src[j * dim + i - 1].blue + src[j * dim + i].blue + src[j * dim + i + 1].blue +
                                     src[j * dim + i + dim - 1].blue + src[j * dim + i + dim].blue + src[j * dim + i + dim + 1].blue) / 9;
        }
        // II-3. col dim-1
        dst[j * dim + i].red = (src[j * dim + i - dim - 1].red + src[j * dim + i - dim].red +
                                src[j * dim + i - 1].red + src[j * dim + i].red +
                                src[j * dim + i + dim - 1].red + src[j * dim + i + dim].red) / 6;
        dst[j * dim + i].green = (src[j * dim + i - dim - 1].green + src[j * dim + i - dim].green +
                                  src[j * dim + i - 1].green + src[j * dim + i].green +
                                  src[j * dim + i + dim - 1].green + src[j * dim + i + dim].green) / 6;
        dst[j * dim + i].blue = (src[j * dim + i - dim - 1].blue + src[j * dim + i - dim].blue +
                                 src[j * dim + i - 1].blue + src[j * dim + i].blue +
                                 src[j * dim + i + dim - 1].blue + src[j * dim + i + dim].blue) / 6;
    }

    // III. row dim-1
    // III-1. col 1
    i = 0;
    dst[j * dim + i].red = (src[j * dim + i - dim].red + src[j * dim + i - dim + 1].red + src[j * dim + i].red + src[j * dim + i + 1].red) / 4;
    dst[j * dim + i].green = (src[j * dim + i - dim].green + src[j * dim + i - dim + 1].green + src[j * dim + i].green + src[j * dim + i + 1].green) / 4;
    dst[j * dim + i].blue = (src[j * dim + i - dim].blue + src[j * dim + i - dim + 1].blue + src[j * dim + i].blue + src[j * dim + i + 1].blue) / 4;
    // III-2. col 1 - dim-2
    for (i = 1; i < dim - 1; i++) {
        dst[j * dim + i].red = (src[j * dim + i - dim - 1].red + src[j * dim + i - dim].red + src[j * dim + i - dim + 1].red +
                                src[j * dim + i - 1].red + src[j * dim + i].red + src[j * dim + i + 1].red) / 6;
        dst[j * dim + i].green = (src[j * dim + i - dim - 1].green + src[j * dim + i - dim].green + src[j * dim + i - dim + 1].green +
                                  src[j * dim + i - 1].green + src[j * dim + i].green + src[j * dim + i + 1].green) / 6;
        dst[j * dim + i].blue = (src[j * dim + i - dim - 1].blue + src[j * dim + i - dim].blue + src[j * dim + i - dim + 1].blue +
                                 src[j * dim + i - 1].blue + src[j * dim + i].blue + src[j * dim + i + 1].blue) / 6;
    }
    // III-3. col dim-1
    dst[j * dim + i].red = (src[j * dim + i - dim - 1].red + src[j * dim + i - dim].red + src[j * dim + i - 1].red + src[j * dim + i].red) / 4;
    dst[j * dim + i].green = (src[j * dim + i - dim - 1].green + src[j * dim + i - dim].green + src[j * dim + i - 1].green + src[j * dim + i].green) / 4;
    dst[j * dim + i].blue = (src[j * dim + i - dim - 1].blue + src[j * dim + i - dim].blue + src[j * dim + i - 1].blue + src[j * dim + i].blue) / 4;

}

char smooth_0[] = "naive smooth + macro";
void smooth_func_0 (int dim, pixel *src, pixel *dst) {
    int i, j;

    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            dst[RIDX(i, j, dim)] = avg_with_macro(dim, i, j, src);
}

char smooth_1[] = "no procedure calls";
void smooth_func_1 (int dim, pixel *src, pixel *dst) {
    int i, j;

    // I. row 0
    j = 0;
    // I-1. col 1
    i = 0;
    dst[j * dim + i].red = (src[j * dim + i].red + src[j * dim + i + 1].red + src[j * dim + i + dim].red + src[j * dim + i + dim + 1].red) / 4;
    dst[j * dim + i].green = (src[j * dim + i].green + src[j * dim + i + 1].green + src[j * dim + i + dim].green + src[j * dim + i + dim + 1].green) / 4;
    dst[j * dim + i].blue = (src[j * dim + i].blue + src[j * dim + i + 1].blue + src[j * dim + i + dim].blue + src[j * dim + i + dim + 1].blue) / 4;
    // I-2. col 1 - dim-2
    for (i = 1; i < dim - 1; i++) {
        dst[j * dim + i].red = (src[j * dim + i - 1].red + src[j * dim + i].red + src[j * dim + i + 1].red +
                                src[j * dim + i + dim - 1].red + src[j * dim + i + dim].red + src[j * dim + i + dim + 1].red) / 6;
        dst[j * dim + i].green = (src[j * dim + i - 1].green + src[j * dim + i].green + src[j * dim + i + 1].green +
                                  src[j * dim + i + dim - 1].green + src[j * dim + i + dim].green + src[j * dim + i + dim + 1].green) / 6;
        dst[j * dim + i].blue = (src[j * dim + i - 1].blue + src[j * dim + i].blue + src[j * dim + i + 1].blue +
                                 src[j * dim + i + dim - 1].blue + src[j * dim + i + dim].blue + src[j * dim + i + dim + 1].blue) / 6;
    }
    // I-3. col dim-1
    dst[j * dim + i].red = (src[j * dim + i - 1].red + src[j * dim + i].red + src[j * dim + i + dim - 1].red + src[j * dim + i + dim].red) / 4;
    dst[j * dim + i].green = (src[j * dim + i - 1].green + src[j * dim + i].green + src[j * dim + i + dim - 1].green + src[j * dim + i + dim].green) / 4;
    dst[j * dim + i].blue = (src[j * dim + i - 1].blue + src[j * dim + i].blue + src[j * dim + i + dim - 1].blue + src[j * dim + i + dim].blue) / 4;

    //II. row 1 - dim-2
    for (j = 1; j < dim - 1; j++) {
        // II-1. col 1
        i = 0;
        dst[j * dim + i].red = (src[j * dim + i - dim].red + src[j * dim + i - dim + 1].red +
                                src[j * dim + i].red + src[j * dim + i + 1].red +
                                src[j * dim + i + dim].red + src[j * dim + i + dim + 1].red) / 6;
        dst[j * dim + i].green = (src[j * dim + i - dim].green + src[j * dim + i - dim + 1].green +
                                  src[j * dim + i].green + src[j * dim + i + 1].green +
                                  src[j * dim + i + dim].green + src[j * dim + i + dim + 1].green) / 6;
        dst[j * dim + i].blue = (src[j * dim + i - dim].blue + src[j * dim + i - dim + 1].blue +
                                 src[j * dim + i].blue + src[j * dim + i + 1].blue +
                                 src[j * dim + i + dim].blue + src[j * dim + i + dim + 1].blue) / 6;
        // II-2. col 1 - dim-2
        for (i = 1; i < dim - 1; i++) {
            dst[j * dim + i].red = (src[j * dim + i - dim - 1].red + src[j * dim + i - dim].red + src[j * dim + i - dim + 1].red +
                                    src[j * dim + i - 1].red + src[j * dim + i].red + src[j * dim + i + 1].red +
                                    src[j * dim + i + dim - 1].red + src[j * dim + i + dim].red + src[j * dim + i + dim + 1].red) / 9;
            dst[j * dim + i].green = (src[j * dim + i - dim - 1].green + src[j * dim + i - dim].green + src[j * dim + i - dim + 1].green +
                                      src[j * dim + i - 1].green + src[j * dim + i].green + src[j * dim + i + 1].green +
                                      src[j * dim + i + dim - 1].green + src[j * dim + i + dim].green + src[j * dim + i + dim + 1].green) / 9;
            dst[j * dim + i].blue = (src[j * dim + i - dim - 1].blue + src[j * dim + i - dim].blue + src[j * dim + i - dim + 1].blue +
                                     src[j * dim + i - 1].blue + src[j * dim + i].blue + src[j * dim + i + 1].blue +
                                     src[j * dim + i + dim - 1].blue + src[j * dim + i + dim].blue + src[j * dim + i + dim + 1].blue) / 9;
        }
        // II-3. col dim-1
        dst[j * dim + i].red = (src[j * dim + i - dim - 1].red + src[j * dim + i - dim].red +
                                src[j * dim + i - 1].red + src[j * dim + i].red +
                                src[j * dim + i + dim - 1].red + src[j * dim + i + dim].red) / 6;
        dst[j * dim + i].green = (src[j * dim + i - dim - 1].green + src[j * dim + i - dim].green +
                                  src[j * dim + i - 1].green + src[j * dim + i].green +
                                  src[j * dim + i + dim - 1].green + src[j * dim + i + dim].green) / 6;
        dst[j * dim + i].blue = (src[j * dim + i - dim - 1].blue + src[j * dim + i - dim].blue +
                                 src[j * dim + i - 1].blue + src[j * dim + i].blue +
                                 src[j * dim + i + dim - 1].blue + src[j * dim + i + dim].blue) / 6;
    }

    // III. row dim-1
    // III-1. col 1
    i = 0;
    dst[j * dim + i].red = (src[j * dim + i - dim].red + src[j * dim + i - dim + 1].red + src[j * dim + i].red + src[j * dim + i + 1].red) / 4;
    dst[j * dim + i].green = (src[j * dim + i - dim].green + src[j * dim + i - dim + 1].green + src[j * dim + i].green + src[j * dim + i + 1].green) / 4;
    dst[j * dim + i].blue = (src[j * dim + i - dim].blue + src[j * dim + i - dim + 1].blue + src[j * dim + i].blue + src[j * dim + i + 1].blue) / 4;
    // III-2. col 1 - dim-2
    for (i = 1; i < dim - 1; i++) {
        dst[j * dim + i].red = (src[j * dim + i - dim - 1].red + src[j * dim + i - dim].red + src[j * dim + i - dim + 1].red +
                                src[j * dim + i - 1].red + src[j * dim + i].red + src[j * dim + i + 1].red) / 6;
        dst[j * dim + i].green = (src[j * dim + i - dim - 1].green + src[j * dim + i - dim].green + src[j * dim + i - dim + 1].green +
                                  src[j * dim + i - 1].green + src[j * dim + i].green + src[j * dim + i + 1].green) / 6;
        dst[j * dim + i].blue = (src[j * dim + i - dim - 1].blue + src[j * dim + i - dim].blue + src[j * dim + i - dim + 1].blue +
                                 src[j * dim + i - 1].blue + src[j * dim + i].blue + src[j * dim + i + 1].blue) / 6;
    }
    // III-3. col dim-1
    dst[j * dim + i].red = (src[j * dim + i - dim - 1].red + src[j * dim + i - dim].red + src[j * dim + i - 1].red + src[j * dim + i].red) / 4;
    dst[j * dim + i].green = (src[j * dim + i - dim - 1].green + src[j * dim + i - dim].green + src[j * dim + i - 1].green + src[j * dim + i].green) / 4;
    dst[j * dim + i].blue = (src[j * dim + i - dim - 1].blue + src[j * dim + i - dim].blue + src[j * dim + i - 1].blue + src[j * dim + i].blue) / 4;}

char smooth_2[] = "reduction in strength";
void smooth_func_2 (int dim, pixel *src, pixel *dst) {

    int i, j;

    // I. row 0
    j = 0;
    // I-1. col 1
    i = 0;
    dst[j + i].red = (src[j + i].red + src[j + i + 1].red + src[j + i + dim].red + src[j + i + dim + 1].red) / 4;
    dst[j + i].green = (src[j + i].green + src[j + i + 1].green + src[j + i + dim].green + src[j + i + dim + 1].green) / 4;
    dst[j + i].blue = (src[j + i].blue + src[j + i + 1].blue + src[j + i + dim].blue + src[j + i + dim + 1].blue) / 4;
    // I-2. col 1 - dim-2
    for (i = 1; i < dim - 1; i++) {
        dst[j + i].red = (src[j + i - 1].red + src[j + i].red + src[j + i + 1].red +
                          src[j + i + dim - 1].red + src[j + i + dim].red + src[j + i + dim + 1].red) / 6;
        dst[j + i].green = (src[j + i - 1].green + src[j + i].green + src[j + i + 1].green +
                            src[j + i + dim - 1].green + src[j + i + dim].green + src[j + i + dim + 1].green) / 6;
        dst[j + i].blue = (src[j + i - 1].blue + src[j + i].blue + src[j + i + 1].blue +
                           src[j + i + dim - 1].blue + src[j + i + dim].blue + src[j + i + dim + 1].blue) / 6;
    }
    // I-3. col dim-1
    dst[j + i].red = (src[j + i - 1].red + src[j + i].red + src[j + i + dim - 1].red + src[j + i + dim].red) / 4;
    dst[j + i].green = (src[j + i - 1].green + src[j + i].green + src[j + i + dim - 1].green + src[j + i + dim].green) / 4;
    dst[j + i].blue = (src[j + i - 1].blue + src[j + i].blue + src[j + i + dim - 1].blue + src[j + i + dim].blue) / 4;

    //II. row 1 - dim-2
    for (j = dim; j < dim * dim - dim; j += dim) {
        // II-1. col 1
        i = 0;
        dst[j + i].red = (src[j + i - dim].red + src[j + i - dim + 1].red +
                          src[j + i].red + src[j + i + 1].red +
                          src[j + i + dim].red + src[j + i + dim + 1].red) / 6;
        dst[j + i].green = (src[j + i - dim].green + src[j + i - dim + 1].green +
                            src[j + i].green + src[j + i + 1].green +
                            src[j + i + dim].green + src[j + i + dim + 1].green) / 6;
        dst[j + i].blue = (src[j + i - dim].blue + src[j + i - dim + 1].blue +
                           src[j + i].blue + src[j + i + 1].blue +
                           src[j + i + dim].blue + src[j + i + dim + 1].blue) / 6;
        // II-2. col 1 - dim-2
        for (i = 1; i < dim - 1; i++) {
            dst[j + i].red = (src[j + i - dim - 1].red + src[j + i - dim].red + src[j + i - dim + 1].red +
                              src[j + i - 1].red + src[j + i].red + src[j + i + 1].red +
                              src[j + i + dim - 1].red + src[j + i + dim].red + src[j + i + dim + 1].red) / 9;
            dst[j + i].green = (src[j + i - dim - 1].green + src[j + i - dim].green + src[j + i - dim + 1].green +
                                src[j + i - 1].green + src[j + i].green + src[j + i + 1].green +
                                src[j + i + dim - 1].green + src[j + i + dim].green + src[j + i + dim + 1].green) / 9;
            dst[j + i].blue = (src[j + i - dim - 1].blue + src[j + i - dim].blue + src[j + i - dim + 1].blue +
                               src[j + i - 1].blue + src[j + i].blue + src[j + i + 1].blue +
                               src[j + i + dim - 1].blue + src[j + i + dim].blue + src[j + i + dim + 1].blue) / 9;
        }
        // II-3. col dim-1
        dst[j + i].red = (src[j + i - dim - 1].red + src[j + i - dim].red +
                          src[j + i - 1].red + src[j + i].red +
                          src[j + i + dim - 1].red + src[j + i + dim].red) / 6;
        dst[j + i].green = (src[j + i - dim - 1].green + src[j + i - dim].green +
                            src[j + i - 1].green + src[j + i].green +
                            src[j + i + dim - 1].green + src[j + i + dim].green) / 6;
        dst[j + i].blue = (src[j + i - dim - 1].blue + src[j + i - dim].blue +
                           src[j + i - 1].blue + src[j + i].blue +
                           src[j + i + dim - 1].blue + src[j + i + dim].blue) / 6;
    }

    // III. row dim-1
    // III-1. col 1
    i = 0;
    dst[j + i].red = (src[j + i - dim].red + src[j + i - dim + 1].red + src[j + i].red + src[j + i + 1].red) / 4;
    dst[j + i].green = (src[j + i - dim].green + src[j + i - dim + 1].green + src[j + i].green + src[j + i + 1].green) / 4;
    dst[j + i].blue = (src[j + i - dim].blue + src[j + i - dim + 1].blue + src[j + i].blue + src[j + i + 1].blue) / 4;
    // III-2. col 1 - dim-2
    for (i = 1; i < dim - 1; i++) {
        dst[j + i].red = (src[j + i - dim - 1].red + src[j + i - dim].red + src[j + i - dim + 1].red +
                          src[j + i - 1].red + src[j + i].red + src[j + i + 1].red) / 6;
        dst[j + i].green = (src[j + i - dim - 1].green + src[j + i - dim].green + src[j + i - dim + 1].green +
                            src[j + i - 1].green + src[j + i].green + src[j + i + 1].green) / 6;
        dst[j + i].blue = (src[j + i - dim - 1].blue + src[j + i - dim].blue + src[j + i - dim + 1].blue +
                           src[j + i - 1].blue + src[j + i].blue + src[j + i + 1].blue) / 6;
    }
    // III-3. col dim-1
    dst[j + i].red = (src[j + i - dim - 1].red + src[j + i - dim].red + src[j + i - 1].red + src[j + i].red) / 4;
    dst[j + i].green = (src[j + i - dim - 1].green + src[j + i - dim].green + src[j + i - 1].green + src[j + i].green) / 4;
    dst[j + i].blue = (src[j + i - dim - 1].blue + src[j + i - dim].blue + src[j + i - 1].blue + src[j + i].blue) / 4;

}

char smooth_3[] = "-o1";
void smooth_func_3 (int dim, pixel *src, pixel *dst) {

    int i, j;
    int dim_dim_dim;

    dim_dim_dim = dim * dim - dim;
    // I. row 0
    j = 0;
    // I-1. col 1
    i = 0;
    dst[j + i].red = (src[j + i].red + src[j + i + 1].red + src[j + i + dim].red + src[j + i + dim + 1].red) / 4;
    dst[j + i].green = (src[j + i].green + src[j + i + 1].green + src[j + i + dim].green + src[j + i + dim + 1].green) / 4;
    dst[j + i].blue = (src[j + i].blue + src[j + i + 1].blue + src[j + i + dim].blue + src[j + i + dim + 1].blue) / 4;
    // I-2. col 1 - dim-2
    for (i = 1; i < dim - 1; i++) {
        dst[j + i].red = (src[j + i - 1].red + src[j + i].red + src[j + i + 1].red +
                          src[j + i + dim - 1].red + src[j + i + dim].red + src[j + i + dim + 1].red) / 6;
        dst[j + i].green = (src[j + i - 1].green + src[j + i].green + src[j + i + 1].green +
                            src[j + i + dim - 1].green + src[j + i + dim].green + src[j + i + dim + 1].green) / 6;
        dst[j + i].blue = (src[j + i - 1].blue + src[j + i].blue + src[j + i + 1].blue +
                           src[j + i + dim - 1].blue + src[j + i + dim].blue + src[j + i + dim + 1].blue) / 6;
    }
    // I-3. col dim-1
    dst[j + i].red = (src[j + i - 1].red + src[j + i].red + src[j + i + dim - 1].red + src[j + i + dim].red) / 4;
    dst[j + i].green = (src[j + i - 1].green + src[j + i].green + src[j + i + dim - 1].green + src[j + i + dim].green) / 4;
    dst[j + i].blue = (src[j + i - 1].blue + src[j + i].blue + src[j + i + dim - 1].blue + src[j + i + dim].blue) / 4;

    //II. row 1 - dim-2
    for (j = dim; j < dim_dim_dim; j += dim) {
        // II-1. col 1
        i = 0;
        dst[j + i].red = (src[j + i - dim].red + src[j + i - dim + 1].red +
                          src[j + i].red + src[j + i + 1].red +
                          src[j + i + dim].red + src[j + i + dim + 1].red) / 6;
        dst[j + i].green = (src[j + i - dim].green + src[j + i - dim + 1].green +
                            src[j + i].green + src[j + i + 1].green +
                            src[j + i + dim].green + src[j + i + dim + 1].green) / 6;
        dst[j + i].blue = (src[j + i - dim].blue + src[j + i - dim + 1].blue +
                           src[j + i].blue + src[j + i + 1].blue +
                           src[j + i + dim].blue + src[j + i + dim + 1].blue) / 6;
        // II-2. col 1 - dim-2
        for (i = 1; i < dim - 1; i++) {
            dst[j + i].red = (src[j + i - dim - 1].red + src[j + i - dim].red + src[j + i - dim + 1].red +
                              src[j + i - 1].red + src[j + i].red + src[j + i + 1].red +
                              src[j + i + dim - 1].red + src[j + i + dim].red + src[j + i + dim + 1].red) / 9;
            dst[j + i].green = (src[j + i - dim - 1].green + src[j + i - dim].green + src[j + i - dim + 1].green +
                                src[j + i - 1].green + src[j + i].green + src[j + i + 1].green +
                                src[j + i + dim - 1].green + src[j + i + dim].green + src[j + i + dim + 1].green) / 9;
            dst[j + i].blue = (src[j + i - dim - 1].blue + src[j + i - dim].blue + src[j + i - dim + 1].blue +
                               src[j + i - 1].blue + src[j + i].blue + src[j + i + 1].blue +
                               src[j + i + dim - 1].blue + src[j + i + dim].blue + src[j + i + dim + 1].blue) / 9;
        }
        // II-3. col dim-1
        dst[j + i].red = (src[j + i - dim - 1].red + src[j + i - dim].red +
                          src[j + i - 1].red + src[j + i].red +
                          src[j + i + dim - 1].red + src[j + i + dim].red) / 6;
        dst[j + i].green = (src[j + i - dim - 1].green + src[j + i - dim].green +
                            src[j + i - 1].green + src[j + i].green +
                            src[j + i + dim - 1].green + src[j + i + dim].green) / 6;
        dst[j + i].blue = (src[j + i - dim - 1].blue + src[j + i - dim].blue +
                           src[j + i - 1].blue + src[j + i].blue +
                           src[j + i + dim - 1].blue + src[j + i + dim].blue) / 6;
    }

    // III. row dim-1
    // III-1. col 1
    i = 0;
    dst[j + i].red = (src[j + i - dim].red + src[j + i - dim + 1].red + src[j + i].red + src[j + i + 1].red) / 4;
    dst[j + i].green = (src[j + i - dim].green + src[j + i - dim + 1].green + src[j + i].green + src[j + i + 1].green) / 4;
    dst[j + i].blue = (src[j + i - dim].blue + src[j + i - dim + 1].blue + src[j + i].blue + src[j + i + 1].blue) / 4;
    // III-2. col 1 - dim-2
    for (i = 1; i < dim - 1; i++) {
        dst[j + i].red = (src[j + i - dim - 1].red + src[j + i - dim].red + src[j + i - dim + 1].red +
                          src[j + i - 1].red + src[j + i].red + src[j + i + 1].red) / 6;
        dst[j + i].green = (src[j + i - dim - 1].green + src[j + i - dim].green + src[j + i - dim + 1].green +
                            src[j + i - 1].green + src[j + i].green + src[j + i + 1].green) / 6;
        dst[j + i].blue = (src[j + i - dim - 1].blue + src[j + i - dim].blue + src[j + i - dim + 1].blue +
                           src[j + i - 1].blue + src[j + i].blue + src[j + i + 1].blue) / 6;
    }
    // III-3. col dim-1
    dst[j + i].red = (src[j + i - dim - 1].red + src[j + i - dim].red + src[j + i - 1].red + src[j + i].red) / 4;
    dst[j + i].green = (src[j + i - dim - 1].green + src[j + i - dim].green + src[j + i - 1].green + src[j + i].green) / 4;
    dst[j + i].blue = (src[j + i - dim - 1].blue + src[j + i - dim].blue + src[j + i - 1].blue + src[j + i].blue) / 4;

}

char smooth_4[] = " ";
void smooth_func_4 (int dim, pixel *src, pixel *dst) {

}
char smooth_5[] = "";
void smooth_func_5 (int dim, pixel *src, pixel *dst) {

}
char smooth_6[] = "";
void smooth_func_6 (int dim, pixel *src, pixel *dst) {

}
/*********************************************************************
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_smooth_functions() {
#if 1
    add_smooth_function(&smooth, smooth_descr);
    // add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* ... Register additional test functions here */
    // add_smooth_function(&smooth_func_0, smooth_0);
    /*
    add_smooth_function(&smooth_func_1, smooth_1);
    add_smooth_function(&smooth_func_2, smooth_2);
    add_smooth_function(&smooth_func_3, smooth_3);
    add_smooth_function(&smooth_func_4, smooth_4);
    add_smooth_function(&smooth_func_5, smooth_5);
*/
#endif
}



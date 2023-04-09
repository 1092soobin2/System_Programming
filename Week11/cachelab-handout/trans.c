// 2019011449

/*
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";

void transpose_32(int M, int N, int A[N][M], int B[M][N]) {

    int row, col;
    int i, j;
    int buffer[2];

    for (row = 0; row < M; row += 8)
        for (col = 0; col < N; col += 8) {
            for (i = row; i < row + 8; i++) {
                buffer[1] = -1;
                for (j = col; j < col + 8; j++)
                    if (i != j)
                        B[j][i] = A[i][j];
                    else {
                        buffer[0] = A[i][j];
                        buffer[1] = i;
                    }
                if (buffer[1] != -1)
                    B[buffer[1]][buffer[1]] = buffer[0];
            }
        }
}

void transpose_64(int M, int N, int A[N][M], int B[M][N]) {
    int row, col;
    int i, j;
    int buffer[8];

    for (row = 0; row < M; row += 8)
        for (col = 0; col < N; col += 8) {
            for (i = row; i < row + 4; i++) {
                for (j = 0; j < 8; j++)
                    buffer[j] = A[i][col + j];
                for (j = 0; j < 4; j++) {
                    B[col + j][i] = buffer[j];
                    B[col + j][i + 4] = buffer[j + 4];
                }
            }
            for (i = col; i < col + 4; i++) {
                for (j = 0; j < 4; j++)
                    buffer[j] = B[i][row + 4 + j];
                for (j = 0; j < 4; j++)
                    B[i][row + 4 + j] = A[row + 4 + j][i];
                for (j = 0; j < 4; j++)
                    B[i + 4][row + j] = buffer[j];
                for (j = 4; j < 8; j++)
                    B[i + 4][row + j] = A[row + j][i + 4];
            }
        }
}

void transpose_61(int M, int N, int A[N][M], int B[M][N]) {
    int row, col;
    int i, j;
    int buffer[2];
    for (row = 0; row < N; row += 16)
        for (col = 0; col < M; col += 16) {
            for (i = row; (i < row + 16) && (i < N); i++) {
                buffer[1] = -1;
                for (j = col; (j < col + 16) && (j < M); j++)
                    if (i != j)
                        B[j][i] = A[i][j];
                    else {
                        buffer[0] = A[i][j];
                        buffer[1] = i;
                    }
                if (buffer[1] != -1)
                    B[buffer[1]][buffer[1]] = buffer[0];
            }
        }
}

void transpose_submit(int M, int N, int A[N][M], int B[M][N]) {

    if (M == 32)
        transpose_32(M, N, A, B);
    else if (M == 64)
        transpose_64(M, N, A, B);
    else if (M == 61)
        transpose_61(M, N, A, B);
}
/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}



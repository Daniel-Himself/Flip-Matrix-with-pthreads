#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>

#define CORE 4

// Initialize two matrices
int M; //rows
int N; //columns
int **mat_A;
int **mat_B;

pthread_t threads[CORE];

/*Function decleration*/
void *flip_matrix(void *arg);
void printMatrix(int **mat, int rows, int columns);

int main()
{

    int status, i, j;

    printf("Please choose the row size: ");
    if (!scanf("%d", &M))
    {
        printf("Input Error!");
        exit(EXIT_FAILURE);
    }
    printf("Please choose the column size: ");
    if (!scanf("%d", &N))
    {
        printf("Input Error!");
        exit(EXIT_FAILURE);
    }

    /* Dinamiclly allocate space for matrix A*/
    if ((mat_A = (int **)malloc(M * sizeof(int *))) == NULL)
    {
        printf("Allocation Error!\n");
        free(mat_A);
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < M; ++i)
    {
        if ((mat_A[i] = (int *)malloc(N * sizeof(int))) == NULL)
        {
            printf("Allocation Error!\n");
            free(mat_A);
            exit(EXIT_FAILURE);
        }
    }

    /* Dinamiclly allocate space for Matrix B, with the amount of rows and columns flipped from matrix A*/
    if ((mat_B = (int **)malloc(N * sizeof(int *))) == NULL)
    {
        printf("Allocation Error!\n");
        free(mat_B);
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < N; i++)
    {
        if ((mat_B[i] = (int *)malloc(M * sizeof(int))) == NULL)
        {
            printf("Allocation Error!\n");
            free(mat_B);
            exit(EXIT_FAILURE);
        }
    }

    if (M < 1 || N < 1) // Error check for illegal matrix size
    {
        printf("There must be at least one column and one row!\n");
        exit(EXIT_FAILURE);
    }

    /* User input for matrix A*/
    for (i = 0; i < M; i++)
    {
        for (j = 0; j < N; j++)
        {
            printf("Choose a number for the (%d,%d) position of the matrix: ", i, j);
            if (!scanf("%d", &mat_A[i][j]))
            {
                printf("Input Error!\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    // Display matrix A
    printf("\nMatrix:\n");
    printMatrix(mat_A,M,N);

    /* Create CORE (defined) threads so each one flips one fourth of the matrix elements*/
    for (i = 0; i < CORE; i++)
    {
        status = pthread_create(&threads[i], NULL, &flip_matrix, (void *)(intptr_t)i);

        if (status) // Error check for thread creation
        {
            fputs("pthread_create failed in main", stderr);
            exit(EXIT_FAILURE);
        }
    }

    // Main thread waits for the four threads to finish their execution
    for (i = 0; i < CORE; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Display the flipped matrix, matrix B
    printf("\nThe rotated matrix:\n");
    printMatrix(mat_B,N,M);

    // Free the allocated memory of both matrices
    for (int i = 0; i < M; i++)
        free(mat_A[i]);
    free(mat_A);

    for (int i = 0; i < N; i++)
        free(mat_B[i]);
    free(mat_B);

    return (EXIT_SUCCESS);
} // END OF MAIN 

/* Function for the element flipping logic */
void *flip_matrix(void *arg)
{
    int threadIndex = (intptr_t)arg, index, i, j; // Convert argument back to int

    for (index = threadIndex; index < M * N; index += CORE)
    {
        i = index / N;
        j = index % N;
        mat_B[j][M - 1 - i] = mat_A[i][j];
    }
}

/* Function to print a matrix */
void printMatrix(int **mat, int rows, int columns)
{
    int i, j;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < columns; j++)
        {
            printf("%d\t", mat[i][j]);
        }
        printf("\n");
    }
}
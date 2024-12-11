#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

float *createMatrix(int matrixSize) {
    return (float *)calloc(matrixSize * matrixSize, sizeof(float));
}

void deleteMatrix(float *matrix) {
    free(matrix);
}

void swapMatrix(float *source, float *target, int matrixSize) {
    for (int row = 0; row < matrixSize; row++) {    
        for (int col = 0; col < matrixSize; col++) {
            target[row * matrixSize + col] = source[col * matrixSize + row];
        }
    }
}

// ||A||1
float computeNorm1(float *matrix, int matrixSize) {
    float maxRowSum = 0;
    for (int row = 0; row < matrixSize; row++) {
        float currentRowSum = 0;
        for (int col = 0; col < matrixSize; col++) {
            currentRowSum += fabs(matrix[row * matrixSize + col]);
        }
        if (currentRowSum > maxRowSum) {
            maxRowSum = currentRowSum;
        }
    }
    return maxRowSum;
}

// ||A||inf
float computeNormInf(float *matrix, int matrixSize) {
    float maxColSum = 0;
    for (int col = 0; col < matrixSize; col++) {
        float currentColSum = 0;
        for (int row = 0; row < matrixSize; row++) {
            currentColSum += fabs(matrix[row * matrixSize + col]);
        }
        if (currentColSum > maxColSum) {
            maxColSum = currentColSum;
        }
    }
    return maxColSum;
}

void multiplyMatrices(float *firstMatrix, float *secondMatrix, float *resultMatrix, int matrixSize) {
    for (int row = 0; row < matrixSize; row++) {
        for (int col = 0; col < matrixSize; col++) {
            resultMatrix[row * matrixSize + col] = 0;
            for (int k = 0; k < matrixSize; k++) {
                resultMatrix[row * matrixSize + col] += firstMatrix[row * matrixSize + k] * secondMatrix[k * matrixSize + col];
            }
        }
    }
}

float *createIdentityMatrix(int matrixSize) {
        float *identityMatrix = createMatrix(matrixSize);
        for (int row = 0; row < matrixSize; row++) {
            identityMatrix[row * matrixSize + row] = 1;
        }
        return identityMatrix;
}

float *computeB(float *inputMatrix, int matrixSize) {
        float *transposedMatrix = createMatrix(matrixSize);
        swapMatrix(inputMatrix, transposedMatrix, matrixSize);

        float norm1 = computeNorm1(inputMatrix, matrixSize);
        float normInf = computeNormInf(inputMatrix, matrixSize);
        float scalingFactor = norm1 * normInf;
        float *B = createMatrix(matrixSize);
        for (int row = 0; row < matrixSize; row++) {
            for (int col = 0; col < matrixSize; col++) {
                B[row * matrixSize + col] = transposedMatrix[row * matrixSize + col] / scalingFactor;
            }
        }

        deleteMatrix(transposedMatrix);
        return B;
}

float *computeR(float *BA, float *identityMatrix, int matrixSize) {
        float *R = createMatrix(matrixSize);
        for (int row = 0; row < matrixSize; row++) {
            for (int col = 0; col < matrixSize; col++) {
                R[row * matrixSize + col] = identityMatrix[row * matrixSize + col] - BA[row * matrixSize + col];
            }
        }
        return R;
}

float *invertMatrix(float *inputMatrix, int matrixSize, int maxIterations) {
        float *identityMatrix = createIdentityMatrix(matrixSize);
        float *B = computeB(inputMatrix, matrixSize);
        float *BA = createMatrix(matrixSize);
        multiplyMatrices(B, inputMatrix, BA, matrixSize);
        float *R = computeR(BA, identityMatrix, matrixSize);
        deleteMatrix(BA);

        float *resultMatrix = createMatrix(matrixSize);
        float *currentTermMatrix = createMatrix(matrixSize);

        for (int row = 0; row < matrixSize; row++) {
            for (int col = 0; col < matrixSize; col++) {
                resultMatrix[row * matrixSize + col] = identityMatrix[row * matrixSize + col];
                currentTermMatrix[row * matrixSize + col] = R[row * matrixSize + col];
            }
        }

        for (int iteration = 1; iteration < maxIterations; iteration++) {
            for (int row = 0; row < matrixSize; row++) {
                for (int col = 0; col < matrixSize; col++) {
                    resultMatrix[row * matrixSize + col] += currentTermMatrix[row * matrixSize + col];
                }
            }
            if (iteration != (maxIterations - 1)) {
                float *tempMatrix = createMatrix(matrixSize);
                multiplyMatrices(currentTermMatrix, R, tempMatrix, matrixSize);
                for (int row = 0; row < matrixSize; row++) {
                    for (int col = 0; col < matrixSize; col++) {
                        currentTermMatrix[row * matrixSize + col] = tempMatrix[row * matrixSize + col];
                    }
                }
                deleteMatrix(tempMatrix);
            }
        }

        float *inverseMatrix = createMatrix(matrixSize);
        multiplyMatrices(resultMatrix, B, inverseMatrix, matrixSize);

        deleteMatrix(identityMatrix);
        deleteMatrix(B);
        deleteMatrix(R);
        deleteMatrix(resultMatrix);
        deleteMatrix(currentTermMatrix);

        return inverseMatrix;
}

void displayMatrix(float *matrix, int totalSize, int displaySize) {
        if(displaySize > 5) {
            printf("Display size greater than 5. Displaying first 5 elements\n");
            displaySize = 5;
        }

        if (displaySize > totalSize) {
            printf("Display size greater than matrix size! Displaying whole matrix\n");
            displaySize = totalSize;
        }

        

        for (int row = 0; row < displaySize; row++) {
            for (int col = 0; col < displaySize; col++) {
                printf("%8.3f ", matrix[row * totalSize + col]); 
            }
            printf("\n"); 
        }
        printf("\n"); 
}

int main(int argc, char **argv) {
        if (argc < 4) {
            printf("Must be 3 args\nsyntax: main.exe <N> <M> <n>\nN-size of matrix; M-count of interations; n-size of first printed elements (max 5)");
            return 1;
        }

        int matrixSize = atoi(argv[1]);
        int iterations = atoi(argv[2]);
        int n = atoi(argv[3]);

        if (matrixSize <= 0 || iterations <= 0) {
            printf("Bad matrix or iterations number!\n");
            return 1;
        }

        srand(time(NULL));

        float *inputMatrix = createMatrix(matrixSize);
        for (int row = 0; row < matrixSize; row++) {
            for (int col = 0; col < matrixSize; col++) {
                inputMatrix[row * matrixSize + col] = rand() % 10;
            }
        }
        printf("Original matrix (first %dx%d):\n", n, n);
        displayMatrix(inputMatrix, matrixSize, n);
        
        
        struct timeval startTime, endTime;
        gettimeofday(&startTime, NULL);
        float *inverseMatrix = invertMatrix(inputMatrix, matrixSize, iterations);
        gettimeofday(&endTime, NULL);

        double elapsedTime = (endTime.tv_sec - startTime.tv_sec) + 
                            (endTime.tv_usec - startTime.tv_usec) / 1000000.0; // преобразование микросекунд в секунды
        printf("Time taken: %lf seconds\n", elapsedTime);
        printf("Inverse matrix (first %dx%d):\n", n, n);
        displayMatrix(inverseMatrix, matrixSize, n);
        deleteMatrix(inputMatrix);
        deleteMatrix(inverseMatrix);

        return 0;
}


#include<stdio.h>
#include<string.h>

double** getMatrixFromCsv(char* filename, int size){
    FILE* fp = fopen(filename,"r");
    double **m = malloc(size * sizeof(double *));

    for (int i = 0; i < size; i++)
    {
        char fileRow[20*size];
        char* ret = fgets(fileRow, 20*size, fp);
        if(ret==NULL){
            printf("Error parsing!\n");
            return NULL;
        }
        
        m[i] = malloc(size * sizeof(double));
        m[i][0] = atof(strtok(fileRow, ","));
        for (int j = 1; j < size; j++)
        {
            double n = atof(strtok(NULL,","));
            m[i][j] = n;
        }
    }
    return m;
}

void writeLineToCsv(char* filename, long int* array, long int arraySize){
    FILE* fp = fopen(filename,"w+");

    fprintf(fp, "%ld",array[0]);
    for(long int i=1; i<arraySize; i++){
        fprintf(fp, ",%ld",array[i]);
    }

    fclose(fp);
}

void writeMatrixToCsv(char* filename, double** matrix, int matrixSize)
{
    FILE* fp = fopen(filename,"w+");

    for(long int i=0; i<matrixSize; i++)
    {
        fprintf(fp, "%f",matrix[i][0]);
        for(long int j=1; j<matrixSize; j++){
            fprintf(fp, ",%f",matrix[i][j]);
        }
        if(i<matrixSize-1){
            fprintf(fp, "\n");
        }
    }

    fclose(fp);
}
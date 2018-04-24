#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <papi.h>

float **multiplyMatrices(float **m1, float **m2, int n)
{
    //generate zero matrix
    float **m = (float**)malloc(n * sizeof(float*));

    for (int i = 0; i < n; i++)
    {
        m[i] = (float*)malloc(n*sizeof(float));
        for (int j = 0; j < n; j++)
        {
            m[i][j] = 0;
        }
    }

    // mult matrices
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            //printf("(%d, %d)\n",i,j);
            for (int t = 0; t < n; t++)
            {
                m[i][j] += m1[i][t] * m2[t][j];
            }
        }
    }

    return m;
}

//---------------------------------------------------

void displayTabbedMatrix(float** m, int n, int spaces){
    for(int i=0; i<n; i++){
        for(int t=0; t<spaces; t++){
            printf(" ");
        }
        for(int j=0; j<n; j++){
            printf("%.0f ",m[i][j]);
        }
        printf("\n");
    }
}

void displayMatrix(float** m, int n){
    displayTabbedMatrix(m, n, 0);
}

float** generateMatrix(int n){
    float** m = malloc(n * sizeof(float*));

    for (int i = 0; i < n; i++){
        float* row = malloc(n * sizeof(float));
        m[i]=row;
        for (int j = 0; j < n; j++){
            m[i][j] = rand()%10;
        }
    }
    return m;
}

float** generateUpperMatrix(int n){
    float** m = malloc(n * sizeof(float*));

    for (int i = 0; i < n; i++){
        float* row = malloc(n * sizeof(float));
        m[i]=row;
        for (int j = 0; j < n; j++){
            if(i>j)
                m[i][j]=0;
            //else m[i][j]=-1;
        }
    }
    return m;
}

float** generateLowerMatrix(int n){
    float** m = malloc(n * sizeof(float*));

    for (int i = 0; i < n; i++){
        float* row = malloc(n * sizeof(float));
        m[i]=row;
        for (int j = 0; j < n; j++){
            if(i<j)
                m[i][j]=0;
            else if(i==j)
                m[i][j]=1;
            //else m[i][j]=-1;
        }
    }
    return m;
}

// http://mathonline.wikidot.com/the-algorithm-for-doolittle-s-method-for-lu-decompositions
void dolittle_naive(float** a, float** l, float** u, int n){
    for(int k=0; k<n; k++){
        for(int m=k; m<n; m++){ //kth row of u
            u[k][m] = a[k][m];
            for(int j=0; j<k; j++){
                u[k][m] -= l[k][j]*u[j][m];
            }
        }
        for(int i=k; i<n; i++){ //kth column of l
            l[i][k] = a[i][k];
            for(int j=0; j<k; j++){
                l[i][k] -= l[i][j]*u[j][k];
            }
            l[i][k] /= u[k][k];
        }
    }
}

int main(int argc, char **argv){
    srand(time(NULL));

    int n = 3;
    float** a = generateMatrix(n);
    float** l = generateLowerMatrix(n);
    float** u = generateUpperMatrix(n);
    displayMatrix(a, n);
    printf("\n");
    displayTabbedMatrix(u, n, 5);
    displayMatrix(l, n);//*/

    dolittle_naive(a, l, u, n);

    printf("\n");
    displayTabbedMatrix(u, n, 5);
    displayMatrix(l, n);//*/

    printf("\n");
    displayMatrix(multiplyMatrices(l,u,n),n);

    return 0;
}
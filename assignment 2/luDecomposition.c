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
    int ret;

    ret = PAPI_library_init(PAPI_VER_CURRENT);
    if (ret != PAPI_VER_CURRENT)
        printf("ERROR: Failed version\n");
    
    

    for(int i=1000; i<=6000; i+=1000){
        long long startTime = PAPI_get_real_usec();

        float** a = generateMatrix(i);
        float** l = generateLowerMatrix(i);
        float** u = generateUpperMatrix(i);
        /*displayMatrix(a, i);
        printf("\n");
        displayTabbedMatrix(u, i, 5);
        displayMatrix(l, i);//*/

        dolittle_naive(a, l, u, i);
        long long endTime = PAPI_get_real_usec();
        double deltaTime = ((double)endTime - startTime) / 1000000; //in seconds
        printf("Calculated %dx%d in %.3lfs\n", i, i, deltaTime);

        /*printf("\n");
        displayTabbedMatrix(u, i, 5);
        displayMatrix(l, i);//*/

        /*printf("\n");
        displayMatrix(multiplyMatrices(l,u,i),i);//*/
    }

    return 0;
}
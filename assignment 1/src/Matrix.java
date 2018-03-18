import java.util.Random;
import java.util.Arrays;


public class Matrix{

    public static float[][] generateMatrix(int n){
        Random r = new Random();
        float[][] matrix = new float[n][n];
        
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                matrix[i][j]=r.nextInt(10);
            }
        }
        return matrix;
    }

    public static float[][] multiplyMatrices(float[][] m1, float[][] m2, int n){
        float[][] m3 = new float[n][n];

        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                for(int t=0; t<n; t++){
                    m3[i][j] += m1[i][t]*m2[t][j];
                }
            }
        }

        return m3;
    }

    public static void genAndMultMatrices(int n){
        float[][] m1 = generateMatrix(n);
        //printMatrix(m1,n);
        float[][] m2 = generateMatrix(n);
        //printMatrix(m2,n);
        float[][] m3 = multiplyMatrices(m1,m2,n);
        //printMatrix(m3,n);
    }

    public static void main(String[] args){
        for(int i=500; i<=3000; i+=500){
            long startMillis = System.currentTimeMillis();
            genAndMultMatrices(i);
            long finishMillis = System.currentTimeMillis();
            float elapsed = (finishMillis - startMillis)/1000F;
            System.out.println(i+"x"+i+": "+elapsed+"s");
        }
    }

    public static void printMatrix(float[][] m, int n){
        for(int i=0; i<n; i++){
            System.out.print("| ");
            for(int j=0; j<n; j++){
                System.out.print(m[i][j]+" | ");
            }
            System.out.println();
        }
        System.out.println();
    }
}
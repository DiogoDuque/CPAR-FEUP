import java.util.Random;
import java.util.Arrays;


public class Matrix{

    public static int[][] generateMatrix(int n){
        Random r = new Random();
        int[][] matrix = new int[n][n];
        
        for(int i=0; i<n; i++){
            int[] row = new int[n];
            for(int j=0; j<n; j++){
                row[j]=r.nextInt(10);
            }
            matrix[i]=row;
        }
        return matrix;
    }

    public static int[][] multiplyMatrices(int[][] m1, int[][] m2, int n){
        int[][] m3 = new int[n][n];

        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                for(int t=0; t<n; t++){
                    m3[i][j] += m1[i][t]*m2[t][j];
                }
            }
        }

        return m3;
    }

    public static void main(String[] args){
        final int n = 2;
        int[][] m1 = generateMatrix(n);
        printMatrix(m1,n);
        int[][] m2 = generateMatrix(n);
        printMatrix(m2,n);
        int[][] m3 = multiplyMatrices(m1,m2,n);
        printMatrix(m3,n);
    }

    public static void printMatrix(int[][] m, int n){
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
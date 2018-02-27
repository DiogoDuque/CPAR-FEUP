using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CPAR
{
    class Matrix
    {
        static Random random;

        static int[][] GenerateMatrix(int n)
        {
            int[][] matrix = new int[n][];

            for(int i=0; i<n; i++)
            {
                int[] row = new int[n];
                for(int j=0; j<n; j++)
                {
                    row[j] = random.Next(0, 10);
                }
                matrix[i] = row;
            }

            return matrix;
        }

        static int[][] multMatrices(int[][] m1, int[][] m2, int n)
        {
            int[][] m3 = new int[n][];

            for (int i = 0; i < n; i++)
            {
                int[] row = new int[n];
                for (int j = 0; j < n; j++)
                {
                    int cell = 0;
                    for (int t = 0; t < n; t++)
                    {
                        cell += m1[i][t] * m2[t][j];
                    }
                    row[j] = cell;
                }
                m3[i] = row;
            }

            return m3;
        }

        static void Main(string[] args)
        {
            random = new Random();
            int n = 2;

            int[][] m1 = GenerateMatrix(n);
            int[][] m2 = GenerateMatrix(n);
            int[][] m3 = multMatrices(m1, m2, n);

            printMatrix(m1, n);
            printMatrix(m2, n);
            printMatrix(m3, n);
            //Console.ReadKey();
        }

        static void printMatrix(int[][] m, int n)
        {
            for(int i=0; i<n; i++)
            {
                for(int j=0; j<n; j++)
                {
                    Console.Write("|"+m[i][j]);
                }
                Console.WriteLine("|");
            }
            Console.WriteLine();
        }
    }
}

from random import randint
import time


def generateMatrix(n):
    matrix = []
    for i in range(0, n):
        matrix.append([randint(0,9) for x in range(0,n)])
    return matrix

def multMatrices(m1, m2, n):
    m3 = []

    for i in range(0,n):
        row = []
        for j in range(0,n):
            cell = 0
            for t in range(0,n):
                cell = cell + (m1[i][t] * m2[t][j])
            row.append(cell)
        m3.append(row)
    return m3
                

def generateAndMultMatrices(n):
    m1 = generateMatrix(n)
    m2 = generateMatrix(n)
    m3 = multMatrices(m1,m2,n)

def main():
    n = 500
    maxN = 3000
    incN = 500

    absoluteStartTime = time.time()
    while n<=maxN:
        startTime = time.time()
        
        generateAndMultMatrices(n)
        
        finishTime = time.time()
        elapsedTime = finishTime - startTime
        absoluteElapsedTime = finishTime - absoluteStartTime
        print("%sx%s"%(n,n))
        print("current set time: %.2f"%(elapsedTime))
        print("global time: %.2f"%(absoluteElapsedTime))
        print("")

        n += incN

if __name__ == "__main__":
    main()
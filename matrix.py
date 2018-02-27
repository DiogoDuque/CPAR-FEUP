from random import randint


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
                

def main():
    n = 2
    m1 = generateMatrix(n)
    m2 = generateMatrix(n)
    m3 = multMatrices(m1,m2,n)

if __name__ == "__main__":
    main()
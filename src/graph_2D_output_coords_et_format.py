import sys
import matplotlib.pyplot as plt
import numpy as np

#PATH="\Users\maxen\Desktop\FORMATION\2A\INF442\projet\code\connected-components\data\"
#print(PATH)
PATH="data/"
#print(PATH)

def distance_2D(ax, ay, bx, by):
    return np.sqrt( (ax - bx) ** 2 + (ay - by) ** 2)


def create_graph(n, p):
    # Save coordinates
    np.random.seed(0)
    xn = np.random.randn(n)
    yn = np.random.randn(n)

    with open(PATH + "graph_2D_coordinates.txt", "w") as f:
        for i in range(n):
            f.write(str(xn[i]) + ' ' + str(yn[i]) + "\n")

    # Set edges
    distances = np.ones((n, n)) * -1
    nb_edge = 0
    for i in range(n):
        for j in range(n):
            if (i != j and np.random.binomial(1, p) > 0.5):
                nb_edge += 1
                distances[i][j] = np.sqrt(distance_2D(xn[i], yn[i], xn[j], yn[j]) )
    # Save edges
    with open(PATH + "graph_2D.txt", "w") as f:
        f.write(str(1) + ' ' + str(1) + '\n')
        f.write(str(n) + ' ' + str(nb_edge) + '\n')
        for i in range(n):
            for j in range(n):
                if distances[i][j] > 0:
                    f.write(str(i) + ' ' + str(j) + ' ' + str(distances[i][j]) + '\n')
    return 0

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print('Usage: python graph_2D_output_coords_et_format.py nb_points proba_edges')
        sys.exit(1)

    n=int(sys.argv[1])
    p=float(sys.argv[2])

    create_graph(n, p)

import matplotlib.pyplot as plt
import numpy as np
import sys

# Given a prediction, return the linear regression and the r2 value
def regression(x, y):
    beta = (x * y).sum()
    beta /= (x * x).sum()
    mean = (y - beta * x).mean()
    prediction = beta * x + mean
    r2 = 1 - ((prediction - y) ** 2).sum() / ((prediction - prediction.mean()) ** 2).sum()
    return beta, mean, r2

def show():
    plt.figure(figsize=(10, 10))
    print("Reading file...")
    ns_loc = []
    ps = []
    nb_clusters = []
    proportions = []

    # Read file
    with open("scc-p/result_n-p.txt", 'r') as f:
        lines = f.readlines()
        for line in lines:
            n, p, nb_cluster, proportion = line.split(" ")
            n = int(n)
            p = float(p)
            nb_cluster = float(nb_cluster)
            proportion = float(proportion)
            ns_loc.append(n)
            ps.append(p)
            nb_clusters.append(nb_cluster)
            proportions.append(proportion)

    ns, ps = zip(*sorted(zip(ns_loc, ps)))
    _, nb_clusters = zip(*sorted(zip(ns_loc, nb_clusters)))
    _, proportions = zip(*sorted(zip(ns_loc, proportions)))
    
    ns = np.array(ns)
    ps = np.array(ps)
    start = 0
    x_base = np.linspace(0, 1000, 100000, endpoint=True)

    # Diagram n-p with maximum scc.
    plt.subplot("311")
    x = ns[start:]
    y = (ps[start:]) * ns[start:] * ns[start:]
    plt.plot(x, y)
    a, b, r = regression(x, y)
    plt.plot(x_base, a * x_base + b, label="{}x + {}".format(a, b))
    plt.xticks([])
    plt.ylabel("p * n * n")
    plt.legend()
    plt.title("Diagram n-p with maximum scc.\n R^2 = {}".format(r))

    # Diagram n-nb_scc with maximum scc.
    plt.subplot("312")
    x = ns[start:]
    y = nb_clusters[start:] / np.log(ns[start:]) * ns[start:]
    plt.plot(x, y)
    a, b, r = regression(x, y)
    plt.plot(x_base, a * x_base + b, label="{}x + {}".format(a, b))
    plt.xticks([])
    plt.ylabel("nb_scc * n / log(n)")
    plt.legend()
    plt.title("Diagram n-nb_scc with maximum scc.\n R^2 = {}".format(r))

    # Diagram n-proportions with maximum scc
    plt.subplot("313")
    x = ns[start:]
    y = proportions[start:] * ns[start:] ** (1.5)
    plt.plot(x, y)
    a, b, r = regression(x, y)
    plt.plot(x_base, a * x_base + b, label="{}x + {}".format(a, b))
    plt.xlabel("n")
    plt.ylabel("proportions * n ^ 1.5")
    plt.legend()
    plt.title("Diagram n-proportions with maximum scc.\n R^2 = {}".format(r))
    plt.savefig("scc-p/scc-p_critique.png")

    plt.show()
    return 0


if __name__ == '__main__':
    show()

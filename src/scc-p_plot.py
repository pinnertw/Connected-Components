import matplotlib.pyplot as plt
import sys

def show():
    # Read file
    print("Reading file...")
    xs = []
    ys_loc = []
    proportions = []
    maximums = []
    maximum = 0
    with open("output/ER_digraph_" + sys.argv[1] + "n_varp.txt", 'r') as f:
        lines = f.readlines()
        for line in lines:
            p, nb_scc, nb_proportion = line.split(" ")
            p = float(p)
            nb_scc = float(nb_scc)
            nb_proportion = float(nb_proportion)
            xs.append(p)
            ys_loc.append(nb_scc)
            proportions.append(nb_proportion)
            # Maximum will be the average of p who get maximum partitions.
            if (nb_scc > maximum):
                maximums = [p]
                maximum = nb_scc
            elif (nb_scc == maximum):
                maximums.append(p)

    # sort in increasing order with xs
    xs, ys = zip(*sorted(zip(xs, ys_loc)))
    ys_loc, proportions = zip(*sorted(zip(ys_loc, proportions)))

    plt.plot(xs, ys)
    plt.xlim(0, 0.1)
    plt.xlabel("p")
    plt.ylabel("Number of partitions with size > 1")
    plt.title("Diagram p-number of scc (n = {}).\n Maximum found around {} with {} partitions.".format(sys.argv[1], sum(maximums) / len(maximums), maximum))

    with open("scc-p/result_n-p.txt", "a") as f:
        f.write("\n" + str(sys.argv[1]) + " " + str(sum(maximums) / len(maximums)) + " " + str(maximum) + " " + str(proportions[-1]))

    # Uncomment it if you want to save figure
    #plt.savefig("scc-p/" + sys.argv[1] + ".png")
    #plt.show()
    return 0


if __name__ == '__main__':
    show()

import sys
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import numpy as np

# Read coordinates of the 2D graph
def read_coordinates():
    nb_points = 0
    xs = []
    ys = []
    with open(sys.argv[1], "r") as f:
        lines = f.readlines()
        for line in lines:
            nb_points += 1
            x, y = line.split(sep=' ')
            xs.append(float(x))
            ys.append(float(y))

    xs = np.array(xs)
    ys = np.array(ys)
    return nb_points, xs, ys

# Read edge list of the 2D graph
def read_edges():
    froms = []
    tos = []
    with open(sys.argv[2], "r") as f:
        lines = f.readlines()
        for line in range(len(lines)):
            from_loc, to_loc = lines[line].split(sep=' ')
            froms.append(int(from_loc))
            tos.append(int(to_loc))
    return froms, tos

# Read the partitions of the 2D graph (dbscan or scc for str_type)
def read_partitions(str_type):
    index = 0;
    dic = {}
    with open("output/graph_2D_" + str_type + ".txt", "r") as f:
        lines = f.readlines()
        for line in lines:
            partition = int(line)
            if partition in dic.keys():
                dic[partition].append(index)
            else:
                dic[partition] = [index]
            index += 1
    # Delete clusters with size = 1
    number = 0
    for represent in dic.keys():
        if len(dic[represent]) < 2:
            dic[represent] = []
        else:
            number += 1

    max_nb = 0
    for represent in dic.keys():
        max_nb = max(max_nb, len(dic[represent]))
    print("Max partition/cluster ({}) : {}".format(str_type, max_nb))
    print("Number of clusters : {}".format(number))
    return dic

# Draw
def draw(nb_points, xs, ys, froms, tos, partition_dic, fig):
    # Check if point is not noise (cluster size < 2)
    points = []
    for represent in partition_dic.keys():
        for point in partition_dic[represent]:
            points.append(point)

    if len(points) == 0:
        print("No cluster detected!")
        return 1

    # Print edges
    for edge in range(len(froms)):
        from_loc = froms[edge]
        to_loc = tos[edge]
        if from_loc in points and to_loc in points:
            fig.arrow(xs[from_loc], ys[from_loc], xs[to_loc] - xs[from_loc], ys[to_loc] - ys[from_loc], color='m', alpha=0.1)

    # Print points
    color_max = len(partition_dic.keys())
    color_index = 0
    for represent in partition_dic.keys():
        r, g, b, a = cm.BrBG(color_index / color_max)
        for point in partition_dic[represent]:
            fig.scatter(xs[point], ys[point], c=np.array([[r, g, b, a]]), zorder=2)
        color_index += 1

    # Set limits :
    fig.set_xlim(np.min(xs) - 0.1, np.max(xs) + 0.1)
    fig.set_ylim(np.min(ys) - 0.1, np.max(ys) + 0.1)

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print('Usage: python graph_2D_plot.py coords.txt liste_edges.txt')
        sys.exit(1)

    _, (fig_dbscan, fig_scc) = plt.subplots(2)

    str_type = "dbscan"
    print("Printing {}".format(str_type))
    nb_points, xs, ys = read_coordinates()
    froms, tos = read_edges()
    partition_dic = read_partitions(str_type)
    draw(nb_points, xs, ys, froms, tos, partition_dic, fig_dbscan)
    fig_dbscan.set_title("dbscan")

    str_type = "scc"
    print("Printing {}".format(str_type))
    partition_dic = read_partitions(str_type)
    draw(nb_points, xs, ys, froms, tos, partition_dic, fig_scc)
    fig_scc.set_title("scc")
    plt.savefig("output/graph_2D.png")
    plt.show()

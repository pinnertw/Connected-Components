import networkx as nx
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import sys # For argument reading.

color_general = ['r', 'y', 'c', 'm', 'g']

def read_colors(str_type):
    # Read str_type file
    scc_name = sys.argv[1].replace("edges", str_type)
    index = 0;
    dic = {}
    with open(scc_name, 'r') as f:
        lines = f.readlines()
        for line in lines:
            partition = int(line)
            if partition in dic.keys():
                dic[partition].append(index)
            else:
                dic[partition] = [index]
            index += 1
    # Find max cluster/partitions. Print 5 max cluster/partiitons
    nb_partitions = []
    max_nb = 0
    for represent in dic.keys():
        max_nb = max(max_nb, len(dic[represent]))
        nb_partitions.append(len(dic[represent]))
    print("Max partition/cluster ({}) : {}".format(str_type, max_nb))
    nb_partitions.sort(reverse=True)
    print("Number of nodes fo the 5 biggest partitions : " + " ".join(map(str, (nb_partitions[:5]))))
    # If the number of partitions with size larger than 1 is less than 5 then print with color grcmy
    try:
        colorful = (nb_partitions[5] == 1)
    except IndexError: # nb_partitions < 5
        colorful = True

    return dic, colorful

def show_graph(g, pos, partition_dic, colorful, str_type):
    n = g.number_of_nodes()
    plt.figure(figsize=(30, 30))
    # Read color for points
    color_max = len(partition_dic.keys()) 
    # Different color for different clusters/partition
    color_index = 0
    color_general_index = 0

    # Blue edges
    nx.draw_networkx_edges(g, pos, edge_color='b', alpha=0.3)

    for represent in partition_dic.keys():
        # if it's a little cluster/partition -> more transparency
        if len(partition_dic[represent]) > 10:
            alpha = 0.8
        else:
            alpha = 0.1

        # If there are no more than 5 partitions with size bigger than 1 then print with colorful layout
        if colorful and len(partition_dic[represent]) > 1:
            color = color_general[color_general_index]
            color_general_index += 1
            alpha = 0.8
        else:
            color = cm.BrBG(color_index / color_max) 


        # If type = dbscan -> the cluster with 1 point will not be drawn
        if str_type != "dbscan" or len(partition_dic[represent]) > 2:
            for point in partition_dic[represent]:
                try:
                    nx.draw_networkx_nodes(g,
                            pos,
                            nodelist=[point],
                            node_color=[color],
                            node_size=100,
                            alpha=alpha)
                except nx.exception.NetworkXError:
                    continue

        color_index += 1

    # Figure settings
    plt.axis('off')
    plt.show(block=False)
    plt.savefig(sys.argv[1].replace("edges.txt", str_type + ".png"))
    return 0

def show():
    print("\nDrawing partitions/clusters...")
    if len(sys.argv) == 1 or len(sys.argv) > 3:
        print("Usage : python plot.py ../output/data_name_edges.txt, (mode)")
        return 1
    print("Reading file...")
    g = nx.read_edgelist(sys.argv[1], create_using=nx.DiGraph(), nodetype=int) # Read edge list
    print(nx.info(g))

    print("Calculating the graph...")
    # Calculate positions for nodes
    if len(sys.argv) == 2 or "scc" in sys.argv[2]:
        scc_dic, colorful = read_colors("scc")
    
    if len(sys.argv) == 2 or "dbscan" in sys.argv[2]:
        dbscan_dic, colorful = read_colors("dbscan")

    # Plot
    pos=nx.spring_layout(g) # Positions for all nodes
    if len(sys.argv) == 2 or "scc" in sys.argv[2]:
        print("Drawing scc...")
        if show_graph(g, pos, scc_dic, colorful, "scc") != 0:
            return 1
    if len(sys.argv) == 2 or "dbscan" in sys.argv[2]:
        print("Drawing dbscan...")
        if show_graph(g, pos, dbscan_dic, colorful, "dbscan") != 0:
            return 1
    plt.show()
    return 0


if __name__ == '__main__':
    show()

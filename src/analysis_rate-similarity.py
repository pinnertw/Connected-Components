import matplotlib.pyplot as plt
import numpy as np
import sys

def show():
    plt.figure(figsize=(10, 10))
    print("Reading file...")
    rates = []
    ers = []
    fars = []
    drs = []
    fss = []
    ps = []

    # Read file
    with open(sys.argv[1], 'r') as f:
        lines = f.readlines()
        for line in lines:
            rate, er, far, dr, fs, p= [float(s) for s in line.split(" ")]

            rates.append(rate)
            ers.append(er)
            fars.append(far)
            drs.append(dr)
            fss.append(fs)
            ps.append(p)

    # Error rate
    plt.subplot("511")
    plt.plot(rates, ers)
    plt.ylabel("Error rate")
    plt.xticks([])
    plt.title("Diagram rate - Error rate")

    # False alarm rate
    plt.subplot("512")
    plt.plot(rates, fars)
    plt.ylabel("False alarm rate")
    plt.xticks([])
    plt.title("Diagram rate - False alarm rate")

    # Detection rate
    plt.subplot("513")
    plt.plot(rates, drs)
    plt.ylabel("Detection rate")
    plt.xticks([])
    plt.title("Diagram rate - Detection rate")

    # F1 score
    plt.subplot("514")
    plt.plot(rates, fss)
    plt.ylabel("F1-score")
    plt.xticks([])
    plt.title("Diagram rate - F1score")

    # Precision
    plt.subplot("515")
    plt.plot(rates, ps)
    plt.xlabel("Rate\n" + sys.argv[1].replace("output/", "").replace("_dbscan_find-rate.txt", ""))
    plt.ylabel("Precision")
    plt.title("Diagram rate - Precision")

    # Output figure
    outputfilename = sys.argv[1].replace("output", "rate-sim")
    outputfilename = outputfilename.replace(".txt", ".png")
    plt.savefig(outputfilename)

    plt.show()
    return 0


if __name__ == '__main__':
    show()

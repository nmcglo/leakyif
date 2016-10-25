import numpy as np
import matplotlib.pyplot as plt



def plotRaster(inputCSVFileName):
    fig = plt.figure()
    csv = np.genfromtxt(inputCSVFileName, delimiter=",")

    x = np.nonzero(csv)[0]
    y = np.nonzero(csv)[1]

    plt.scatter(y,x,s=2)


    plt.ylim(-.1,len(csv))
    plt.xlim(0,len(csv[0]))
    plt.title("Neurons Fired")
    plt.xlabel("Time t")
    plt.ylabel("Neuron #")

    fig.savefig("firings.png")


def plotVHistory(inputCSVFileName):
    fig = plt.figure()
    csv = np.genfromtxt(inputCSVFileName, delimiter=",")

    csv = csv[-1][:]

    plt.plot(csv)
    fig.savefig("Vhistory.png")


plotRaster("firings.csv")
plotVHistory("vh.csv")

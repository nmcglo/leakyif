import numpy as np
import matplotlib.pyplot as plt



def plotRaster(inputCSVFileName):
    fig = plt.figure()
    csv = np.genfromtxt(inputCSVFileName, delimiter=",")

    x = np.nonzero(csv)[0]
    y = np.nonzero(csv)[1]

    plt.scatter(y,x,s=2)
    fig.savefig("firings.png")


def plotVHistory(inputCSVFileName):
    fig = plt.figure()
    csv = np.genfromtxt(inputCSVFileName, delimiter=",")

    plt.plot(csv)
    fig.savefig("Vhistory.png")


plotRaster("tmp.csv")

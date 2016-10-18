#Graph Parser V1

import numpy as np

weightMultiplier = 30


def parseGraphInput(inputFilename):

    infile = open(inputFilename, 'r')

    print("Parsing the input file...")
    params = []
    with open('input.txt', 'r') as f:
        for line in f:
            s = line.split()
            param = list(map(int,(s[1].split(','))))
            params.append(param)

    print("Parameters: ",params)
    Layers = params[0][0]
    Populations = np.array(params[1])

    InputIDs = np.zeros(Populations[0])
    for i in range(0,Populations[0]):
        InputIDs[i] = i

    print("Layers: ", Layers)
    print("Populations:", Populations)
    print("InputIDs:", InputIDs)

    TotalNodes = sum(Populations)
    print("Total Nodes: ",TotalNodes)

    W = np.zeros((TotalNodes,TotalNodes))
    # print(W)
    print("Layers:")
    for p in range(0,len(Populations)):
        if(p==0):
            startndx = 0
            endndx = sum(Populations[0:p+1]) -1
        else:
            startndx = sum(Populations[0:p])
            endndx = sum(Populations[0:p+1]) -1

        print(p,"\tstartndx:", startndx, "endndx:", endndx)
        for i in range(startndx,endndx+1):
            # print(i)
            for j in range(endndx+1,sum(Populations[0:p+2])):
                # print("\t",i,j)
                W[i,j] = np.random.randn() * .5 + 1


    print("\nInteraction Matrix:")
    print(W)
    #
    # for i in range(0,np.shape(W)[0]):
    #     for j in range(0,np.shape(W)[1]):
    #         W[i,j] = W[i,j] * np.random.randn()
    #
    # print("\nInteraction Matrix:")
    # print(W)

    return TotalNodes, Layers, Populations, InputIDs, W

import sys
import numpy as np
import matplotlib.pyplot as plt
import graphParser as gp


RandomWeightFlag = False
AllToAllFlag = False
NoInputLayerFlag = False
AllExitatoryFlag = False
RandomInputFiring = False


if(len(sys.argv) > 1):
    if "-W" in sys.argv:
        RandomWeightFlag = True
    if "-A" in sys.argv:
        AllToAllFlag = True
    if "-noInputLayer" in sys.argv:
        NoInputLayerFlag = True
    if "-E" in sys.argv:
        AllExitatoryFlag = True
    if "-randpre" in sys.argv:
        RandomInputFiring = True


def init():
    global N, Layers, Populations, InputIDs, Weights, times, T, tstep, refractory_length, Rmem, Cmem, tau_m, Vthresh, V_spike, J_0, Vm, Fired, periods, Ibias
    # global N


    np.random.seed(1)
    T = 1000
    tstep = 1
    refractory_length = 30
    times = np.arange(0,T,tstep)

    # same for all neurons
    Rmem = 10
    Cmem = 25
    tau_m = Rmem*Cmem
    Vthresh = 1.6
    V_spike = .25

    J_0 = 50

    # Current Inputs
    Ibias = 1.5

    N, Layers, Populations, InputIDs, Weights = gp.parseGraphInput('input.txt')

    if AllToAllFlag is True:
        if RandomWeightFlag is False:
            Weights = np.zeros((N,N)) + (J_0/N)
        else:
            Weights = np.random.randn(N,N)/N

    if NoInputLayerFlag is True:
        InputIDs = range(0,N)
        # Weights[0:int(N*2/3),:] = abs(Weights[0:int(N*2/3),:])  #Exitatory - maybe buggy
        # Weights[int(N*2/3):N,:] = -abs(Weights[int(N*2/3):N,:]) #Inhibiatory - maybe buggy

    if AllExitatoryFlag is True:
        Weights = abs(Weights)


    Vm = np.zeros((N,len(times)))
    Fired = np.zeros((N,len(times))) #zero if not, 1 if did

    # randomShuffleOfIds = np.arange(0,N)
    # np.random.shuffle(randomShuffleOfIds)
    # toBeFiredFirst = randomShuffleOfIds[0:int(N/10)]
    #
    # for index in toBeFiredFirst:
    #     Fired[index,0] =1

    periods = np.arange(.1,2,.1)
    np.random.shuffle(periods)


def raster(EventMatrix, color='k'):
    fig = plt.figure()
    x = np.nonzero(EventMatrix)[0]
    y = np.nonzero(EventMatrix)[1]

    plt.scatter(y,x,s=2)
    return fig


def getExternalCurrentAtTime(t):

    funperiods = []
    numPeriods = 4
    for i in range(0,numPeriods):
        funperiods.append(periods[i])
        # funperiods[i] = periods[i]

    output = 0
    for P in funperiods:
        output += np.sin(P*t)/numPeriods

    return output + Ibias

def getInputCurrentAtTime(t,nodeid):
    #TODO this currently does the "did you fire right now" approach (dirac): generalize it.
    firedForNow = Fired[:,t-1]
    indicesICareAbout = set(np.nonzero(Weights[:,nodeid])[0])
    indicesOfFired = set(np.nonzero(firedForNow)[0])

    firedIndicesICareAbout = list(indicesICareAbout & indicesOfFired)

    # if(len(firedIndicesICareAbout) > 0):
        # print(t, firedIndicesICareAbout)

    weightSum = 0
    if(len(firedIndicesICareAbout) > 0):
        for j in firedIndicesICareAbout:
            weightSum += Weights[j,nodeid]
        print('Node %i: weights of fired neurons is %f'%(nodeid, weightSum))



    if nodeid in InputIDs:
        Iext = getExternalCurrentAtTime(t)
    else:
        Iext = 0


    return weightSum + Iext

def isFiredDecision():
    chanceOfFiringEachTimestep = .5/N

    rn = np.random.rand()
    if rn > 1 - chanceOfFiringEachTimestep:
        return True
    else:
        return False


if __name__ == '__main__':
    print("Leaky Integrate and Fire Neural Model Simulation - Alpha\n")

    init()

    print("Final Weights\n", Weights)

    print("Beginning Simulation...")


    if RandomInputFiring is False:
        t_rest = np.random.rand(N)*refractory_length
    else:
        t_rest = np.zeros(N)

    for i in range(0,len(t_rest)):
        t_rest[i] = int(t_rest[i])
    for i in range(1, len(times)-1):
        t = times[i]

        #put the loop for per node here
        for n in range(0,N): #for each node n in N
            if n in InputIDs and RandomInputFiring:
                #Do random firing stuff here: Fired[n,t] = some random choice of 0 or 1 if t > t_test[n]
                if t > t_rest[n]:
                    toFire = isFiredDecision()
                    if toFire is True:
                        Fired[n,t] = 1
                        t_rest[n] = t + np.random.rand()*refractory_length
            else:
                #Not an input node (or RandomINputFiring is not flagged), thus do traditional integrating and firing
                if t > t_rest[n]:
                    Vm[n,t] = Vm[n,i-1] + ((-Vm[n,t-1] + getInputCurrentAtTime(t,n)*Rmem) / tau_m) #* timestep
                if Vm[n,t] >= Vthresh:
                    print("Node %i Fired at time %i!" % (n, t))
                    Vm[n,t] = Vm[n,t] + V_spike
                    Fired[n,t] = 1
                    t_rest[n] = t + np.random.rand()*refractory_length





    print("Simulation Complete, generating graph...")
    fig1 = raster(Fired)

    plt.ylim(-.1,N)
    plt.xlim(0,T)
    plt.title("Neurons Fired")
    plt.xlabel("Time t")
    plt.ylabel("Neuron #")

    fig2 = plt.figure()
    plt.plot(Vm[N-1,:])

    plt.show()



    print("Exiting...")

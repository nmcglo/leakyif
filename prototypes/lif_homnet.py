import sys
import numpy as np
import matplotlib.pyplot as plt

N = 100
T = 1000
tstep = 1
refractory_length = 100
times = np.arange(0,T,tstep)

# same for all neurons
Rmem = 1
Cmem = 10
tau_m = Rmem*Cmem
Vthresh = 2.3
V_spike = .5

J_0 = 1

#Data Matrices

Weights = np.zeros((N,N)) + (J_0/N)
# Weights = np.random.randn(N,N)
# Weights[0:int(N*2/3),:] = abs(Weights[0:int(N*2/3),:])  #Exitatory - maybe buggy
# Weights[int(N*2/3):N,:] = -abs(Weights[int(N*2/3):N,:]) #Inhibiatory - maybe buggy


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
    x = np.nonzero(EventMatrix)[0]
    y = np.nonzero(EventMatrix)[1]

    plt.scatter(y,x,s=2)

# Current Inputs
Ibias = 1.7
def getExternalCurrentAtTime(t):

    multiplier = np.random.rand()*5 + 1

    funperiods = []
    numPeriods = 5
    for i in range(0,numPeriods):
        funperiods.append(periods[i])
        # funperiods[i] = periods[i]

    output = 0
    for P in funperiods:
        output += np.sin(P*t)/numPeriods

    return output*multiplier + Ibias

def getInputCurrentAtTime(t,i):
    #TODO this currently does the "did you fire right now" approach (dirac): generalize it.
    FiredAtNow = Fired[:,t-1]
    indicesOfFired = np.nonzero(FiredAtNow)
    indicesOfFired = indicesOfFired[0]

    weightSum = 0
    if(len(indicesOfFired) > 0):
        for j in indicesOfFired:
            weightSum += Weights[i,j]

    Iext = getExternalCurrentAtTime(t)

    return weightSum + Iext


if __name__ == '__main__':

    t_rest = np.zeros(N)
    for i in range(1, len(times)-1):
        t = times[i]

        #put the loop for per node here
        for n in range(0,N):
            if t > t_rest[n]:
                Vm[n,i] = Vm[n,i-1] + ((-Vm[n,i-1] + getInputCurrentAtTime(t,n)*Rmem) / tau_m) #* timestep
            if Vm[n,i] >= Vthresh:
                Vm[n,i] = Vm[n,i] + V_spike
                Fired[n,i] = 1
                t_rest[n] = t + refractory_length


    raster(Fired)
    plt.ylim(0,N)
    plt.xlim(0,T)
    plt.show()

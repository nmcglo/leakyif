import sys
import numpy as np
import matplotlib.pyplot as plt

VARIABLE_CURRENT = True
N = 100
T = 1000
tstep = 1
refractory_length = 10
times = np.arange(0,T,tstep)

Rmem = 1
Cmem = 10
tau_m = Rmem*Cmem
Vthresh = 2
V_spike = .5

# Current Inputs
Ibias = 2.2
def getCurrentAtTime(t):
    Ibias = 2.2
    periods = np.arange(.1,2,.1)
    np.random.shuffle(periods)

    funperiods = []
    for i in range(0,5):
        funperiods.append(periods[i])
        # funperiods[i] = periods[i]

    output = 0
    for P in funperiods:
        output += np.sin(P*t)

    return output + Ibias



if __name__ == '__main__':
    if len(sys.argv) > 1:
        if sys.argv[1] == '-c':
            VARIABLE_CURRENT = False

    Vm = np.zeros(N,len(times))
    t_rest = 0
    for i in range(0, len(times)):
        t = times[i]

        if t > t_rest:
            if VARIABLE_CURRENT == True:
                Vm[i] = Vm[i-1] + ((-Vm[i-1] + getCurrentAtTime(t)*Rmem) / tau_m) * tstep
            else:
                Vm[i] = Vm[i-1] + ((-Vm[i-1] + Ibias*Rmem) / tau_m) * tstep
        if Vm[i] >= Vthresh:
            Vm[i] = Vm[i] + V_spike
            t_rest = t + refractory_length

    plt.plot(times, Vm)
    plt.show()

import sys
import numpy as np
import matplotlib.pyplot as plt

VARIABLE_CURRENT = False
T = 100
tstep = 1
refractory_length = 10
times = np.arange(0,T,tstep)

Rmem = 1
Cmem = 10
tau_m = Rmem*Cmem
Vthresh = 2
V_spike = .5

# Current Inputs
def myfun(x):
    Ibias = 2.2
    periods = np.arange(.1,2,.1)
    np.random.shuffle(periods)

    funperiods = []
    for i in range(0,5):
        funperiods.append(periods[i])
        # funperiods[i] = periods[i]

    output = 0
    for P in funperiods:
        output += np.sin(P*x)

    return output + Ibias



if __name__ == '__main__':
    if sys.argv[1] == '-v':
        VARIABLE_CURRENT = True

    Vm = np.zeros(len(times))
    t_rest = 0
    for i in range(0, len(times)):
        t = times[i]

        if t > t_rest:
            Vm[i] = Vm[i-1] + (-Vm[i-1] + myfun(t)*Rmem) / tau_m * tstep
        if Vm[i] >= Vthresh:
            Vm[i] = Vm[i] + V_spike
            t_rest = t + refractory_length

    plt.plot(times, Vm)
    plt.show()

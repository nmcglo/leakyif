clear;

VARIABLE_CURRENT = true;


T = 100; %total timesteps in simulation
tstep = 1; %timestep size
refractory_length = 10; %waiting period after a spike before integrating further
times = 0:tstep:T; %Possible times in the simulation


Rmem = 1; %The membrane resistance
Cmem = 10; %The membrande capacitance
tau_m = Rmem*Cmem; %The time constant
Vthresh = 2; %The Threshold voltage at which the neuron spikes
V_spike = .5; %How much added voltage once the neuron spikes

%Current inputs
Ibias = 2.2;
if (VARIABLE_CURRENT == true)
    periods = randsample(.1:.1:2,5)
    f1 = @(x) sin(periods(1) * x);
    f2 = @(x) sin(periods(2) * x);
    f3 = @(x) sin(periods(3) * x);
    f4 = @(x) sin(periods(4) * x);
    f5 = @(x) sin(periods(5) * x);

    I = @(x) (f1(x) + f2(x) + f3(x) + f4(x) + f5(x)) + Ibias;   
else
    I(1:length(times)) = Ibias; 
end



Vm = zeros(1,length(times));
t_rest = 0;
for i = 1:1:length(times)
    t = times(i);
    
    if t > t_rest
        Vm(i) = Vm(i-1) + (-Vm(i-1) + I(i)*Rmem) / tau_m * tstep; 
    end
    if Vm(i) >= Vthresh
        Vm(i) = Vm(i) + V_spike;
        t_rest = t + refractory_length;
    end
    
    
end

plot(times, Vm)
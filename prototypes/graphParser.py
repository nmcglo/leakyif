infile = open('input.txt', 'r')

print("Parsing the input file...")
params = []
with open('input.txt', 'r') as f:
    for line in f:
        s = line.split()
        param = list(map(int,(s[1].split(','))))
        params.append(param)

print("Parameters: ",params)
Layers = params[0][0]
Populations = params[1]
print("Layers: ", Layers)
print("Populations:", Populations)

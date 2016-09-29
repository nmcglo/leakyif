import numpy as np
import numpy.ctypeslib as npct
import ctypes

x = np.random.rand(3,3)
print("Generated Random Array: \n", x)
xc = npct.as_ctypes(x)


f = open("testFile.dat",'wb')
f.write(xc)
f.close()

import numpy as np
import numpy.ctypeslib as npct
import ctypes
import struct

n = 5;

x = np.random.rand(n,n)
print("Generated Random Array: \n", x)
xc = npct.as_ctypes(x)


f = open("testFile.dat",'wb')
f.write(struct.pack("i",n))
f.write(xc)
f.close()

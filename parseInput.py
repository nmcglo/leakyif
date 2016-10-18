import numpy as np
import numpy.ctypeslib as npct
import ctypes
import struct
import graphParserV1 as gp


TotalNodes, Layers, Populations, InputIDs,W = gp.parseGraphInput('input.txt')

f = open("generatedInput.dat",'wb')
f.write(struct.pack("i",TotalNodes)) #write int
f.write(struct.pack("i",len(InputIDs))) #write int
InputIDsCtype = npct.as_ctypes(InputIDs)
f.write(InputIDsCtype) #write Matrix
WCtype = npct.as_ctypes(W)
f.write(WCtype) #write Matrix

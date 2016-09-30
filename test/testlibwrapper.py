import ctypes
import numpy as np
from numpy.ctypeslib import ndpointer

testlib = ctypes.CDLL('/Users/shiv/Dropbox/slaterrules/slater_in_c/test/testlib.so')

bitstring = ctypes.POINTER(ctypes.c_uint64)
arrbitstring = ctypes.POINTER(bitstring)
a_alpha = np.array([00011111,00011111],dtype=np.dtype('u8'))
b_alpha = np.array([00011111,00011111],dtype=np.dtype('u8'))
a_beta = np.array([00101111,00011111],dtype=np.dtype('u8'))
b_beta = np.array([00011111,00011111],dtype=np.dtype('u8'))
print "particle 1 alpha passed :", a_alpha
print "particle 1 beta passed :", a_beta
print "particle 2 alpha passed :", a_alpha
print "particle 2 beta passed :", a_beta
c = np.array([1],dtype=np.dtype('u8'))
d = np.array([0],dtype=np.dtype('u8'))
testlib.n_excitations.restype = None
testlib.n_excitations.argtypes = [
np.ctypeslib.ndpointer(ctypes.c_uint64 , ndim=1, flags="C_CONTIGUOUS"),
np.ctypeslib.ndpointer(ctypes.c_uint64 , ndim=1, flags="C_CONTIGUOUS"),
np.ctypeslib.ndpointer(ctypes.c_uint64 , ndim=1, flags="C_CONTIGUOUS"),
np.ctypeslib.ndpointer(ctypes.c_uint64 , ndim=1, flags="C_CONTIGUOUS"),
np.ctypeslib.ndpointer(ctypes.c_uint64, ndim=1, flags="C_CONTIGUOUS"),
np.ctypeslib.ndpointer(ctypes.c_uint64, ndim=1, flags="C_CONTIGUOUS")
]
testlib.n_excitations(a_alpha,a_beta,b_alpha,b_beta,c,d)
#testlib.n_excitations(ctypes.c_uint64(a.ctypes.data),ctypes.c_uint64(b.ctypes.data),ctypes.c_uint64(c.ctypes.data),ctypes.c_uint64(d.ctypes.data))
print d

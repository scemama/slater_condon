import ctypes
import numpy as np
from numpy.ctypeslib import ndpointer

testlib = ctypes.CDLL('/Users/shiv/Dropbox/slaterrules/slater_in_c/test/testlib.so')

bitstring = ctypes.POINTER(ctypes.c_uint64)
arrbitstring = ctypes.POINTER(bitstring)
a_alpha = np.random.randint(low =4,high =10, size = (2),dtype=np.dtype('u8'))
b_alpha = np.random.randint(low =4,high =10, size = (2),dtype=np.dtype('u8'))
a_beta = np.random.randint(low =4,high =10, size = (2),dtype=np.dtype('u8'))
b_beta = np.random.randint(low =4,high =10, size = (2),dtype=np.dtype('u8'))
print a_alpha
c = np.array([1],dtype=np.dtype('u8'))
d = np.array([0],dtype=np.dtype('u8'))
print 'shit', c,d
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

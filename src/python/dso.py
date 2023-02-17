from ctypes import cdll
import sys


class DSO:

    # -------------------------------------------------------------------------
    # call with dll name: get the C++ functions, none is called
    # -------------------------------------------------------------------------
    def __init__(self, soname):
        print('<DSO Python Load>')

        # load library
        self.lib = cdll.LoadLibrary(soname)

    def __del__(self):
        print('<DSO Python Quit>')


print(sys.argv)
if (len(sys.argv) >= 2):
    print("dll=", sys.argv[1])
    dso = DSO(sys.argv[1])
    

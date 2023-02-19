from ctypes import cdll
#import sys
import platform
import os.path


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


print(platform.machine())
#print(platform.platform(terse=True))
print(platform.system())

# print(sys.argv)
# if (len(sys.argv) >= 2):
#     print("dll=", sys.argv[1])
#     dso = DSO(sys.argv[1])

soname = "./dsoxx.dll"
if os.path.isfile(soname):
    dso = DSO(soname)

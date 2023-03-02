#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Feb 27 15:24:02 2023

@author: ybouret
"""

from ctypes import *
from engine import Engine
import numpy as np

print('-- creating engine')
engine = Engine("./engine.dll")

print('-- initializing engine')
if not engine.WasInit():
    engine.Init(4)

print('-- computing')
arr = np.array([1, 2, 3.2], dtype=np.float64)
ave = engine.Average(arr, arr.size)
print('average',ave)

rng = np.random.default_rng()

arr  = rng.standard_normal(10000,dtype=np.float64)
ave0 = engine.Average(arr, arr.size)
print('average0',ave0)

ave1 = engine.AverageMP(arr, arr.size)
print('average1',ave0)

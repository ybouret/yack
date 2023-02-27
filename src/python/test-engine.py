#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Feb 27 15:24:02 2023

@author: ybouret
"""

from ctypes import *
from engine import Engine
import numpy as np

engine = Engine("./engine.dll")

if not engine.WasInit():
    engine.Init(4)

arr = np.array([1, 2, 3.2], dtype=np.float64)
ave = engine.Average(arr, arr.size)
print('average',ave)

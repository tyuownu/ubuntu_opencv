#!/usr/bin/env python
# change_from_octave.py import this file
import numpy as np
def s_alpha_beta(p1,p2,x,y):
	sum = 0
	len = max(p1.shape[0],p1.shape[1])
	for _ in np.arange(len):
		sum += p1[y,_]*p2[x,_]
	return sum
#-------function-*distance----
def dist(p1,p2):
	r = (p1[0] - p2[0])**2 + (p1[1] - p2[1])**2 + (p1[2] - p2[2])**2
	return r**0.5

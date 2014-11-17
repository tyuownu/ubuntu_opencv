#!/usr/bin/env python

import numpy as np
import numpy.linalg as nplg
from function import s_alpha_beta
from function import dist
import sys

p11 = np.loadtxt('/home/tyu/ubuntu_opencv/result/p1.txt',delimiter=' ')
p22 = np.loadtxt('/home/tyu/ubuntu_opencv/result/p2.txt',delimiter=' ')
p1 = p11.transpose()
p2 = p22.transpose()
if max(p1.shape[0],p2.shape[1])<10:
	print 'not enough keypoints'
	sys.exit()


#RTlist = np.zeros([3,4 * max(p1.shape[0],p2.shape[1])])
RTlist = np.zeros([3,100])




iterations = 0
distance = 100.
Error = 0.
while iterations < 100:
	num = 0
	np1 = np.zeros(p1.shape)
	np2 = np.zeros(p2.shape)
	for x in np.arange(p1.shape[1]):
		if dist(p1[:,x],p2[:,x]) < 2*distance:
			np1[:,num] = p1[:,x]
			np2[:,num] = p2[:,x]
			num += 1
	np1 = np1[:,0:num]
	np2 = np2[:,0:num]

	cp1 = np1.sum(axis = 1) / num
	cp2 = np2.sum(axis = 1) / num
	cp1.shape = (cp1.shape[0],1)
	cp2.shape = (cp2.shape[0],1)
	np1_ = np1 - cp1
	np2_ = np2 - cp2
	sxx = s_alpha_beta(np1_,np2_,0,0)
	sxy = s_alpha_beta(np1_,np2_,0,1)
	sxz = s_alpha_beta(np1_,np2_,0,2)
	syx = s_alpha_beta(np1_,np2_,1,0)
	syy = s_alpha_beta(np1_,np2_,1,1)
	syz = s_alpha_beta(np1_,np2_,1,2)
	szx = s_alpha_beta(np1_,np2_,2,0)
	szy = s_alpha_beta(np1_,np2_,2,1)
	szz = s_alpha_beta(np1_,np2_,2,2)

	N = np.array([[sxx + syy + szz,syz - szy      ,szx - sxz      ,sxy - syx],\
				   [syz - szy      ,sxx - syy - szz,sxy + syx      ,szx + sxz],\
				   [szx - sxz      ,sxy + syx      ,syy - szz - sxx,syz + szy],\
				   [sxy - syx      ,szx + sxz      ,syz + szy      ,szz - sxx - syy]])
	a,b = nplg.eig(N)
	for _ in np.arange(a.shape[0]):
		if a[_] > 0:
			break
	v = b[:,_]
	R = np.array([[v[0]**2+v[1]**2-v[2]**2-v[3]**2,2*(v[1]*v[2]-v[0]*v[3]),2*(v[1]*v[3]+v[0]*v[2])],\
		 		  [2*(v[1]*v[2]+v[0]*v[3]),v[0]**2-v[1]**2+v[2]**2-v[3]**2,2*(v[2]*v[3]-v[0]*v[1])],\
	 			  [2*(v[1]*v[3]-v[0]*v[2]),2*(v[2]*v[3]+v[0]*v[1]),v[0]**2-v[1]**2-v[2]**2+v[3]**2] ]);
	T = cp1 - np.dot(R,cp2)
	RTlist[:,[4*iterations,4*iterations+1,4*iterations+2]] = R
	RTlist[:,[4*iterations + 3]] = T.reshape(-1,1)
	np2_RT = np.dot(R,np2) + T
	for _ in np.arange(num):
		Error += dist(np1[:,_],np2_RT[:,_])
	Error = Error/num
	p2 = np.dot(R,p2) + T
	iterations += 1
	if np.abs(distance - Error) < 0.0001:
		distance = Error
		break
	distance = Error
print 'RTlist DONE.'
RTlist = RTlist[:,np.arange(4*iterations + 4)]
Rtemp = np.eye(3)
Ttemp = np.array([0,0,0])
Ttemp.shape = [3,1]
for _ in np.arange(iterations):
	a = RTlist[:,4*_+3]
	a.shape = [3,1]
	Ttemp =np.dot(RTlist[:,[4*_,4*_ + 1,4*_+2]],Ttemp) + a
	Rtemp = np.dot(RTlist[:,np.array([0,1,2]) + 4*_],Rtemp)
print 'Rtemp = '
print Rtemp
print 'Ttemp = '
print Ttemp
print 'Rtemp,Ttemp DONE.'
ply = np.loadtxt('/home/tyu/ubuntu_opencv/ply/image2_ty.ply',delimiter = ' ')
Ttemp.shape = [3,]
result_without_rgba = np.dot(ply,Rtemp.transpose()) + Ttemp
print 'result_without_rgba DONE.'



def change_from_octave(ply_file):
	f1 = open('/home/tyu/ubuntu_opencv/ply/image2.ply','r')
#	f = open('/home/tyu/ubuntu_opencv/ply/result.ply','w')
	number = 0
	result =[]
	while 1:
		line1 = f1.readline()
		if line1.startswith('p') or line1.startswith('e') or line1.startswith('f'):
			continue
		if not line1 or line1.startswith('\n'):
			break
		l1 = map(float,line1.split(' '))
		result.append('%f %f %f %d %d %d 0\n'%(result_without_rgba[number,0],result_without_rgba[number,1],result_without_rgba[number,2],l1[3],l1[4],l1[5]))
		number += 1
	f1.close()
	f = open(ply_file,'w')
	f.write('''ply
format ascii 1.0
element vertex %d
property float x
property float y
property float z
property uchar red
property uchar green
property uchar blue
property uchar alpha
end_header
%s
'''%(len(result),''.join(result)))
	f.close()
	print 'New result.ply DONE'



ply_file = sys.argv[1]
change_from_octave(ply_file)

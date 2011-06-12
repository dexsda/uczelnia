#!/usr/bin/python
import sys
def dda(ls):
	if len(ls)<=1:
		return None
	else:
		output=[]
		for i in xrange(len(ls)-1):
			output.append(ls[i+1]-ls[i])
		return output

def ddm(ls):
	if len(ls)<=1:
		return None
	else:
		output=[]
		for i in xrange(len(ls)-1):
			if ls[i]==0:
				output.append(0)
			else:
				output.append(ls[i+1]/ls[i])
		return output

def det_same(ls): #to dostaje ddx
	if len(ls)<=1:
		return false
	else:
		return all(x == ls[0] for x in ls)

def answer_part(ls,depth,maxdepth=2):
	if depth==maxdepth:
		return []
	else:
		ddals = dda(ls)
		ddmls = ddm(ls)
		if det_same(ddals):
			ls.append(ls[-1]+ddals[0])
		elif det_same(ddmls):
			ls.append(ls[-1]*ddmls[0])
		else:
			a=answer_part(ddals,depth+1,maxdepth)
			b=answer_part(ddmls,depth+1,maxdepth)
			if a!=[]:
				ls.append(ls[-1]+ddals[-1])
			elif b!=[]:
				ls.append(ls[-1]*ddmls[-1])
			else:
				ls=[]
		return ls

def answer(ls,maxdepth=2):
	if maxdepth>len(ls)-2:
		maxdepth=len(ls)-2
	ret = answer_part(ls,0,maxdepth)
	if ret!=[]:
		return ret
	elif ls[:-2]==dda(ls)[1:]:
		ls.append(ls[-2]+ls[-1])
		return ls
	else:
		return ret


if __name__=='__main__':
	#string=""
	ls=[]
	for i in sys.argv[1:]:
		ls.append(float(i))
	#ls = [float(x) for x in string.split(',')]
	print answer(ls,4)

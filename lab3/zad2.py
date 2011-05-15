#!/usr/bin/python2
def epsilon():
	eps=float(1)
	while float(1)+float(eps)!=float(1):
		eps_prev=eps
		eps=float(eps)/float(2)
	return eps_prev

class derivative(object):
	def __init__(self,f):
		self.f=f
		self.eps=epsilon()**(1/3.0)
	def __call__(self,x):
		return (self.f(x+self.eps)-self.f(x-self.eps))/(2*self.eps)

#def derivative(f):
#	def derive(x):
#		eps=epsilon()**(1/3.0)
#		return (f(x+eps)-f(x-eps))/(2*eps)
#	return derive

@derivative
def f(x):
	return x**2

print f(2.0)

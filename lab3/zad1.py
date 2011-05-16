#!/usr/bin/python2
class dependency(object):
	done_func=[]
	def __init__(self,*args):
		self.deps=[]
		for func in args:
			self.deps.append(func)
	def __call__(self,f):
		self.deps.append(f)
		def function(*args):
			for func in self.deps:
				if func not in self.done_func:
					func(*args)
				self.done_func.append(func)
		return function

def g():
	print 'funkcja g'
@dependency(g)
def f():
	print 'funkcja f'
@dependency(f,g)
def h():
	print 'funkcja h'
print "funkcja h:"
h()

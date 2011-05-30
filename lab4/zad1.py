import multiprocessing
import time
import math

def sigma(x,y):
	time.sleep(0.4)
	print '>>', x+y
	return x+y

def sigmasquared(x,y):
	return x+y*y

def pi(x,y):
	time.sleep(0.1)
	return x*y

def max(x,y):
	time.sleep(0.1)
	if y>x:
		return y
	else:
		return x

def splitlist(ls,parts):
	length=int(math.ceil(float(len(ls))/float(parts)))
	for i in xrange(0, len(ls), length):
		yield ls[i:i+length]

def reducethreaded(func, iterable, initializer=None,cores=4):
	pool=multiprocessing.Pool(cores)
	results=[]
	if initializer!=None:
		ls[0]=func(initializer,ls[0])
	for ls in list(splitlist(iterable,cores)):
		results.append(pool.apply_async(reduce,(func,ls),))
	while not all([ x.ready() for x in results]):
		#print [ x.ready() for x in results]
		pass
	#print [x.ready() for x in results]
	return reduce(func,[x.get() for x in results])

def stddevthreaded(ls):
	sum_sqr=ls[0]*reducethreaded(sigmasquared,ls)

if __name__=='__main__':
	ls=range(1,11)
	print "suma, bez multithreadingu: "
	wynik=reduce(sigma,ls)
	print "Wynik:",wynik
	print "suma, z multithreadingiem: "
	wynik=reducethreaded(sigma,ls,cores=5)
	print "Wynik:",wynik
	print "iloczyn, bez multithreadingu: "
	wynik=reduce(pi,ls)
	print "Wynik:",wynik
	print "iloczyn, z multithreadingiem: "
	wynik=reducethreaded(pi,ls,cores=5)
	print "Wynik:",wynik
	print "max, bez multithreadingu: "
	wynik=reduce(max,ls)
	print "Wynik:",wynik
	print "max, z multithreadingiem: "
	wynik=reducethreaded(max,ls,cores=5)
	print "Wynik:",wynik
	g=0.0
	print "srednia:",float(reducethreaded(sigma,ls))/len(ls)
	print "srednia:",reduce(av,ls)
	#print "Ready?: ", [ x.ready() for x in results]
	#while not all([ x.ready() for x in results]):
#		print [ x.ready() for x in results]
#		time.sleep(1)
#	print [ x.ready() for x in results]
#	print [ x.get() for x in results ]

#!/usr/bin/python
import weakref

class litera(object):
	_litery = weakref.WeakValueDictionary()
	def __new__(cls,glif):
		"""glify beda znakami na potrzeby przykladu"""
		obj = litera._litery.get(glif, None)
		if not obj:
			obj = object.__new__(cls)
			litera._litery[glif]=obj
			obj.glif=glif
			print "Robiony nowy obiekt (" + glif + ")"
		return obj
	def __str__(self):
		return self.glif

def elements():
	return str([a.glif for a in litera._litery.values()])
def length():
	return len([a.glif for a in litera._litery.values()])

if __name__=='__main__':
	znak1 = litera('y')
	znak2 = litera('a')
	znak3 = litera('y')
	print znak2
	print elements()
	znak2 = litera('y')
	print elements()
	znak2 = litera('a')
	znak1,znak2,znak3=None,None,None
	print elements()
	znaki = [litera(i) for i in "monty"] #bez powtorzen, 5 liter
	print elements(), "dlugosc: ", length()
	znaki = [] #czyscimy stringa
	print elements(), "dlugosc: ", length()
	znaki = [litera(i) for i in "we are the knights who say ni"] #z powtorzeniami, 29 liter
	for i in znaki:
		print i,
	print
	print elements(), "dlugosc: ", length()

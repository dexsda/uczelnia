#!/usr/bin/python

class component:
	def __init__(self):
		pass
	def printout(self):
		pass

class trojkat(component):
	def printout(self):
		print "ting"

class trabka(component):
	def printout(self):
		print "fruuuu"

class kobza(component):
	def printout(self):
		print "DUUUUUUUUUUU"

class skrzypce(component):
	def printout(self):
		print "skrzyyyyp"

class wiolonczela(component):
	def printout(self):
		print "SKRZYYYYP"

class sekcja(component):
	def __init__(self, *args):
		self.instruments=[]
		self.add(*args)
	def add(self,*args):
		for i in args:
			self.instruments.append(i)
	def remove(self,*args):
		for i in args:
			self.instruments.remove(i)
	def printout(self):
		for i in self.instruments:
			i.printout()

if __name__=='__main__':
	sekcja_deta=sekcja(trabka(),kobza(),kobza(),trabka(),kobza())
	sekcja_smyczkowa=sekcja(skrzypce(),skrzypce(),wiolonczela())
	sekcja_smyczkowa.add(skrzypce())
	orkiestra=sekcja(sekcja_deta,trojkat(),sekcja_smyczkowa,trojkat(),kobza())
	orkiestra.printout()


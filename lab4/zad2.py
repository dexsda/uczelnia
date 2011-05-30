#!/usr/bin/python2
# -*- coding: utf-8 -*-
import re
import string
import time
import math
import operator
def create_dictionaries(filename):
	print ">tworze slowniki..."
	altdict=dict()
	basedict=dict()
	f=open(filename,'r')
	for line in f:
		temp=line.rstrip().lower().split(', ')
		for i in temp[1:]:
			altdict[i]=temp[0]
		basedict[temp[0]]=0
	return altdict,basedict

def prepare_basedict(base,alt,filename):
	print ">przeliczam idf..."
	altdict=dict()
	f=open(filename,'r')
	art_sep=re.compile(r'#(\d{6})')
	line_spl=regex = re.compile(r'[%s\s\d]+' % re.escape(string.punctuation))
	words_in_art=[]
	docs=0
	for line in f:
		if art_sep.match(line):
			words_in_art=[]
			docs+=1
		else:
			temp=line_spl.split(line.rstrip().lower())
			for i in temp:
				if i not in alt and i not in words_in_art:
					if i in base:
						base[i]+=1
						words_in_art.append(i)
					else:
						words_in_art.append(i)
						base[i]=1
				elif i in alt and alt[i] not in words_in_art:
					base[alt[i]]+=1
					words_in_art.append(alt[i])
	docs-=1
	for i in base.keys():
		if base[i]==0:
			del base[i]
		else:
			base[i]=math.log10(float(docs)/(base[i]))
	return base

def top_rated(idf,alt,filename):
	print ">licze wskazniki..."
	f=open(filename,'r')
	art_sep=re.compile(r'#(\d{6})')
	line_spl=regex = re.compile(r'[%s\s\d]+' % re.escape(string.punctuation))
	words_in_art={}
	total_words=0
	for line in f:
		if art_sep.match(line) and total_words!=0:
			print "Art#",art
			art=art_sep.match(line).group(1)
			for u,v in words_in_art.items():
				if u=='':
					del words_in_art['']
				else:
					words_in_art[u]=(float(v)/float(total_words))*idf[u]
			for u,v in sorted(words_in_art.iteritems(), key=operator.itemgetter(1))[-1:-6:-1]:
				print u,":",v
			words_in_art={}
			total_words=0
		elif not art_sep.match(line):
			temp=line_spl.split(line.rstrip().lower())
			for i in temp:
				if i not in alt:
					if i not in words_in_art:
						words_in_art[i]=0
					words_in_art[i]+=1
					total_words+=1
				else:
					if alt[i] not in words_in_art:
						words_in_art[alt[i]]=0
					words_in_art[alt[i]]+=1
					total_words+=1
		else:
			art=art_sep.match(line).group(1)

if __name__=='__main__':
	altdictionary,basedictionary=create_dictionaries("odm.txt")
	#najpierw liczymy ilosc wystapien slow w kazdym z artykulow
	idf=prepare_basedict(basedictionary,altdictionary,"pap.txt")

	top_rated(idf,altdictionary,"pap.txt")

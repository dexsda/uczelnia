#!/usr/bin/python2
import sys
import re
import pprint

def parse(expr):
	expr=expr.translate(None,' ')
	regex=re.compile(r"(\d+)(?=[a-zA-Z]+)")
	expr=regex.sub(r"\1*", expr)
	regex=re.compile(r"(\w)(?=[a-zA-Z]+)")
	expr=regex.sub(r"\1*", expr)
	return expr

def tokenize(expr):
	tokens=[]
	tok=''
	for c in expr:
		if c.isdigit():
			tok=tok+c
		else:
			if tok!='':
				tokens.append(int(tok))
				tok=''
			tokens.append(c)
	return tokens

prec={'+':1,'-':1,'*':2,'(':8,')':0}

def make_tree(a,b,op):
	return [op,a,b]



def astree(tokens):
	operands=[]
	operators=[]
	for tok in tokens:
		if tok in prec.keys():
			while operators!=[] and prec[tok]<prec[operators[-1]]:
				op=operators.pop()
				x2=operands.pop()
				x1=operands.pop()
				operands.append(make_tree(a=x1,b=x2,op=op))
			operators.append(tok)
		else:
			operands.append(tok)
	while operators!=[]:
		op=operators.pop()
		x2=operands.pop()
		x1=operands.pop()
		operands.append(make_tree(a=x1,b=x2,op=op))
	return operands[0]
def print_tree(tree):
	return pprint.pformat(tree,indent=2)
def cat(tree):
	treecat=[]
	op=tree[0]
	for i in tree:
		if isinstance(i,list) and op!='-' and  op==i[0]:
			for j in i[1:]:
				treecat.append(j)
		else:
			if isinstance(i,list):
				i=cat(i)
			treecat.append(i)
	return treecat
def aggregate_test(tree):
	count=0
	op=tree[0]
	out=[]
	for i in tree:
		if type(i)==type(1):
			if op=='+':
				count+=1
			elif op=='*':
				count+=1
		else:
			if isinstance(i,list):
				i=aggregate_test(i)
		out.append(i)
	if count>=2:
		tree=out[:]
		others=[]
		if op=='+':
			count=0
		else:
			count=1
		for i in tree[1:]:
			if type(i)==type(1):
				if op=='+':
					count+=i
				else:
					count*=i
			else:
				others.append(i)
		out=[op,count]+others
	return out
def crunch(tree):
	copy=tree[:]
	tree=cat(tree)
	tree=aggregate_test(tree)
	if copy==tree:
		return copy
	else:
		return crunch(tree)

expr = sys.argv[1]
print "expr:", expr
tokens = tokenize(parse(expr))
print "tokenized:", tokens
tree = astree(tokens)
print "tree:\n", print_tree(tree)
tree=crunch(tree)
print "tree after crunch:\n", print_tree(tree)

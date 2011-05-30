#!/usr/bin/python2
import sys
import re
import pprint
import time

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
	if tok!='':
		tokens.append(int(tok))
	return tokens

prec={'+':1,'-':1,'*':2,'(':8}

def make_tree(a,b,op):
	return [op,a,b]

def astree(tokens):
	operands=[]
	operators=[]
	for tok in tokens:
		if tok in ('+','-','*','('):
			while operators!=[] and operators[-1]!='(' and prec[tok]<prec[operators[-1]]:
				op=operators.pop()
				x2=operands.pop()
				x1=operands.pop()
				operands.append(make_tree(a=x1,b=x2,op=op))
			operators.append(tok)
		elif type(tok)==type('') and tok in (')'):
			while operators!=[] and operators[-1]!='(':
				op=operators.pop()
				x2=operands.pop()
				x1=operands.pop()
				operands.append(make_tree(a=x1,b=x2,op=op))
			if operators==[] or operators[-1]!='(':
				print 'mismatched'
			else:
				operators.pop()
		else:
			operands.append(tok)
	while operators!=[]:
		if operators[-1]=='(':
			print 'mismatched paren'
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
def expand(tree):
	if tree[0]=='+':
		counter=0
		for i in tree[1:]:
			counter+=1
			if type(i)==type([]):
				if i[0]=='*' and type(i[1])!=type(1):
					i.insert(1,1)
			elif type(i)==type(''):
				tree[counter]=['*',1,i]
	return tree
def rem_minus(tree):
	op=tree[0]
	out=[]
	if op!='-':
		for i in tree:
			if isinstance(i,list):
				i=rem_minus(i)
			out.append(i)
	else:
		out.append('+')
		out.append(tree[1])
		out.append(['*',-1,tree[2]])
	return out
def reduce_similar(tree):
	out=[]
	out.append(tree[0])
	for i in xrange(1,len(tree)):
		for j in xrange(i+1,len(tree)):
			if type(tree[i])==type([]):
				tree[i]=reduce_similar(tree[i])
			if type(tree[j])==type([]):
				tree[j]=reduce_similar(tree[j])
			if tree[0]=='+' and type(tree[i])==type(tree[j]) and type(tree[i])==type([]) and tree[i][0]=='*' and tree[j][0]=='*':
				if sorted(tree[i][2:])==sorted(tree[j][2:]):
					tree[i][1]=tree[i][1]+tree[j][1]
					tree[j][1]=0
		if type(tree[i])!=type([]) or tree[i][0]!='*' or tree[i][1]!=0:
			out.append(tree[i])
	return out
def truncate(tree):
	if tree[0]=='+':
		counter=0
		for i in tree[1:]:
			counter+=1
			if type(i)==type([]):
				if i[0]=='*' and i[1]==1:
					i.remove(1)
				if len(i)==2:
					tree[counter]=i[1]
	if (tree[0]=='+' or tree[0]=='*') and len(tree)==2:
		tree=tree[1]
	return tree
def mul_lists(tree):
	out=[]
	if tree[0]=='*' and len(tree)==3 and type(tree[1])==type(tree[2]) and type(tree[1])==type([]):
		for i in tree[1][1:]:
			for j in tree[2][1:]:
				out.append(['*',i,j])
	elif tree[0]=='*' and len(tree)==3 and type(tree[1])==type([]):
		for i in tree[1][1:]:
			out.append(['*',tree[2],i])
	elif tree[0]=='*' and len(tree)==3 and type(tree[2])==type([]):
		for i in tree[2][1:]:
			out.append(['*',tree[1],i])
	else:
		out.append(tree[0])
		for i in tree[1:]:
			if type(i)==type([]):
				out.append(mul_lists(i))
			else:
				out.append(i)
	return out
def remove_useless_lists(tree):
	out=[]
	for i in tree:
		if type(i)==type([]) and len(i)==2:
			out.append(i[1])
		elif type(i)==type([]):
			out.append(remove_useless_lists(i))
		else:
			out.append(i)
	return out

def crunch(tree):
	copy=tree[:]
	tree=cat(tree)
	tree=aggregate_test(tree)
	tree=rem_minus(tree)
	#tree=mul_lists(tree)
	tree=expand(tree)
	tree=reduce_similar(tree)
	tree=truncate(tree)
	tree=remove_useless_lists(tree)
	if copy==tree:
		return copy
	else:
		return crunch(tree)
def print_out(tree):
	result=''
	if type(tree)==type(1) or type(tree)==type(''):
		result=str(tree)
	elif tree[0]=='+':
		for i in tree[1:]:
			result+=print_out(i)
			result+='+'
		result=result[:-1]
	elif tree[0]=='*':
		for i in sorted(tree[1:]):
			result+=print_out(i)
	return result
expr = sys.argv[1]
print "expr:", expr
tokens = tokenize(parse(expr))
print "tokenized:", tokens
tree = astree(tokens)
print "tree:\n", print_tree(tree)
tree=crunch(tree)
print "tree after crunch:\n", print_tree(tree)
print print_out(tree)

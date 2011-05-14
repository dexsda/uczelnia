#!/usr/bin/python2
import sys
import re

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
				tokens.append(tok)
				tok=''
			tokens.append(c)
	return tokens

def polish_notation(tokens):
	out=[]
	stack=[]
	for tok in tokens[::-1]:
		if tok in {'*','+','-','(',')'}:
			if tok=='(':
				stack.append(tok)
			elif tok==')':
				while stack[-1]!='(':
					out.append(stack.pop())
				stack.pop()
			else:
				while len(stack)>0 and (stack[-1]=='*' or (tok in {'+','-'})):
					out.append(stack.pop())
				stack.append(tok)
		else:
			out.append(tok)
	while len(stack)>0:
		out.append(stack.pop())
	return out[::-1]

expr = sys.argv[1]
tokens = tokenize(parse(expr))
print polish_notation(tokens)


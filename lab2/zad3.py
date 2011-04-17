#!/usr/bin/python
def flatten(ls):
	if not isinstance(ls,list):
		return [ls]
	elif len(ls)==0:
		return []
	elif len(ls)==1:
		return flatten(ls[0])
	else:
		return reduce(lambda x,y:flatten(x)+flatten(y),ls)

flatten2 = lambda ls:(not isinstance(ls,list) and [ls]) or (len(ls)==1 and flatten2(ls[0])) or (ls and reduce(lambda x,y:flatten2(x)+flatten2(y),ls))

def flatten3(x):
	result = []
	for el in x:
		if isinstance(el, list):
			result.extend(flatten(el))
		else:
			result.append(el)
	return result

def rem(x):
	while isinstance(x,list) and len(x)==1:
		x=x[0]
	return x

flatten_filt = lambda ls:filter(lambda x:x,
	reduce(lambda x,y:flatten_filt(x)+flatten_filt(y),
		map(lambda x:(not isinstance(x,list) and [x]) or x,
			map(lambda x:(isinstance(x,list) and len(x)==1 and x[0]) or x,ls))))


ls = [1,[2,3],[[[[]],[[4]]]]]
print flatten(ls)
print flatten2(ls)
print flatten3(ls)

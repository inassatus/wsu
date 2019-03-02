#Jeonghyeon Woo 011618854
#HW 3
#windows 10

import random

def addDict(d):
	newdict={};
	for course in d:
		for day in d[course]:
			if day in newdict:
				newdict[day]+=d[course][day]
			else:
				newdict[day]=d[course][day]
	return newdict

def testaddDict():
	d1={'a':{'Mon':1}, 'b':{'Tue':2}, 'c':{'Wed':3}}
	d2={'a':{'Mon':1, 'Tue':2}, 'b':{'Tue':2, 'Wed':3}, 'c':{'Wed':3, 'Thur':4}}
	if addDict({})!={}:
		return False
	if dict(sorted(list(addDict(d1).items()))) != {'Mon':1, 'Tue':2,'Wed':3}:
		return False
	if dict(sorted(list(addDict(d2).items()))) != {'Mon':1, 'Tue':4, 'Wed':6, 'Thur':4}:
		return False
	return True

def addDictN(L):
	newdict={}
	for dic in L:
		added = addDict(dic)
		for day in added:
			if day in newdict:
				newdict[day] += added[day]
			else:
				newdict[day] = added[day]
	return newdict

def testaddDictN():
	d1=[{'a':{'Mon':1}, 'b':{'Tue':2}, 'c':{'Wed':3}}]
	d2=[{'a':{'Mon':1}, 'b':{'Tue':2}, 'c':{'Wed':3}}, {'a':{'Mon':1, 'Tue':2}, 'b':{'Tue':2, 'Wed':3}, 'c':{'Wed':3, 'Thur':4}}]
	if addDict([])!={}:
		return False
	if dict(sorted(list(addDictN(d1).items())))!= {'Mon':1, 'Tue':2,'Wed':3}:
		return False
	if dict(sorted(list(addDictN(d2).items())))!= {'Mon':2, 'Tue':6, 'Wed':9, 'Thur':4}:
		return False
	return True


def lookupVal(L, k):
	L=L[::-1]
	for x in L:
		if k in x:
			return x[k]
	return None

def testlookupVal():
	l=[{"a": 5}, {"a": 1, "b":"yes!"}]
	if lookupVal([], 5) != None:
		return False
	if lookupVal(l, "a")!=1:
		return False
	if lookupVal(l, "b")!="yes!":
		return False
	return True

def lookupVal2(tL, k):
	if tL==[]:
		return None
	newdic=[]
	current=tL[-1]
	while(current!=tL[0]):
		newdic.append(current[1])
		current=tL[current[0]]
	newdic.append(current[1])
	for x in newdic:
		if k in x:
			return x[k]
	return None

def testlookupVal2():
	l = [(0, {'x': 1}), (0, {'x': 2, 'y':1}), (1, {'y':2})]
	if lookupVal2(l, "t") != None:
		return False
	if lookupVal2([], 'x') != None:
		return False
	if lookupVal2(l, 'x')!=2:
		return False
	if lookupVal2(l, 'y')!=2:
		return False
	return True

def numPaths(m, n, blocks):
	if m<1 or n<1:
		return 0
	if m==1 and n==1:
		return 1
	if (m,n) in blocks:
		return 0
	if m==1:
		return numPaths(m, n-1, blocks)
	if n==1:
		return numPaths(m-1, n, blocks)
	return numPaths(m-1, n, blocks)+numPaths(m, n-1, blocks)
#it is dfs
def testnumPaths():
	if numPaths(0, 0, [])!=0:
		return False
	if numPaths(1,1,[])!=1:
		return False
	if numPaths(2,2,[(2,2)])!=0:
		return False
	if numPaths(3,3,[])!=6:
		return False
	return True

def palindromes(s):
	newlist=[]
	for x in range(len(s)):
		for y in range(x+2, len(s)+1):
			if s[x:y]==s[x:y][::-1]:
				if s[x:y] in newlist:
					continue
				newlist+=[s[x:y]]
	return sorted(newlist)

def testpalindromes():
	if palindromes('')!=[]:
		return False
	if palindromes('aa')!=['aa']:
		return False
	if palindromes('abba')!=['abba','bb']:
		return False
	return True

class iterApply(object):
	def __init__(self, n, f):
		self.n=n
		self.f=f
	def __next__(self):
		result = self.f(self.n)
		self.n=self.n+1
		return result
	def __iter__(self):
		return self.f(self.n)

def iMerge(iNumbers1, iNumbers2, n):
	merged = []
	for i in range(n):
		if iNumbers2.__iter__()>iNumbers1.__iter__():
			merged.append(iNumbers1.__iter__())
			iNumbers1.__next__()
		else:
			merged.append(iNumbers2.__iter__())
			iNumbers2.__next__()
	return merged

def testiMerge():
	a=iterApply(1, lambda x: x**1)
	b=iterApply(1, lambda x: x**2)
	c=iterApply(1, lambda x: x**3)
	if iMerge(a, b, 5)!=[1,1,2,3,4]:
		return False
	if iMerge(b, c, 6)!= [1,8,9,16,25,27]:
		return False
	return True

class Stream(object):
	def __init__(self, first, compute_rest, empty= False):
		self.first = first
		self._compute_rest = compute_rest
		self.empty = empty
		self._rest = None
		self._computed = False
	@property
	def rest(self):
		assert not self.empty, 'Empty streams have no rest.'
		if not self._computed:
			self._rest = self._compute_rest()
			self._computed = True
		return self._rest

def streamRandoms(k, min, max):
	def compute_rest():
		return streamRandoms(random.randint(min, max), min, max)
	return Stream(k, compute_rest)

def oddStream(s):
	while(s.first%2!=1):
		s=s.rest
	def compute_rest():
		return oddStream(s.rest)
	return Stream(s.first, compute_rest)

def testoddStream():
	oddS = oddStream(streamRandoms(1,1,100))
	myList=[]
	for i in range(100):
		myList.append(oddS.first)
		oddS=oddS.rest
	for i in range(100):
		if myList[i]%2==0:
			return False
	return True

if __name__=='__main__':
	passedMsg = "%s passed"
	failedMsg = "%s failed"
	funlist=['addDict', 'addDictN', 'lookupVal', 'lookupVal2', 'numPaths', 'palindromes', 'iMerge', 'oddStream']
	testlist=[testaddDict(), testaddDictN(), testlookupVal(), testlookupVal2(), testnumPaths(), testpalindromes(), testiMerge(), testoddStream()]
	for x in range(len(testlist)):
		if testlist[x]:
			print(passedMsg % funlist[x])
		else:
			print(failedMsg % funlist[x])

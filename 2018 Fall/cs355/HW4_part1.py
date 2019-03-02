#Jeonghyeon Woo 011618854
#HW 4-1

opstack=[];
dictstack=[];
def opPop():
	global opstack
	if(opstack):
		return opstack.pop()
	else:
		print("error: empty stack")

def opPush(value):
	global opstack
	global dictstack
	if(type(value)==str):
		if(value[0]=='/'):
			opstack.append(value[1:])
		else:
			result=lookup(value)
			if(value!=None):
				opstack.append(result)
				return
			else:
				print("error: varaible not fount")
	else:
		opstack.append(value)

def dictPop():
	global dictstack
	if(dictstack):
		return dictstack.pop()
	else:
		print("empty dict")

def dictPush(x):
	dictstack.append(x)

def define(name, value):
	global dictstack
	if not dictstack:
		dictPush({})
	dictstack[-1][name]=value

def lookup(name):
	global dictstack
	for i in reversed(dictstack):
		if name in i:
			return i[name]
	print("error: not defined")
	return None

def isempty():
	global opstack
	if not opstack: return True
	return False

def iscoputable():
	if(isempty()):
		print("error: empty stack")
		return False
	n=opPop()
	if(type(n)!=int and type(n)!=float):
		print("error: invalid type")
		return False
	return n

def add():
	first=iscoputable()
	if(not first):
		return
	second=iscoputable()
	if(not second):
		return
	opPush(first+second)

def sub():
	first=iscoputable()
	if(not first):
		return
	second=iscoputable()
	if(not second):
		return
	opPush(second-first)

def mul():
	first=iscoputable()
	if(not first):
		return
	second=iscoputable()
	if(not second):
		return
	opPush(first*second)

def div():
	first=iscoputable()
	if(not first):
		return
	second=iscoputable()
	if(not second):
		return
	opPush(second/first)

def eq():
	first=iscoputable()
	if(not first):
		return
	second=iscoputable()
	if(not second):
		return
	opPush(second==first)

def lt():
	first=iscoputable()
	if(not first):
		return
	second=iscoputable()
	if(not second):
		return
	opPush(second<first)

def gt():
	first=iscoputable()
	if(not first):
		return
	second=iscoputable()
	if(not second):
		return
	opPush(second>first)

def length():
	if(isempty()):
		print("error: empty stack")
		return
	array=opPop()
	if(type(array)!=list):
		print("error: not array")
		return
	opPush(len(array))

def get():
	if(isempty()):
		print("error: empty stack")
		return
	index=opPop()
	if(isempty()):
		print("error: empty stack")
		return
	array=opPop()
	if(type(index)!=int or type(array)!=list):
		print("error: type error")
		return
	opPush(array[index])

def psAnd():
	if(isempty()):
		print("error: empty stack")
		return
	first=opPop()
	if(isempty()):
		print("error: empty stack")
		return
	second=opPop()
	if(type(first)!=bool or type(second)!=bool):
		print("error: not bool type")
		return
	opPush(first and second)

def psOr():
	if(isempty()):
		print("error: empty stack")
		return
	first=opPop()
	if(isempty()):
		print("error: empty stack")
		return
	second=opPop()
	if(type(first)!=bool or type(second)!=bool):
		print("error: not bool type")
		return
	opPush(first or second)

def psNot():
	if(isempty()):
		print("error: empty stack")
		return
	first=opPop()
	if(type(first)!=bool):
		print("error: not bool type")
		return
	opPush(not first)

def dup():
	if(isempty()):
		print("error: empty stack")
		return
	global opstack
	opstack.append(opstack[-1])

def pop():
	if(isempty()):
		print("error: empty stack")
		return
	global opstack
	opstack.pop()

def exch():
	if(isempty()):
		print("error: empty stack")
		return
	first=opPop()
	if(isempty()):
		print("error: empty stack")
		return
	second=opPop()
	opPush(first)
	opPush(second)

def copy():
	global opstack
	if not opstack:
		print("error: n count is not given")
		return
	n=opstack.pop()
	copy=[]
	for i in range(n):
		if not opstack:
			opstack+=copy+copy
			return
		copy=[opstack.pop()]+copy
	opstack+=copy+copy

def clear():
	global opstack
	while(opstack):
		opstack.pop()

def stack():
	global opstack
	for i in opstack:
		print(i)

def psDict():
	if(isempty()):
		print("error: initial size is not given")
		return
	opPop()
	opPush({})

def begin():
	dictPush(opPop())

def end():
	dictPop()

def psDef():
	if(isempty()):
		print("error: empty stack")
		return
	value=opPop()
	if(isempty()):
		print("error: empty stack")
		return
	name=opPop()
	define(name, value)

#opPop, opPush, dictPop, dictPush, define, lookup are not tested since the other test functions always call those function
#also pop and clear will not be tested since they are called by stack manipulate functions
#stack will be called at the end of main function

def testadd():
	opPush(1)
	opPush(2)
	add()
	result=opPop()
	if(result!=3): return False
	opPush(3)
	opPush(4)
	add()
	result=opPop()
	if(result!=7): return False
	return True

def testsub():
	opPush(2)
	opPush(1)
	sub()
	result=opPop()
	if result!=1: return False
	opPush(7)
	opPush(5)
	sub()
	result=opPop()
	if result!=2: return False
	return True

def testmul():
	opPush(1)
	opPush(2)
	mul()
	result=opPop()
	if result!=2: return False
	opPush(2)
	opPush(2)
	mul()
	result=opPop()
	if result!=4: return False
	return True

def testdiv():
	opPush(2)
	opPush(2)
	div()
	result=opPop()
	if result!=1.0: return False
	opPush(2)
	opPush(4)
	div()
	result=opPop()
	if result!=0.5: return False
	return True

def testeq():
	opPush(2)
	opPush(2)
	eq()
	result=opPop()
	if result==False: return False
	opPush(4)
	opPush(1)
	eq()
	result=opPop()
	if result==True: return False
	return True

def testlt():
	opPush(2)
	opPush(2)
	lt()
	result=opPop()
	if result==True: return False
	opPush(2)
	opPush(4)
	lt()
	result=opPop()
	if result==False: return False
	return True

def testgt():
	opPush(2)
	opPush(2)
	gt()
	result=opPop()
	if result==True: return False
	opPush(2)
	opPush(4)
	gt()
	result=opPop()
	if result==True: return False
	return True

def testlength():
	opPush([1,2,3,4,5])
	length()
	result=opPop()
	if result!=5: return False
	opPush([1,2])
	length()
	result=opPop()
	if result!=2: return False
	return True

def testget():
	opPush([1,2,3,4,5])
	opPush(0)
	get()
	result=opPop()
	if result!=1: return False
	opPush([1,2,3,4,5])
	opPush(3)
	get()
	result=opPop()
	if result!=4: return False
	return True

def testpsAnd():
	opPush(True)
	opPush(False)
	psAnd()
	result=opPop()
	if result==True: return False
	opPush(True)
	opPush(True)
	psAnd()
	result=opPop()
	if result==False: return False
	return True

def testpsOr():
	opPush(True)
	opPush(False)
	psOr()
	result=opPop()
	if result==False: return False
	opPush(False)
	opPush(False)
	psAnd()
	result=opPop()
	if result==True: return False
	return True

def testpsNot():
	opPush(True)
	psNot()
	result=opPop()
	if result==True: return False
	opPush(False)
	psNot()
	result=opPop()
	if result==False: return False
	return True

def testdup():
	opPush(1)
	opPush(2)
	dup()
	if(opPop()!=2): return False
	dup()
	if(opPop()!=2): return False
	clear()
	return True

def testexch():
	opPush(True)
	opPush(False)
	exch()
	result=opPop()
	if result==False: return False
	opPush(True)
	exch()
	result=opPop()
	if result==True: return False
	pop()
	return True

def testcopy():
	opPush(1)
	opPush(2)
	opPush(2)
	copy()
	result=opPop()
	if result!=2: return False
	result=opPop()
	if result!=1: return False
	clear()
	return True

def testDicts():
	opPush("/x")
	opPush(111)
	psDef()
	opPush("x")
	opPush(5)
	psDict()
	begin()
	opPush("/x")
	opPush(222)
	psDef()
	opPush("x")
	end()
	opPush("x")
	if(opPop()!=111): return False
	if(opPop()!=222): return False
	if(opPop()!=111): return False
	return True


if __name__ == '__main__':
	testcases = [('add', testadd), ('sub', testsub), ('mul', testmul), ('div', testdiv), ('eq', testeq), ('lt', testlt), ('gt', testgt), ('length', testlength), ('get', testget), ('psAnd', testpsAnd), ('psOr', testpsOr), ('psNot', testpsNot), ('dup', testdup), ('exch', testexch), ('copy', testcopy), ('dictionary manipulation', testDicts)]                            
	failedcase=0
	for (testName, testProc) in testcases:
		if not testProc():
			failedcase+=1
			print("%s is failed", testName)
	if(failedcase):
		print("%d cases failed", failedcase)
	else:
		stack()
		print("all good")

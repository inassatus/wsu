#Jeonghyeon Woo 011618854
#HW 4-2

opstack=[];
dictstack=[];
def opPop():
	global opstack
	if(opstack):
		return opstack.pop()
	else:
		return None

def opPush(value):
	global opstack
	global dictstack
	if(type(value)==str):
		if(value[0]=='/'):
			opstack.append(value)
		else:
			result=lookup("/"+value)
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
	

def add():
	first=opPop()
	second=opPop()
	opPush(first+second)

def sub():
	first=opPop()
	second=opPop()
	opPush(second-first)

def mul():
	first=opPop()
	second=opPop()
	opPush(first*second)

def div():
	first=opPop()
	second=opPop()
	opPush(second/first)

def eq():
	first=opPop()
	second=opPop()
	opPush(second==first)

def lt():
	first=opPop()
	second=opPop()
	opPush(second<first)

def gt():
	first=opPop()
	second=opPop()
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

#here are given codes

import re
def tokenize(s):
    return re.findall("/?[a-zA-Z][a-zA-Z0-9_]*|[[][a-zA-Z0-9_\s!][a-zA-Z0-9_\s!]*[]]|[-]?[0-9]+|[}{]+|%.*|[^ \t\n]", s)

# The it argument is an iterator. The sequence of return characters should
# represent a string of properly nested {} parentheses pairs, from which
# the leasing '{' has been removed. If the parentheses are not properly
# nested, returns False.
def groupMatching(it):
    res = []
    for c in it:
        if c == '}':
            return res
        else:
            # Note how we use a recursive call to group the inner matching
            # parenthesis string and append it as a whole to the list we are
            # constructing. Also note how we have already seen the leading
            # '{' of this inner group and consumed it from the iterator.
            res.append(groupMatching(it))
    return False

# Function to parse a string of { and } braces. Properly nested parentheses
# are arranged into a list of properly nested lists.
def group(s):
    res = []
    it = iter(s)
    for c in it:
        if c=='}':  #non matching closing paranthesis; return false
            return False
        else:
            res.append(groupMatching(it))
    return res

#group("{{}{{}}}")


# The it argument is an iterator.
# The sequence of return characters should represent a list of properly nested
# tokens, where the tokens between '{' and '}' is included as a sublist. If the
# parenteses in the input iterator is not properly nested, returns False.
def groupMatching2(it):
    res = []
    for c in it:
        if c == '}':
            return res
        elif c=='{':
            # Note how we use a recursive call to group the tokens inside the
            # inner matching parenthesis.
            # Once the recursive call returns the code array for the inner
            # paranthesis, it will be appended to the list we are constructing
            # as a whole.
            res.append(groupMatching2(it))
        else:
            res.append(c)
    return False


# RENAME THIS FUNCTION AS parse
# Function to parse a list of tokens and arrange the tokens between { and } braces
# as code-arrays.
# Properly nested parentheses are arranged into a list of properly nested lists.
def parse(L):
    res = []
    it = iter(L)
    for c in it:
        if c=='}':  #non matching closing paranthesis; return false since there is
                    # a syntax error in the Postscript code.
            return False
        elif c=='{':
            res.append(groupMatching2(it))
        else:
            res.append(c)
    return res

# Write the necessary code here; again write
# auxiliary functions if you need them. This will probably be the largest
# function of the whole project, but it will have a very regular and obvious
# structure if you've followed the plan of the assignment.
#
def interpretSPS(code): # code is a code array
    cmd={'add':add, 'sub':sub, 'mul':mul, 'div':div, 'eq':eq, 'lt':lt, 'gt':gt,
    	'length':length, 'get':get, 'and':psAnd, 'or':psOr, 'not':psNot, 'dup':dup,
    	'exch':exch, 'pop':pop, 'copy':copy, 'clear':clear, 'stack':stack, 'dict':psDict, 
    	'begin':begin, 'end':end, 'def':psDef}
    for x in code:
    	if type(x)==list:
    		opPush(func(x))
    	elif type(x)==str:
	    	if x in cmd:
	    		cmd[x]()
	    	elif x=='True' or x=='true':
	    		opPush(True)
	    	elif x=='False' or x=='false':
	    		opPush(False)
	    	elif isint(x):
	    		opPush(int(x))
	    	elif isfloat(x):
	    		opPush(float(x))
	    	#unfortunately, can not use functions as keys, so hardcode above
	    	elif x[0]=='[' and x[-1]==']':
	    		opPush(parsearr(x))
	    	elif x=='if':
	    		psif()
	    	elif x=='ifelse':
	    		psifelse()
	    	elif x=='forall':
	    		forall()
	    	elif x=='for':
	    		psfor()
	    	else:
	    		opPush(x)
	    		top=opPop()
	    		if type(top)==func:
	    			interpretSPS(top.get())
	    		else:
	    			opPush(x)

class func:
	def __init__(self, a):
		self.data=[]
		self.data=a
	def get(self):
		return self.data
#this is the class for defined functions: it is called when the function name is pushed

def isint(n):
	try:
		int(n)
	except ValueError:
		return False
	return True

def isfloat(n):
	try:
		float(n)
	except ValueError:
		return False
	return True

def parsearr(a):
	result=[]
	for x in a[1:-1].split():
		if isint(x):
			result.append(int(x))
		elif isfloat(x):
			result.append(float(x))
		elif x=='add':
			result.append(result.pop()+result.pop())
		elif x=='sub':
			result.append(-(result.pop()-result.pop()))
		elif x=='mul':
			result.append(result.pop()*result.pop())
		elif x=='div':
			result.append(1/result.pop()/result.pop())
	return result

def psif():
	task=opPop()
	condition=opPop()
	if (task is None or condition is None):
		print("error: empty stack")
		return
	if(type(task)!=func):
		print("error: if-do is not correct")
		return
	if(type(condition)!=bool):
		print("error: if-condition is not correct")
		return
	if condition:
		interpretSPS(task.get())

def psifelse():
	task2=opPop()
	task1=opPop()
	condition=opPop()
	if (task1 is None or task2 is None or condition is None):
		print("error: empty stack")
		return
	if(type(task1)!=func or type(task2)!=func):
		print("error: ifelse-do is not correct")
		return
	if(type(condition)!=bool):
		print("error: ifelse-condition is not correct")
		return
	if condition:
		interpretSPS(task1.get())
	else:
		interpretSPS(task2.get())

def psfor():
	task=opPop()
	until=opPop()
	incr=opPop()
	start=opPop()
	if(task is None or until is None or incr is None or start is None):
		print("error: empty stack")
		return
	if(type(start)!=int or type(incr)!=int or type(until)!=int):
		print("error: for-condition is not correct")
		return
	if (type(task)!=func):
		print("error: for-task is not correct")
		return

	while(start!=until+incr):
		opPush(start)
		interpretSPS(task.get())
		start+=incr



def forall():
	task=opPop()
	arr=opPop()
	if(task is None or arr is None):
		print("error: empty stack")
		return
	if (type(task)!=func):
		print("error: forall-task is not correct")
		return
	if (type(arr)!=list):
		print("error: forall-array is not correct")
		return
	for x in arr:
		opPush(x)
		interpretSPS(task.get())



# Copy this to your HW4_part2.py file>
def interpreter(s): # s is a string
    interpretSPS(parse(tokenize(s)))



input1 = """
  /square {dup mul} def  
  [1 2 3 4] {square} forall 
  add add add 30 eq true 
  stack
"""
input2 = """ 
  [1 2 3 4 5] dup length /n exch def
  /fact {
      0 dict begin
         /n exch def
         n 2 lt
         { 1}
         {n 1 sub fact n mul }
         ifelse
      end 
  } def
  n fact stack    
"""

input3 = """
  [9 9 8 4 10] {dup 5 lt {pop} if}  forall 
  stack 
"""

# this test case should not be working
# according to the lecture note for operator of poscript pops 
# 4 values from the stack (i, i<n, i++, and task),
# and this test case assumes i is remaining on the stack so
# sum+=i^2 is same as the forall one
# however, i is already popped up, so it is not possible to use i
input4 = """
  [1 2 3 4 5] dup length exch {dup mul}  forall
  add add add add
  exch 0 exch dup -1 1 {dup mul add} for
  eq stack 
"""
input5="""/yes 0 1 5 {dup} for stack"""
input6="""/oh 5 def [1 2 3 add] {oh add} forall stack"""

def testinput1():
	interpreter(input1)
	if(opPop()!=True):
		clear()
		return False
	if(opPop()!=True):
		clear()
		return False
	clear()
	return True

def testinput2():
	interpreter(input2)
	if(opPop()!=120):
		clear()
		return False
	if(opPop()!=[1,2,3,4,5]):
		clear()
		return False
	clear()
	return True

def testinput3():
	interpreter(input3)
	if(opPop()!=10):
		clear()
		return False
	if(opPop()!=8):
		clear()
		return False
	if(opPop()!=9):
		clear()
		return False
	if(opPop()!=9):
		clear()
		return False
	clear()
	return True

def testinput5():
	interpreter(input5)
	for x in range(4):
		if(x!="/yes"):
			clear()
			return False
	clear()
	return True

def testinput6():
	interpreter(input6)
	if(opPop()!=10):
		clear()
		return False
	if(opPop()!=6):
		clear()
		return False
	clear()
	return True

#if __name__ == '__main__':
#	if(testinput1() and testinput2() and testinput3() and testinput5() and testinput6()):
#		print("all good!")
#	else:
#		print("some test cases failed")

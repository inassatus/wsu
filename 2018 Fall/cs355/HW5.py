#Jeonghyeon Woo 011618854
#HW 5


opstack=[]
dictstack=[{'level': 0}]
level=0
scope=""
cwd=""

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
	if(name in dictstack[-1]):
		return dictstack[-1][name]
	for i in reversed(dictstack):
		if(scope=='static'):
			if(level<i['level']):
				continue
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
	global dictstack
	opPop()
	opPush({'level': len(dictstack)})

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
def interpretSSPS(code): # code is a code array
    cmd={'add':add, 'sub':sub, 'mul':mul, 'div':div, 'eq':eq, 'lt':lt, 'gt':gt,
    	'length':length, 'get':get, 'and':psAnd, 'or':psOr, 'not':psNot, 'dup':dup,
    	'exch':exch, 'pop':pop, 'copy':copy, 'clear':clear, 'stack':stack, 'dict':psDict, 
    	'begin':begin, 'end':end, 'def':psDef}
    global level
    global dictstack
    olevel=level
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
	    			opPush(1)
	    			psDict()
	    			begin()
	    			level=top.getlevel()
	    			interpretSSPS(top.get())
	    			print('------',x,'------')
	    			for i in dictstack:
	    				print(i)
	    			print('-----------------')
	    			end()
	    			level=olevel
	    		else:
	    			opPush(x)

class func:
	def __init__(self, a):
		global dictstack
		self.level=len(dictstack)-1
		self.data=a
	def get(self):
		return self.data
	def getlevel(self):
		return self.level
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
def interpreter(s, newscope): # s is a string
	global scope
	scope=newscope
	interpretSSPS(parse(tokenize(s)))


def test1():
	testcase="/x 10 def /A { x } def /C { /x 40 def A stack } def /B { /x 30 def /A { x } def C } def B"
	scope1='static'
	scope2='dynamic'

	interpreter(testcase, scope1)
	if(opPop()!=10):
		return False
	interpreter(testcase, scope2)
	if(opPop()!=40):
		return False
	return True

def test2():
	testcase="/m 50 def /n 100 def /egg1 {/m 25 def n} def /chic {/n 1 def /egg2 { n } def m n egg1 egg2 stack } def n chic"
	scope1='static'
	scope2='dynamic'
	interpreter(testcase, scope1)
	if(opPop()!=1 or opPop()!=100 or opPop()!=1 or opPop()!=50 or opPop()!=100):
		return False
	interpreter(testcase, scope2)
	if(opPop()!=1 or opPop()!=1 or opPop()!=1):
		return False
	clear()
	return True

def test3():
	testcase="/x 1 def /a { x } def /b { /x 2 def x a } def b stack"
	scope1='static'
	scope2='dynamic'
	interpreter(testcase, scope1)
	if(opPop()!= 1 or opPop()!= 2):
		return False
	interpreter(testcase, scope2)
	if(opPop()!=2 or opPop()!=2):
		return False
	return True

def test4():
	testcase="/x 1 def /y { /x 2 def x } def /z { /x 3 def x } def z y x stack"
	scope1='static'
	scope2='dynamic'
	interpreter(testcase, scope1)
	if(opPop()!=1 or opPop()!=2 or opPop()!=3):
		return False
	interpreter(testcase, scope2)
	if(opPop()!=1 or opPop()!=2 or opPop()!=3):
		return False
	return True

if __name__ == '__main__':
	if(test1() and test2() and test3() and test4()):
		print("all good!")
	else:
		print("some test cases failed")



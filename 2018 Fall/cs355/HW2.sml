(*Jeonghyeon Woo 011618854*)



Control.Print.printDepth := 100;



(*p1*)

fun numbersToSum a []=[]
|numbersToSum a (x::rest)=
	if (a-x)<0 then []
	else if (a-x)=0 then []
	else x::(numbersToSum (a-x) rest);
(*So, the function adds up the number in the list until 'a' becomes less or equal to 0*)

fun checkpos a =
	if a<0 then 0
	else a;
(*check if the number is a positive integer*)
fun startnumbersToSumTail a [] = []
|startnumbersToSumTail 0 (x::rest)=[]
|startnumbersToSumTail a (x::rest) = x::(startnumbersToSumTail (checkpos (a-hd(rest))) rest);
(*if a - next head of list is less or equal to 0 than stop*)
fun numbersToSumTail a L = (startnumbersToSumTail (checkpos (a-hd(L)))  L);
(*base of numbersToSumTail*)






(*p2*)

fun filter pred [] = []
| filter pred (x::rest) =
	if pred x then x::(filter pred rest)
	else (filter pred rest);
(*filter the list as elements follow the rule*)
fun reversefilter pred [] = []
|reversefilter pred (x::rest) =
	if pred x then (reversefilter pred rest)
	else x::(reversefilter pred rest);
(*filter the list as elements don't follow the rule*)
fun partition a [] = ([], [])
| partition a L= ((filter a L),(reversefilter a L));
(*make tuple with filter and reversefilter*)






(*p3*)

fun exists x [] = false
| exists x (y::rest) = 
	if x=y then true
	else (exists x rest);
(*check if the element exists in the list*)
fun areAllUnique [] = true
| areAllUnique (x::rest) =
	if (exists x rest) then false
	else (areAllUnique rest);
(*if there is an element that exists in the rest list then immidiately return false*)






(*p4*)

fun fold f base [] = base
| fold f base (x::rest) = f x (fold f base rest);
(*fold: process given function from the last element of the list*)

(*a*)
fun add x y = x+y;
fun sum [] = 0
| sum (x::rest) = (fold add 0 x)+(sum rest);
(*implementaion of map*)

(*b*)
datatype option = NONE | SOME of int;
fun value(NONE) = 0 | value(SOME(a))=a;
fun addsome a b = 
	if (value(a)+value(b))=0 then NONE
	else SOME(value(a)+value(b));
fun sumOption [] = NONE
| sumOption (x::rest) = (addsome (fold addsome NONE x) (sumOption rest));
(*same algorithm as sum, but datatype is option*)

(*c*)
datatype either = IString of string | IInt of int;
fun str2int s = valOf(Int.fromString(s));
fun intvalue(IString(a)) = (str2int a) | intvalue(IInt(b)) = b;
fun addeither a b = IInt(intvalue(a)+intvalue(b));
fun sumEither [] = IInt(0)
| sumEither (x::rest) = (addeither (fold addeither (IInt(0)) x) (sumEither rest));
(*same algorith mas sumOption, but value of IString is automatically turned into int*)






(*p5*)
(*design of tree = always have two node and a name, and defined the end as leaf*)
datatype 'a Tree = LEAF of 'a | NODE of 'a * ('a Tree) * ('a Tree);

fun isend(LEAF(a)) = true | isend(NODE(a, b, c)) = false; (*check if it is the deepest node*)
fun firstchild(NODE(a,b,c)) = b; (*access first child*)
fun secondchild(NODE(a,b,c)) = c; (*access second child*)
fun getval(LEAF(a))=a | getval(NODE(a,b,c))=a;

(*a*)
fun depthScan x =
	if isend(x) then getval(x)::[]
	else depthScan(firstchild(x))@depthScan(secondchild(x))@getval(x)::[];
(*add list contain the value of NODE or LEAF from the deepest*)

(*b*)
fun finaldeeper x y z =
	if getval(x)=y then z::[]
	else [];
fun deeper x y z=
	if isend(x) then (finaldeeper x y z) 
	else if getval(x)=y then (deeper (firstchild(x)) y (z+1))@(deeper (secondchild(x)) y (z+1))@z::[]
	else (deeper (firstchild(x)) y (z+1))@(deeper (secondchild(x)) y (z+1))@[];
(*create the list of level that contain NODE value equal to y*)
fun depthSearch x y =
	if (deeper x y 1) = [] then ~1
	else hd((deeper x y 1));
(*return ~1 if deeper returns empty list, or return the first element of the returned list*)

(*c*)
fun copy(x) =
	if isend(x) then x
	else NODE(getval(x), (copy(firstchild(x))), (copy(secondchild(x))));
(*copy subtrees*)
fun finaladd(x,y)=
	if isend(x) then LEAF(getval(x)+getval(y)) (*if both trees are LEAF then just return added LEAF*)
	else copy(NODE((getval(x)+getval(y)), (firstchild(x)), (secondchild(x)))); (*copy the trees after this final add*)
(*final stage of adding trees*)
fun addTrees x y =
	if isend(x) then finaladd(y,x)
	else if isend(y) then finaladd(x,y)
	else NODE((getval(x)+getval(y)), (addTrees (firstchild(x)) (firstchild(y))), (addTrees (secondchild(x)) (secondchild(y))));
(*add trees until one of input tree is LEAF*)

(*d*)
val T1 = NODE(1, NODE(2, NODE(3, LEAF(4), LEAF(4)), LEAF(3)), LEAF(2));
val T2 = NODE(1, LEAF(2), NODE(2, LEAF(3), NODE(3, LEAF(4), LEAF(4))));
val T3 = NODE(1, NODE(2, LEAF(3), NODE(3, LEAF(4), LEAF(4))), NODE(2, NODE(3, LEAF(4), LEAF(4)), LEAF(3)));

fun test_depthScan () =
let
	val test1 = ((depthScan T1) = [4,4,3,3,2,2,1])
	val test2 = ((depthScan T2) = [2,3,4,4,3,2,1])
	val test3 = ((depthScan T3) = [3,4,4,3,2,4,4,3,3,2,1])
in
	print ("depthScan:\n" ^
	"test1: " ^ Bool.toString(test1) ^ “\n" ^
	"test2: " ^ Bool.toString(test2) ^ “\n" ^
	"test3: " ^ Bool.toString(test3) ^ “\n")
end
	val _ = test_depthScan();

fun test_depthSearch () =
let
	val test1 = ((depthSearch T1 4) = 4)
	val test2 = ((depthSearch T2 3) = 3)
	val test3 = ((depthSearch T3 1) = 1)
in
	print ("depthSearch:\n" ^
	"test1: " ^ Bool.toString(test1) ^ “\n" ^
	"test2: " ^ Bool.toString(test2) ^ “\n" ^
	"test3: " ^ Bool.toString(test3) ^ “\n")
end
	val _ = test_depthSearch();

fun test_addTrees () = (*since it takes too much space to write NODEs, use depthScan*)
let
	val test1 = ( (depthScan(addTrees T1 T2)) = [4,4,3,3,4,3,4,4,3,4,2])
	val test2 = ( (depthScan(addTrees T2 T3)) = [3,4,4,3,4,4,4,6,4,4,6,4,2])
	val test3 = ((depthScan(addTrees T1 T3)) = [4,4,6,4,4,6,4,4,4,3,3,4,2])
in
	print ("depthSearch:\n" ^
	"test1: " ^ Bool.toString(test1) ^ “\n" ^
	"test2: " ^ Bool.toString(test2) ^ “\n" ^
	"test3: " ^ Bool.toString(test3) ^ “\n")
end
	val _ = test_addTrees();





(*tests*)

fun test_numbersToSum () =
let
	val numbersToSum1 = ((numbersToSum 50 [10, 20, 30]) = [10, 20])
	val numbersToSum2 = ((numbersToSum 40 [1,2,3,4,40]) = [1,2,3,4])
in
	print ("numbersToSum:\n" ^
	"test1: " ^ Bool.toString(numbersToSum1) ^ “\n" ^
	"test2: " ^ Bool.toString(numbersToSum2) ^ “\n")
end
	val _ = test_numbersToSum();

fun test_numbersToSumTail () =
let
	val numbersToSumTail1 = ( (numbersToSumTail 50 [10, 20, 30]) = [10,20] )
	val numbersToSumTail2 = ( (numbersToSumTail 40 [1,2,3,4,40]) = [1,2,3,4] )
in
	print ("numbersToSumTail:\n" ^
	"test1: " ^ Bool.toString(numbersToSumTail1) ^ “\n" ^
	"test2: " ^ Bool.toString(numbersToSumTail2) ^ “\n")
end
	val _ = test_numbersToSumTail();

fun test_partition () =
let
	val test1 = ( (partition (fn x => (x<=6)) [1,2,3,4,5,6,7]) = ([1,2,3,4,5,6], [7]) )
	val test2 = ( (partition (fn x => (x<=1)) [1,2,3,4,5,6]) = ([1], [2,3,4,5,6]) )
in
	print ("partition:\n" ^
	"test1: " ^ Bool.toString(test1) ^ “\n" ^
	"test2: " ^ Bool.toString(test2) ^ “\n")
end
	val _ = test_partition();

fun test_areAllUnique () =
let
	val test1 = ( (areAllUnique [5,1,5,1,2]) = false )
	val test2 = ( (areAllUnique [1,2,3,4,5]) = true )
in
	print ("areAllUnique:\n" ^
	"test1: " ^ Bool.toString(test1) ^ “\n" ^
	"test2: " ^ Bool.toString(test2) ^ “\n")
end
	val _ = test_areAllUnique();

fun test_sum () =
let
	val test1 = ( (sum [[5,5],[6,7]]) = 23 )
	val test2 = ( (sum [[1],[1]]) = 2 )
in
	print ("sum:\n" ^
	"test1: " ^ Bool.toString(test1) ^ “\n" ^
	"test2: " ^ Bool.toString(test2) ^ “\n")
end
	val _ = test_sum();


fun test_sumOption () =
let
	val test1 = ( (sumOption [[SOME(5),SOME(5)],[SOME(6),SOME(7)]]) = SOME(23) )
	val test2 = ( (sumOption [[SOME(1)],[SOME(1)]]) = SOME(2) )
in
	print ("sumOption:\n" ^
	"test1: " ^ Bool.toString(test1) ^ “\n" ^
	"test2: " ^ Bool.toString(test2) ^ “\n")
end
	val _ = test_sumOption();

fun test_sumEither () =
let
	val test1 = ( (sumEither [[IString("5"),IInt(5)],[IInt(6),IString("7")]]) = IInt(23) )
	val test2 = ( (sumEither [[IInt(1)],[IString("1")]]) = IInt(2) )
in
	print ("sum:\n" ^
	"test1: " ^ Bool.toString(test1) ^ “\n" ^
	"test2: " ^ Bool.toString(test2) ^ “\n")
end
	val _ = test_sumEither();


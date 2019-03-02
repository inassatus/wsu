(*Jeonghyeon Woo 011618854*)

fun exists(x,[]) = false (*when the list is empty, it just returns false*)
|exists(x,y::rest)=
	if x=y then true
	else exists(x,rest);
(*it checks every element of the list from the first to the last*)

(*''a means alpha data type that comes through equality testing and 'a means it is just an alpha data type. since we used if else statement rather than tail recursive, the data type is ''a *)

fun isequal(x,y)=
	if x=y then 1
	else 0;
(*this function returns 1 when x=y is true but 2 when it is false*)
fun counting(x,[]) = 0 (*When the list is empty, it just returns 0*)
|counting(x,y::rest)=isequal(x,y)+counting(x,rest);
fun countInList x y = counting(x,y); (*it calls counting function which is recursive*)

(*this returns list that contains x when there are more or equal number of x in the rest list of first parameter*)
fun getdiff(x,y,z)=
	if counting(x,y)<counting(x,z) then []
	else x::[];
fun listDiff(x::rest, y)=
	if rest=[] then getdiff(x,rest,y)
	else getdiff(x,rest,y)@listDiff(rest,y);

(*this function does recursion until the list is emptied or y becomes 0*)
fun adduntil_n([],y) = []
|adduntil_n(x::rest,y) =
	if y=0 then []
	else x::adduntil_n(rest,y-1);
fun firstN x y = adduntil_n(x,y); (*it calls recursion to get list*)

(*getbus makes input of parameter y, then getroute changes it into string and list parameter by processing pattern*)
fun getroute(x,(y,[]))=[]
|getroute(x,(y,z))=
	if exists(x,z) then y::[]
	else [];
fun getbus(x,[]) = []
|getbus(x,y::rest)=
	if rest=[] then getroute(x,y)
	else getroute(x,y)@getbus(x, rest);
fun busFinder x y =getbus(x,y);

(*For my program, data type of getroute is ''a -> ('b * ''a list) list -> 'b list. The reason is that x and z are input parameters of exists function, so they have same data type. y as a parameter variable and as an element appending to return list have the same data type since it was not called with other variables. So, (x,(y,z)) return y while exists(x,z)<-''a and y <-'b.*)

fun recip(x)=1.0/x;
(*return reciprocal*)
fun adduprecip([])=0.0
|adduprecip(x::rest)=recip(x)+adduprecip(rest);
(*sum of reciprocals*)
fun parallelResistors x = recip(adduprecip(x));
(*when it takes 0 ohm resistor, the total resistance becomes 0. 1/0 -> oo, so a+b+c+...+oo=oo, so the sum becomes infinite anyways, and then 1/oo -> 0, so the total becomes 0. When it takes empty list, the total resistance becomes infinite, and the reason is that I set adduprecip([])=0, so 1/0 -> oo *)

fun listlen([])=0
|listlen(x::rest)=1+listlen(rest);
(*current list length*)
fun removeNth(x,[])=[]
|removeNth(0,y)=y
|removeNth(x,y::rest)=removeNth(x-1,rest);
(*remove until the Nth element*)
fun getlist_pair(x,[])=[]
|getlist_pair(x,y::rest)=
	if x=0 then []
	else y::getlist_pair(x-1,rest);
(*get sublists*)
fun getlistmod(x,y)= listlen(x) mod y;
fun pairNleft(x,[])=[]
|pairNleft(x,y)=
	if getlistmod(y,x)=0 then getlist_pair(x,y)::pairNleft(x,removeNth(x,y))
	(*when it makes no leftover list = list_length%x=0*)
	else getlist_pair(getlistmod(y,x),y)::pairNleft(x,removeNth(getlistmod(y,x),y));
	(*list_length%x!=0 then firstly make a list with the leftover elements*)
fun pairNright(x,[])=[]
|pairNright(x,y)=getlist_pair(x,y)::pairNright(x,removeNth(x,y));
(*make lists until leftover elements, then put the leftovers in a list*)




(*test starts*)
fun test_exists () =
let
	val existst1 = (exists(1,[5,1]) = true)
	val existst2 = (exists("lol",["nope","nope"]) = false)
in
	print ("exists:\n" ^
	"test1: " ^ Bool.toString(existst1) ^ “\n" ^
	"test2: " ^ Bool.toString(existst2) ^ “\n")
end
	val _ = test_exists();

fun test_countInList () =
let
	val countInList_1 = ((countInList 1 [1,1,1,1,1]) = 5)
	val countInList_2 = ((countInList "lol" ["nope","nope"]) = 0)
in
	print ("countInList:\n" ^
	"test1: " ^ Bool.toString(countInList_1) ^ “\n" ^
	"test2: " ^ Bool.toString(countInList_2) ^ “\n")
end
	val _ = test_countInList();

fun test_listDiff () =
let
	val listDiff_1 = ( listDiff(["s","b","3"],["1","b"]) = ["s","3"])
	val listDiff_2 = ( listDiff([1,2,3],[5,6])=[1,2,3])
in
	print ("listDiff:\n" ^
	"test1: " ^ Bool.toString(listDiff_1) ^ “\n" ^
	"test2: " ^ Bool.toString(listDiff_2) ^ “\n")
end
	val _ = test_listDiff();

fun test_firstN () =
let
	val firstN_1 = ( (firstN ["s","3","5","7","5"] 2) = ["s","3"])
	val firstN_2 = ( (firstN [1,2,3,4,5,6,7] 3) = [1,2,3])
in
	print ("firstN:\n" ^
	"test1: " ^ Bool.toString(firstN_1) ^ “\n" ^
	"test2: " ^ Bool.toString(firstN_2) ^ “\n")
end
	val _ = test_firstN();

fun test_busFinder () =
let
 	val buses =
[("Lentil",["Chinook", "Orchard", "Valley", "Emerald","Providence", "Stadium",
"Main", "Arbor", "Sunnyside", "Fountain", "Crestview", "Wheatland", "Walmart",
"Bishop", "Derby", "Dilke"]),
("Wheat",["Chinook", "Orchard", "Valley", "Maple","Aspen", "TerreView", "Clay",
"Dismores", "Martin", "Bishop", "Walmart", "PorchLight", "Campus"]),
("Silver",["TransferStation", "PorchLight", "Stadium", "Bishop","Walmart",
"Shopco", "RockeyWay"]),
("Blue",["TransferStation", "State", "Larry", "TerreView","Grand", "TacoBell",
"Chinook", "Library"]),
("Gray",["TransferStation", "Wawawai", "Main", "Sunnyside","Crestview",
"CityHall", "Stadium", "Colorado"])]

 	val busFinder_1 = ((busFinder "Colorado" buses) = ["Gray"])
 	val busFinder_2 = ((busFinder "State" buses) = ["Blue"])

in
 	print ("busFinder:\n" ^
 	"test1: " ^ Bool.toString(busFinder_1) ^ "\n" ^
 	"test2: " ^ Bool.toString(busFinder_2) ^ "\n")
end
	val _ = test_busFinder ();

fun test_parallelResistors () =
let
    val parallelResistors_1 = Real.==((parallelResistors [10.0, 10.0, 10.0, 10.0]), 2.5)      val parallelResistors_2 = Real.==((parallelResistors [8.0, 16.0, 4.0, 16.0]), 2.0) 
in     
	print ("parallelResistors:\n" ^ 
	"test1: " ^         
         Bool.toString(parallelResistors_1) ^ "\n" ^ 
    "test2: " ^        
         Bool.toString(parallelResistors_2) ^ "\n") 
end
	val _ = test_parallelResistors ();

fun test_pairNleft () =
let
	val pairNleft_1 = ( pairNleft(4,[1,2,3,4,5,6,7,8,9,10]) = [[1,2],[3,4,5,6],[7,8,9,10]])
	val pairNleft_2 = ( pairNleft(1,[1,2,3,4,5]) = [[1],[2],[3],[4],[5]])
in
	print ("pairNleft:\n" ^
	"test1: " ^ Bool.toString(pairNleft_1) ^ “\n" ^
	"test2: " ^ Bool.toString(pairNleft_2) ^ “\n")
end
	val _ = test_pairNleft();
fun test_pairNright () =
let
	val pairNright_1 = ( pairNright(4,[1,2,3,4,5,6,7,8,9,10]) = [[1,2,3,4],[5,6,7,8],[9,10]])
	val pairNright_2 = ( pairNright(1,[1,2,3,4,5]) = [[1],[2],[3],[4],[5]])
in
	print ("pairNright:\n" ^
	"test1: " ^ Bool.toString(pairNright_1) ^ “\n" ^
	"test2: " ^ Bool.toString(pairNright_2) ^ “\n")
end
	val _ = test_pairNright();
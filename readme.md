; L programming language was under the MIT license.
; Copyright(c) 2021 nu11ptr team.

# Welcome to use L programming language.

L programming language was under the MIT license.
Copyright(c) 2021 nu11ptr team.

**Simplified Chinese:**[简体中文开发说明](readme_zhcn.md)

## Features

### Based on C++ Standard template libraries

You don't need any other library to create a minimal L programming language,such as "iomanip" or "iostream".

### Lite

Actually,the L programming language codes was in only two files:var.h and parse.h .
You can see,it's very small but complete. :)

### Embedding

You can use these two files on your project.
You only need to know these APIs/Classes:

```
parse.h->
	namespace L->
	class L_base : The base of L programming language.Please inherit it to develop. ->
		std::string toString(void) : convert parsed statement to std::string.
		std::string name : the name of the statement.If input is a expression,it will be the raw of expression.
		std::vector<std::string> : the arguments of the statement.
	//actually,the following is not very important for L interpreter developing(if you don't want to remake a L interpreter,check it).If you want to develop your own interpreter,plese delete the following classes and functions.
	class L based on L_base : L interpreter. ->
		public enum Return_Type : The return value's type of function eval(). ->
			Expression_Calc_Value : Calculated value.If you execute a statement,Return_Value's member "tp" will be this.
			Function_Return_Value : Oh,someone is using return statement!He is trying to return a value.Usually,you need to process this.
			Throw_Return_Value : Something went wrong or an error threw and no one caught it.
		public class Return_Value : The return value of function eval(). ->
			public var L_base base : the statement which is interpreter executes.
			public var Return_Type tp : the return value's type.Usually,its value is equal to Expression_Calc_Value,maybe...
			public var Variable::var value : the return value of function eval().
			public var Variable::var scope : the scope of command.
			public var Variable::var all_scope : the scope of global.
			public var Variable::var this_scope : the scope of 'this' object.(Hey,it is very important!or not.You decide.)
		public Return_Value eval(Variable::var scope) : execute the command.Oh,you should give it an object,not array!
		private Return_Value eval(Variable::var& scope,Variable::var& all_scope,Variable::var& this_scope) : This is a private overload.Do you want to develop your own command?Okay!JUST add an 'else if' statement!
		private Return_Object get_object(std::string object,Variable::var& scope,Variable::var& all_scope,Variable::var& this_scope) : gets the value's reference,or just a const value.You will get it's parent(for native function?) and it's 'this' object(Will it be a reference forever?Sure,so you can use it safely).
		//information:
		//usually,you can use get_object(...).getValue() (based on condition get_object(...).tp==is_pointer) to get value's reference.
		//also,you can use get_object(...).getConstValue() to get value(not reference,so you can't set it).
		//you can use get_object(...).getParent() (based on condition get_object(...).tp==is_pointer or you can make sure it is a reference!) or get_object(...).getConstParent() to get the variable's parent.
		//you can use get_object(...).getThis() to get a 'this' object's reference.
		private Variable::var exp_calc(Variable::var exp,Variable::var scope,Variable::var all_scope,Variable::var this_scope) : calculate value.You must to use Variable::parse(expression's string) to build an expression object.IT WILL NOT RETURN A REFERENCE.
		private bool isKeyword(std::string x) : if x is a keyword,it will return true.if you need to develop a new command(keyword),you need to modify 'keyword' list.
		private bool isIdentifier(std::string x) : if x is a identifier,it will return true.
var.h->
	namespace Variable : Variables. ->
		enum var_tp : the variable's type. ->
			Null
			Int
			Boolean
			String
			Array
			Object
			Function
			Expression
		class var : variable. ->
			void remove() : removes this variable.After that,you need to execute its parent's update() function to apply this operation.
			void update() : updates members.
			multi-type... IntValue,BooleanValue,StringValue,ArrayValue,ObjectValue,FunctionValue,ExpressionValue : value of the variable.
			bool isConst : if it equals true,it means this variable is const and you cannot modify.
			std::string toString_nonconst() : convert to std::string without const flag.
			std::string toString() : convert to std::string.
			Variable::var convert(const var_tp& tp) : convert variable to tp.IT MAY THROW EXCEPTIONS.
			multi-type... operator... calculate the expression IN C++.
		var parse(std::string p,bool isc=false,bool forceisc=false)  : parse p to variable.if you want to change it's member isConst,please set isc to you want value,and set forcesisc to true.
		std::string getTypeStr(var_tp x) : get tp's string.
		var_tp getStrType(std::string x) : convert x to tp.
```

### Variables

We have these types:
format: sample -> public id(private id) : can convert to : ...
**null** -> null(Null) : can convert to : nothing.
**1 0xf 1.2** -> int(Int) : can convert to : Boolean.
**true false** -> boolean(Boolean) : can convert to : Int.
**"hello world" "\u0032"** -> string(String) : can convert to : Array.
**\[1,2,3\] \[3,4,5\]** -> array(Array) : can convert to : Object.
**{"object":1}** -> object(Object) : can convert to : nothing.

**tips:**
You can use **const<...>** to make sure a literal has const attribute.

### Commands

These commands are the basic L interpreter supports.
tips:\[required\] \(optional=default\)
**var \[name\]\(,value=null\)** : Defines \[name\] \(to value\) \(if \(value\) is not specified,it will default to null\(is not const value\).
sample\[2\]:

```
var z;#After this,variable 'z' will be null(is not const value).
var x,1;#After this,variable 'x' will be 1(is not const value).
var a,const<1>;#After this,variable 'a' will be const<1>(is const value).
var b,a;#After this,variable 'b' will be 1(is not const value).
var c,const<b>;#After this,variable 'c' will be const<1>(is const value).
```

**mov \[name\],\[value\]** : Set \[name\] to \[value\].CANNOT SET A CONST VARIABLE TO NEW VALUE!
sample\[1\]:

```
var a,1;
var b;#Variable 'b' is default to null.
mov b,a;#Set variable 'b' from variable 'a'.Variable 'b' will be 1.
var c,const<2>;
mov b,c;#Set variable 'b' from variable 'c'.Variable 'b' will be 2(is not const value).
```

**delete \[name\]** : Deletes \[name\].CANNOT DELETE A CONST VARIABLE!
sample\[1\]:

```
var x;
delete x;#okay
#x #it will throw an 'ExpressionError' error,because variable 'x' is not exist anymore.
var y,const<null>;
#delete y; #fail!!!delete statement(command) cannot delete a const variable.
```

**if \[exp\],\[then\]\(,else=\(\)\)** If condition \[exp\] is equal to true,it will execute statement block \[then\].Otherwise,it will execute statement block \(else\) \(if it is specified\).
sample\[1\]:

```
if 1,(throw 1),(throw 2);#it will throw 1.
if 0,(throw 1),(throw 2);#it will throw 2.
```

**switch \[case1\],\[case2\],\[case3\],\[case4\],...,\[default\]** : Switches conditions.
case's format(array):\[exp,fn\]
information\[1\]:If \[exp\] is equal to true,it will execute statement block \[fn\].
information\[2\]:If no statment block uses break command,it will execute statement block \[default\].
sample\[1\]:

```
switch [1==1,(
    throw 1;
)],(
    throw 2;#it will not be executed.
);#throws exception 1
var a;
switch [1==1,(
    mov a,1;
    break;#if you don't add this,default will be executed.
)],(
    mov a,2;
);#a=1
```

**while \[exp\],\[fn\]** : Repeats statement block \[fn\] until the condition \[exp\] is not equal to true.
sample\[1\]:

```
var a,4;
var b,0;
while b!=a,(
    mov b,b+1;
);#it will repeat 4 times.
```

**for \[settings\],\[fn\]** : Repeats statement block \[fn\].
setting's format\(array\):\[start(first time),exp(expression,if it is not equal to true,the loop will not start),routine(executes before once loop)\]
sample\[1\]:

```
var a,4;
var b,0;
for [(var i,0),i!=a,(mov i,i+1)],(
    mov b,b+1
);#after this,variable 'b' will be 4.
```

**continue** : (only can use in **while**,**for**) start next loop.
**break** : (only can use in **switch**,**while**,**for**) jump out from the loop/case.
sample\[1\]:

```
while 1,(
    break;#it is not a dead loop.
)
while 1,(
    continue;#it is a dead loop.
)
```

**return \(value=null\)** : Returns \[value\].
sample\[1\]:

```
return 0;#returns 0.
```

**call \[fn\](,arguments=null,back_to=discard)** : Calls function \[fn\] and set variable 'arguments' to \(arguments\),return value to \(back_to\)(it CANNOT be a const value).
sample\[1\]:

```
var fn,const<(
    if arguments.type!="array"||arguments.length!=1,(
        return 0;
    );
    return arguments[0];
)>
var a;
var b;
call fn,0,a;#a=0
call fn,[1],b;#b=1
```

**throw \[Exception\]** Throws \[Exception\].
sample\[1\]:

```
throw 0;#it will throw 0
```

**catch \[func\],\[ExceptionFunc\]** : If \[func\] throws any Exception,\[ExceptionFunc\] will catch it(Exception is stored in variable 'arguments').
sample\[1\]:

```
var errors_last;
catch (throw "error"),(mov errors_last,arguments);#errors_last="error"
```

### Sample
sample\[a+b\]:
```
var fn,const<(
    if arguments.type!="array"||arguments.length!=2,(
        throw "failed";
    );
    return arguments[0]+arguments[1];
)>;
var a,1;
var b,2;
var final;
call fn,[a,b],final;#final=3
return final;
```
sample\[Fibonacci sequence \(recursion\)\]:
```
var fib,const<(
    if arguments==0||arguments==1,(return arguments;);
    var x;
    var y;
    call fib,arguments-1,x;
    call fib,arguments-2,y;
    return x+y;
)>;
var x;
call fib,10,x;
#x=55
```
sample\[Fibonacci sequence \(not recursion\)\]:
```
var fib,const<(
    var a,[0,1,1];
    var n,arguments;
    if n<0,(throw "failed!";);
    if n>=3,(
        for [(var i,3),i<=n,(mov i,i+1)],(
            mov a[i],a[i-1]+a[i-2];
        );
    );
    return a[n];
)>;
var x;
call fib,10,x;
#x=55
```

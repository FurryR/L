; L语言是在 MIT 开源协议下的。
; Copyright(c) 2021 nu11ptr 团队。

# 欢迎使用 L 编程语言！

L语言是在 MIT 开源协议下的。
Copyright(c) 2021 nu11ptr 团队。

## 特性

### 基于 C++ 标准库

你不需要任何其它的库，就可以创建一个最小的L语言，比如"iomanip"或者"iostream"。

### 轻量化

实际上,L编程语言的代码仅仅在两个文件里:var.h和parse.h .
你可以看到,它非常小巧但是完备。:)

### 嵌入化

你可以在你的项目上使用这些两个文件。
你仅仅需要知道这些接口/类:

```
parse.h->
	namespace L->
	class L_base : L语言的基本。请继承它来开发。 ->
		std::string toString(void) : 转换解析了的语句到std::string。
		std::string name : 语句的名称.如果输入一个表达式，它将为原表达式。
		std::vector<std::string> : 语句的函数。
	//实际上,以下内容对于L解释器开发工作并不是非常重要的(如果你不想要重新制作一个L解释器，看它)。如果你想要开发你自己的基于L语言的解释器，请删除以下的类和函数。
	class L 基于 L_base : L 解释器。 ->
		public enum Return_Type : 函数 eval() 的返回值的类型。 ->
			Expression_Calc_Value : 计算了的值。如果你执行一个语句，Return_Value的成员"tp"将为这个。
			Function_Return_Value : 看起来某个人正在使用返回语句！他正尝试返回一个值。一般来说，你需要处理这个。
			Throw_Return_Value : 一些东西错误了或者一个错误抛了出来并且没有人捕获它。
		public class Return_Value : 函数 eval() 的返回值。 ->
			public var L_base base : 解释器执行的语句。
			public var Return_Type tp : 返回值的类型。一般来说,它的值等于Expression_Calc_Value,大概吧...
			public var Variable::var value : 函数 eval() 返回的值。
			public var Variable::var scope : 命令的作用域。
			public var Variable::var all_scope : 全局作用域
			public var Variable::var 这个_scope : 'this'对象的作用域。(它非常重要！或者不是。你决定。)
		public Return_Value eval(Variable::var scope) : 执行命令。你应该给它一个对象，不是数组！
		private Return_Value eval(Variable::var& scope,Variable::var& all_scope,Variable::var& this_scope) : 这是一个私有重载。你想要开发你自己的命令？行！只要添加一个 'else if' 语句！
		private Return_Object get_object(std::string object,Variable::var& scope,Variable::var& all_scope,Variable::var& this_scope) : 得到值的引用，或者只是一个常量值。你将得到它的上一级(为了 native function 吗？)和它的 'this' 对象(它将会永远是一个引用吗？当然，所以你可以放心使用它)。
		//信息:
		//一般来说,你可以使用get_object(...).getValue() (基于条件 get_object(...).tp==is_pointer 成立) 来得到值的引用。
		//并且,你可以使用 get_object(...).getConstValue() 来得到值(不是引用,所以你不能设置它)。
		//你可以使用 get_object(...).getParent() (基于条件 get_object(...).tp==is_pointer 成立，或者你可以确保它是一个引用！) 或者 get_object(...).getConstParent() 来得到变量的上一级（不是引用）。
		//你可以使用 get_object(...).getThis() 来得到 'this' 对象的引用。
		private Variable::var exp_calc(Variable::var exp,Variable::var scope,Variable::var all_scope,Variable::var this_scope) : 计算值。你必须使用 Variable::parse(表达式的字符串) 来生成一个表达式对象。它将不会返回一个引用。
		private bool isKeyword(std::string x) : 如果x是一个关键字,它将返回true。如果你需要开发一个新的命令(关键字),你需要修改 'keyword' 列表。
		private bool isIdentifier(std::string x) : 如果x是一个标识符,它将返回true。
var.h->
	namespace Variable : 变量类。 ->
		enum var_tp : 变量的类型。 ->
			Null
			Int
			Boolean
			String
			Array
			Object
			Function
			Expression
		class var : 变量主类。 ->
			void remove() : 删除这个变量。这个操作之后，你需要执行它的上一级的 update() 函数来应用这个操作。
			void update() : 更新成员。
			multi-type... IntValue,BooleanValue,StringValue,ArrayValue,ObjectValue,FunctionValue,ExpressionValue : 变量的值。
			bool isConst : 如果它等于true，表示这个变量是不可修改的。
			std::string toString_nonconst() : 转换到不含不可修改标志的std::string。 
			std::string toString() : 转换到std::string。
			Variable::var convert(常量 var_tp& tp) : 转换变量到tp。它可能抛出错误。
			multi-type... operator... 计算在C++中的表达式。
		var parse(std::string p,bool isc=false,bool forceisc=false) :解析p到变量。如果你想要改变它的isConst成员,请设置isc到你想要的值，且设置forcesisc为true。
		std::string getTypeStr(var_tp x) : 得到tp的字符串表示。
		var_tp getStrType(std::string x) : 转换x到tp。
```

### Variables

我们有这些类型:
格式: 例子 -> public id(private id) : 可以转换到 : ...
**null** -> null(Null) : 可以转换到 : nothing.
**1 0xf 1.2** -> int(Int) : 可以转换到 : Boolean.
**true false** -> boolean(Boolean) : 可以转换到 : Int.
**"hello world" "\u0032"** -> string(String) : 可以转换到 : Array.
**\[1,2,3\] \[3,4,5\]** -> array(Array) : 可以转换到 : 对象.
**{"对象":1}** -> object(Object) : 可以转换到 : nothing.

**tips:**
你可以使用 **const<...>** 来确保一个字面量有常量属性。

### Commands

这些命令是基本 L 解释器 支持的.
tips:\[必选的\] \(可选的=默认值\)
**var \[name\]\(,value=null\)** : 定义 \[name\] \(值为 value\)。\(如果 \(value\) 未指定,它将默认为 null\(不是常量值\).
例子\[1\]:

```
var z;#这之后,变量 'z' 将为 null(不是常量值)。
var x,1;#这之后,变量 'x' 将为 1(不是常量值)。
var a,const<1>;#这之后,变量 'a' 将为常量<1>(是常量值)。
var b,a;#这之后,变量 'b' 将为 1(不是常量值)。
var c,const<b>;#这之后,变量 'c' 将为 const<1>(是常量值)。
```

**mov \[name\],\[value\]** : 设置 \[name\] 到 \[value\]。不能设置一个常量值到新的值！
例子\[1\]:

```
var a,1;
var b;#变量 'b' 默认为 null。
mov b,a;#设置变量 'b' 为变量 'a' 的值。变量 'b' 将为1。
var c,const<2>;
mov b,c;#设置变量 'b' 为变量 'c' 的值。变量 'b' 将为 2(不是 常量 值).
```

**delete \[name\]** : 删除 \[name\]。不能删除一个常量！
例子\[1\]:

```
var x;
delete x;#可行
#x #它将抛出一个 'ExpressionError' 错误，因为变量 'x' 不再存在了。
var y,const<null>;
#delete y; #失败!!!删除语句(命令)不能删除一个常量。
```

**if \[exp\],\[then\]\(,else=\(\)\)** 如果条件 \[exp\] 等于 true,它将执行语句块 \[then\].否则，它将执行语句块 \(else\) \(如果它指定了\).
例子\[1\]:

```
if 1,(throw 1),(throw 2);#它将抛出 1.
if 0,(throw 1),(throw 2);#它将抛出 2.
```

**switch \[case1\],\[case2\],\[case3\],\[case4\],...,\[default\]** : 遍历语句。
case的格式(数组):\[exp,fn\]
信息\[1\]:如果 \[exp\] 等于 true,它将执行语句块 \[fn\].
信息\[2\]:如果没有语句块用了 break 命令，它将执行语句块 \[default\].
例子\[1\]:

```
switch [1==1,(
    throw 1;
)],(
    throw 2;#它将不会执行.
);#throws exception 1
switch [1==1,(
    mov a,1;
    break;#如果你不添加这个,default 将会被执行。
)],(
    mov a,2;#它将不会执行.
);#a=1
```

**while \[exp\],\[fn\]** : 重复语句块 \[fn\] 直到条件 \[exp\] 不等于 true.
例子\[1\]:

```
var a,4;
var b,0;
while b!=a,(
    mov b,b+1;
);#它将重复 4 次。
```

**for \[settings\],\[fn\]** : 重复语句块 \[fn\].
setting的格式\(数组\):\[start(第一次执行的),exp(条件，如果不成立则不执行循环),routine(每次循环前执行的)\]
例子\[1\]:

```
var a,4;
var b,0;
for [(var i,0),i!=a,(mov i,i+1)],(
    mov b,b+1
);#这之后,变量 'b' 将为 4.
```

**continue** : (仅仅可以用于 **while**,**for**) 开始下一次循环。
**break** : (仅仅可以用于 **switch**,**while**,**for**) 从循环/case中跳出。
例子\[1\]:

```
while 1,(
    break;#它不是一个死循环。
)
while 1,(
    continue;#它是一个死循环。
)
```

**return \(value=null\)** : 返回 \[value\]。
例子\[1\]:

```
return 0;#返回 0 。
```

**call \[fn\](,arguments=null,back_to=discard)** : 调用函数\[fn\]并设置变量 'arguments' 为\(arguments\),返回值赋值到back_to(它不能为一个常量值)。
例子\[1\]:

```
var fn,const<(
    const arguments.type!="array"||arguments.length!=1,(
        return 0;
    );
    return arguments[0];
)>
var a;
var b;
call fn,0,a;#a=0
call fn,[1],b;#b=1
```

**throw \[Exception\]** 抛出 \[Exception\].
例子\[1\]:

```
throw 0;#它将抛出 0 。
```

**catch \[func\],\[ExceptionFunc\]** : 如果 \[func\] 抛出了任何错误,\[ExceptionFunc\] 将捕获它(错误存放于变量 'arguments')。
例子\[1\]:

```
var errors_last;
catch (throw "错误"),(mov errors_last,arguments);#errors_last="错误"
```

### 例子
例子\[a+b\]:
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
例子\[斐波那契数列\(递归\)\]:
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
例子\[斐波那契数列 \(非循环\)\]:
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

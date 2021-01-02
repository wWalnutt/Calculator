# 编译原理——C++版桌面计算器
## 系统描述
设计并实现一个桌面计算器，支持基本四则运算，关系运算，可编程运算
### 基本功能
（1）以命令行方式解析用户输入的表达式，计算并显示结果；
（2）支持基本的整型和浮点类型数值计算；
（3）支持用变量和存储计算结果、变量引用，变量类型需声明，可根据保存的数据变化；
### 扩展功能
（1）支持输出变量类型
（2）支持输出推导产生式
（3）支持输出符号表
（4）支持输出语法分析动作表
（5）支持输出所有产生式
（6）支持输出文法first集
（7）支持输出文法follow集
### 实现语言和平台
实现语言：C++
实现平台：Visual Stdio2019
## 系统分析与设计
### 桌面计算器概述
通常来说编译器应具有词法分析，语法分析，语义分析，中间代码生成，代码优化，目标代码生成，符号表，错误处理器，这7个部分组成，由于本次实践目标是一个简单可编程桌面计算器，所以省去了中间代码生成，代码优化，目标代码生成，三个部分，并将语法分析与语义分析作为一个整体进行编程。
使用技术：上下文无关文法，自底向上的语法分析，与LR语法分析器结合的SDT
### 词法分析
词法分析是编译器的第一部分，词法分析器读入字符流并将它们组织成为有意义的lexeme序列。对于每个词素，词法分析器产生词法单元作为输出，词法单元格式为：
	<token-name,attribute-value>
本编辑器的词法分析器输出存储在out.xml中
本编辑器词法分析器可识别词素如下表：
### 可识别符号表与运算符优先级表
Type	Attribute-value	Token-name
常量	Id	1
常量	True	2
常量	False	3
常量	Number	4
数据类型	Integer	5
数据类型	Decimal	6
数据类型	Bool	7
非关键字		8
运算符	(	10
运算符	)	11
运算符	!	21
运算符	++	22
运算符	--	23
运算符	*	31
运算符	/	32
运算符	%	33
运算符	+	41
运算符	-	42
运算符	<	51
运算符	<=	52
运算符	>	53
运算符	>=	54
运算符	==	61
运算符	!=	62
运算符	&&	71
运算符	||	81
运算符	=	91
注释	/**/	999
结束	##	1000


1运算符和常量	id，true，false，number，decimal
2括号	()
3一元运算	+, - ！
4算术运算	*,/,%
5算术运算	+,-
6关系运算	<, <=, >, >=
7关系运算	==, !=
8逻辑与	&&
9逻辑或	||
10赋值/存储	=
### 语法分析
语法分析为编译器的第二步，语法分析器使用由词法分析器生成的各个Token结合给定文法进行语法分析，目的是成功由文法推导出可能的句子，常用方法为语法分析树，
本次实践中使用方法为与LR语法分析器结合的SDT，基本思路为，程序先读入给定文法，求出LR分析表，根据分析表和词法分析器给的Token，构造语法分析动作表，并将移入规约动作存入Reduce.xml。（程序中$list命令可以输出语法分析动作表）

### CFG
S->L
L->L=H
L->H
H->H||G
H->G
G->G&&F
G->F
F->F!=E
F->F==E
F->E
E->E<D
E->E<=D
E->E>D
E->E>=D
E->D
D->D-C
D->D+C
D->C
C->C*B
C->C/B
C->C%B
C->B
B->B++
B->B--
B->!B
B->A
A->(H)
A->id
A->true
A->false
A->var
### 语义分析
语义分析为编译器的第三步，语义分析器使用语法分析树和符号表中的信息来检查源程序是否和语言定义的语义一致。
根据移入规约动作与Token序列进行求结果，若有变量则去符号表中寻找值
### 符号表
程序中提供了符号表查看命令$symbols，符号表由语法分析过程中建立，当程序读入decimal或integer时进行符号表写入，初始值设定为0，可以在后续输入中进行变量赋值
## 桌面计算器运行实示例
---------------------------------
编辑器中所有变量都需要定义才可使用，数据类型有integer和decimal
可输入命令如下：默认只输出值
    $type（输出类型和值）
    $induction(输出推导产生式）
    $symbols(输出符号表）
    $default(恢复默认值）
    $first(输出文法first集）
    $follow(输出文法follow集）
    $production(输出所有产生式）
    $help(输出帮助）
    $list(输出语法分析动作表）
    ##(退出）
---------------------------------
>>>12-8*3
-12
>>>decimal a
>>>a=5/6.2
>>>a
0.806452
>>>a>10
false
>>>a&&false
false
>>>$type
$type=1
>>>a
[Decimal, 0.806452]
>>>a=12%5
>>>a
[Decimal, 2]
>>>12-7*5
[Integer, -23]
>>>12.8*(8.7*9.2)
[decimal, 1024.51]
>>>decimal b
>>>b=34.5-6
>>>b
[Decimal, 28.5]
>>>a+b
[decimal, 30.5]
>>>$induction
$induction=1
>>>a+12-5*6
reduce(A->var)
reduce(B->A)
reduce(C->B)
reduce(D->C)
reduce(A->id)
reduce(B->A)
reduce(C->B)
reduce(D->D+C)
reduce(A->id)
reduce(B->A)
reduce(C->B)
reduce(A->id)
reduce(B->A)
reduce(C->C*B)
reduce(D->D-C)
reduce(E->D)
reduce(F->E)
reduce(G->F)
reduce(H->G)
reduce(L->H)
[Integer, -16]
>>>((2-9.8)
reduce(A->id)reduce(B->A)reduce(C->B)reduce(D->C)reduce(A->id)reduce(B->A)reduce(C->B)reduce(D->D-)reduce(E->D)reduce(F->E)reduce(G->F)reduce(H->G)reduce(A->(H))reduce(B->A)reduce(C->B)reduce(D->C)reduce(E->D)reduce(F->E)reduce(G->F)reduce(H->G)
Error [error:#]
>>>12>5
reduce(A->id)reduce(B->A)reduce(C->B)reduce(D->C)reduce(E->D)reduce(A->id)reduce(B->A)reduce(C->B)reduce(D->C)reduce(E->E>D)reduce(F->E)reduce(G->F)reduce(H->G)reduce(L->H)
[Bool, true]
>>>$default
init
>>>a
2
>>>b
28.5
>>>c
Error [undefined identifier:c]
>>>$symbols
符号表
[ decimal a ]
[ decimal b ]
>>>$follow
***************FOLLOW集*******************
FOLLOW(A)={!,#,%,&,),*,+,-,/,<,=,>,|}
FOLLOW(B)={!,#,%,&,),*,+,-,/,<,=,>,|}
FOLLOW(C)={!,#,%,&,),*,+,-,/,<,=,>,|}
FOLLOW(D)={!,#,&,),+,-,<,=,>,|}
FOLLOW(E)={!,#,&,),<,=,>,|}
FOLLOW(F)={!,#,&,),=,|}
FOLLOW(G)={#,&,),=,|}
FOLLOW(H)={#,),=,|}
FOLLOW(L)={#,=}
FOLLOW(S)={#}
>>>$first
****************FIRST集***************************
FIRST(A)={(,f,i,t,v}
FIRST(B)={!,(,f,i,t,v}
FIRST(C)={!,(,f,i,t,v}
FIRST(D)={!,(,f,i,t,v}
FIRST(E)={!,(,f,i,t,v}
FIRST(F)={!,(,f,i,t,v}
FIRST(G)={!,(,f,i,t,v}
FIRST(H)={!,(,f,i,t,v}
FIRST(L)={!,(,f,i,t,v}
FIRST(S)={!,(,f,i,t,v}
>>>$production
S->L
L->L=H
L->H
H->H||G
H->G
G->G&&F
G->F
F->F!=E
F->F==E
F->E
E->E<D
E->E<=D
E->E>D
E->E>=D
E->D
D->D-C
D->D+C
D->C
C->C*B
C->C/B
C->C%B
C->B
B->B++
B->B--
B->!B
B->A
A->(H)
A->id
A->true
A->false
A->var
>>>list
$list=1
>>>4+5-9
steps          |op-stack       input          operation           |state-stack    ACTION         GOTO
1              |#              id+id-id#      shift               |0              S13
2              |#i             d+id-id#       shift               |013            S33
3              |#id            +id-id#        reduce(A->id)       |01333          R27            3
4              |#A             +id-id#        reduce(B->A)        |03             R25            4
5              |#B             +id-id#        reduce(C->B)        |04             R21            5
6              |#C             +id-id#        reduce(D->C)        |05             R17            6
7              |#D             +id-id#        shift               |06             S23
8              |#D+            id-id#         shift               |0623           S13
9              |#D+i           d-id#          shift               |062313         S33
10             |#D+id          -id#           reduce(A->id)       |06231333       R27            3
11             |#D+A           -id#           reduce(B->A)        |06233          R25            4
12             |#D+B           -id#           reduce(C->B)        |06234          R21            42
13             |#D+C           -id#           reduce(D->D+C)      |062342         R16            6
14             |#D             -id#           shift               |06             S24
15             |#D-            id#            shift               |0624           S13
16             |#D-i           d#             shift               |062413         S33
17             |#D-id          #              reduce(A->id)       |06241333       R27            3
18             |#D-A           #              reduce(B->A)        |06243          R25            4
19             |#D-B           #              reduce(C->B)        |06244          R21            43
20             |#D-C           #              reduce(D->D-C)      |062443         R15            6
21             |#D             #              reduce(E->D)        |06             R14            7
22             |#E             #              reduce(F->E)        |07             R9             8
23             |#F             #              reduce(G->F)        |08             R6             9
24             |#G             #              reduce(H->G)        |09             R4             10
25             |#H             #              reduce(L->H)        |010            R2             11
26             |#L             #              Accept              |011            acc
0
>>>##
quit

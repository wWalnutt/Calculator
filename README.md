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


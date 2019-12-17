-- 单行注释

--[[
多行注释
-- ]]


--[[
标示符
    Lua 标示符用于定义一个变量，函数获取其他用户定义的项。标示符以一个字母 A 到 Z 或 a 到 z 或下划线 _ 开头后加上0个或多个字母，下划线，数字（0到9）。
    最好不要使用下划线加大写字母的标示符，因为Lua的保留字也是这样的。
    Lua 不允许使用特殊字符如 @, $, 和 % 来定义标示符。 Lua 是一个区分大小写的编程语言。因此在 Lua 中 Runoob 与 runoob 是两个不同的标示符。以下列出了一些正确的标示符：


全局变量
    在默认情况下，变量总是认为是全局的。
    全局变量不需要声明，给一个变量赋值后即创建了这个全局变量，访问一个没有初始化的全局变量也不会出错，只不过得到的结果是：nil。
--]]

--[[
Lua 数据类型
 Lua 是动态类型语言，变量不要类型定义,只需要为变量赋值。 值可以存储在变量中，作为参数传递或结果返回。

Lua 中有 8 个基本类型分别为：nil、boolean、number、string、userdata、function、thread 和 table。
    数据类型	描述
    nil	        这个最简单，只有值nil属于该类，表示一个无效值（在条件表达式中相当于false）。
    boolean	    包含两个值：false和true。
    number	    表示双精度类型的实浮点数
    string	    字符串由一对双引号或单引号来表示
    function	由 C 或 Lua 编写的函数
    userdata	表示任意存储在变量中的C数据结构
    thread	    表示执行的独立线路，用于执行协同程序
    table	    Lua 中的表（table）其实是一个"关联数组"（associative arrays），数组的索引可以是数字、字符串或表类型。
                在 Lua 里，table 的创建是通过"构造表达式"来完成，最简单构造表达式是{}，用来创建一个空表。

    可以使用 type 函数测试给定变量或者值的类型
    对于全局变量和 table，nil 还有一个"删除"作用，给全局变量或者 table 表里的变量赋一个 nil 值，等同于把它们删掉

    nil 作比较时应该加上双引号 "，原因： type(X)==nil 结果为 false 的原因是因为 type(type(X))==string。

nil（空）
    nil 类型表示一种没有任何有效值，它只有一个值 -- nil，例如打印一个没有赋值的变量，便会输出一个 nil 值

    nil 作比较时应该加上双引号 "
        > type(X)
        nil
        > type(X)==nil
        false
        > type(X)=="nil"
        true
        >

boolean（布尔）
    boolean 类型只有两个可选值：true（真） 和 false（假），Lua 把 false 和 nil 看作是"假"，其他的都为"真"

number（数字）
    Lua 默认只有一种 number 类型 -- double（双精度）类型（默认类型可以修改 luaconf.h 里的定义），以下几种写法都被看作是 number 类型

string（字符串）
    字符串由一对双引号或单引号来表示。
    也可以用 2 个方括号 "[""["   "]""]" 来表示"一块"字符串。
    在对一个数字字符串上进行算术操作时，Lua 会尝试将这个数字字符串转成一个数字
    字符串连接使用的是 ..
    使用 # 来计算字符串的长度，放在字符串前面

    Lua 语言中字符串可以使用以下三种方式来表示：

        单引号间的一串字符。
        双引号间的一串字符。
        "[" "[" 和 "]" "]"  间的一串字符

    转义字符

    Lua 提供了很多的方法来支持字符串的操作

    字符串格式化


table（表）
    在 Lua 里，table 的创建是通过"构造表达式"来完成，最简单构造表达式是{}，用来创建一个空表。也可以在表里添加一些数据，直接初始化表
    Lua 中的表（table）其实是一个"关联数组"（associative arrays），数组的索引可以是数字或者是字符串
    不同于其他语言的数组把 0 作为数组的初始索引，在 Lua 里表的默认初始索引一般以 1 开始
    table 不会固定长度大小，有新数据添加时 table 长度会自动增长，没初始的 table 都是 nil。
        a3 = {}
        for i = 1, 10 do
            a3[i] = i
        end
        a3["key"] = "val"
        print(a3["key"]) -- val
        print(a3["none"]) -- nil

    对 table 的索引使用方括号 []。Lua 也提供了 . 操作
        t[i]
        t.i                 -- 当索引为字符串类型时的一种简化写法
        gettable_event(t,i) -- 采用索引访问本质上是一个类似这样的函数调用

    table 是 Lua 的一种数据结构用来帮助我们创建不同的数据类型，如：数组、字典等。
    Lua table 使用关联型数组，你可以用任意类型的值来作数组的索引，但这个值不能是 nil。
    Lua table 是不固定大小的，你可以根据自己需要进行扩容。
    Lua也是通过table来解决模块（module）、包（package）和对象（Object）的。 例如string.format表示使用"format"来索引table string。

    table(表)的构造
        构造器是创建和初始化表的表达式。表是Lua特有的功能强大的东西。最简单的构造函数是{}，用来创建一个空表。可以直接初始化数组:
            -- 初始化表
            mytable = {}

            -- 指定值
            mytable[1]= "Lua"

            -- 移除引用
            mytable = nil
            -- lua 垃圾回收会释放内存

        当我们为 table a 并设置元素，然后将 a 赋值给 b，则 a 与 b 都指向同一个内存。如果 a 设置为 nil ，则 b 同样能访问 table 的元素。如果没有指定的变量指向a，Lua的垃圾回收机制会清理相对应的内存。


    Table 操作常用的方法


function（函数）
    在 Lua 中，函数是被看作是"第一类值（First-Class Value）"，函数可以存在变量里
    function 可以以匿名函数（anonymous function）的方式通过参数传递
        function factorial1(n)
            if n == 0 then
                return 1
            else
                return n * factorial1(n - 1)
            end
        end
        print(factorial1(5))
        factorial2 = factorial1
        print(factorial2(5))


thread（线程）
    在 Lua 里，最主要的线程是协同程序（coroutine）。它跟线程（thread）差不多，拥有自己独立的栈、局部变量和指令指针，可以跟其他协同程序共享全局变量和其他大部分东西。
    线程跟协程的区别：线程可以同时多个运行，而协程任意时刻只能运行一个，并且处于运行状态的协程只有被挂起（suspend）时才会暂停。

userdata（自定义类型）
    userdata 是一种用户自定义数据，用于表示一种由应用程序或 C/C++ 语言库所创建的类型，可以将任意 C/C++ 的任意数据类型的数据（通常是 struct 和 指针）存储到 Lua 变量中调用。


Lua 数组
    数组，就是相同数据类型的元素按一定顺序排列的集合，可以是一维数组和多维数组。
    Lua 数组的索引键值可以使用整数表示，数组的大小不是固定的。

    一维数组
        一维数组是最简单的数组，其逻辑结构是线性表。一维数组可以用for循环出数组中的元素，如下实例
            array = {"Lua", "Tutorial"}

            for i= 1, 2 do          -- 在 Lua 索引值是以 1 为起始，但你也可以指定 0 开始。
               print(array[i])
            end

    多维数组
        多维数组即数组中包含数组或一维数组的索引键对应一个数组。
        以下是一个三行三列的阵列多维数组：
            -- 初始化数组
            array = {}
            for i=1,3 do
               array[i] = {}
                  for j=1,3 do
                     array[i][j] = i*j
                  end
            end

            -- 访问数组
            for i=1,3 do
               for j=1,3 do
                  print(array[i][j])
               end
            end



Lua 变量
    Lua 变量有三种类型：全局变量、局部变量、表中的域。
    Lua 中的变量全是全局变量，那怕是语句块或是函数里，除非用 local 显式声明为局部变量。
    局部变量的作用域为从声明位置开始到所在语句块结束。
    变量的默认值均为 nil。

赋值语句
    赋值是改变一个变量的值和改变表域的最基本的方法
    Lua 可以对多个变量同时赋值，变量列表和值列表的各个元素用逗号分开，赋值语句右边的值会依次赋给左边的变量
        a, b = 10, 2*x       <-->       a=10; b=2*x

    遇到赋值语句Lua会先计算右边所有的值然后再执行赋值操作，所以我们可以这样进行交换变量的值
        x, y = y, x                     -- swap 'x' for 'y'
        a[i], a[j] = a[j], a[i]         -- swap 'a[i]' for 'a[j]'

    当变量个数和值的个数不一致时，Lua会一直以变量个数为基础采取以下策略
        a. 变量个数 > 值的个数             按变量个数补足nil
        b. 变量个数 < 值的个数             多余的值会被忽略



--[[
print(type("Hello world"))      --> string
print(type(10.4*3))             --> number
print(type(print))              --> function
print(type(type))               --> function
print(type(true))               --> boolean
print(type(nil))                --> nil
print(type(type(X)))            --> string
print("hello world")


tab1 = { key1 = "val1", key2 = "val2", "val3" }
for k, v in pairs(tab1) do
    print(k .. " - " .. v)
end
print("\n")
tab1.key1 = nil -- 删除table中的key1
for k, v in pairs(tab1) do
    print(k .. " - " .. v)
end



function Asset(a,b)
    return {a, b}

end
Assets = {
    Asset( "ATLAS", "images/selection_square.xml" ),
    Asset( "IMAGE", "images/selection_square.tex" ),
}

for k,v in pairs(Assets) do
    print (type(v) .. #v)
    for i,j in pairs(v) do
        print( i .. "--" .. j)
    end

end

--]]


--[[

Lua 循环
while 循环语法：
        while(condition)
        do
           statements
        end
    例子
        a=10
        while( a < 20 )
        do
           print("a 的值为:", a)
           a = a+1
        end

Lua for循环
    Lua 编程语言中 for 循环语句可以重复执行指定语句，重复次数可在 for 语句中控制。
    Lua 编程语言中 for语句有两大类：：
        数值for循环
        泛型for循环

    数值for循环
        Lua 编程语言中数值for循环语法格式:
        for var=exp1,exp2,exp3 do
            <执行体>
        end
        说明: var 从 exp1 变化到 exp2，每次变化以 exp3 为步长递增 var，并执行一次 "执行体"。exp3 是可选的，如果不指定，默认为1

    泛型for循环
        泛型 for 循环通过一个迭代器函数来遍历所有值，类似 java 中的 foreach 语句。
        Lua 编程语言中泛型 for 循环语法格式:
            --打印数组a的所有值
            a = {"one", "two", "three"}
            for i, v in ipairs(a) do
                print(i, v)
            end
        说明: i是数组索引值，v是对应索引的数组元素值。ipairs是Lua提供的一个迭代器函数，用来迭代数组。

    Lua repeat...until 循环
        Lua 编程语言中 repeat...until 循环语法格式:
            repeat
               statements
            until( condition )

    break 语句	退出当前循环或语句，并开始脚本执行紧接着的语句。
    goto 语句	将程序的控制点转移到一个标签处。




Lua 迭代器
    迭代器（iterator）是一种对象，它能够用来遍历标准模板库容器中的部分或全部元素，每个迭代器对象代表容器中的确定的地址。
    在 Lua 中迭代器是一种支持指针类型的结构，它可以遍历集合的每一个元素。

    泛型 for 迭代器
        泛型 for 在自己内部保存迭代函数，实际上它保存三个值：迭代函数、状态常量、控制变量。
        泛型 for 迭代器提供了集合的 key/value 对，语法格式如下：
            for k, v in pairs(t) do
                print(k, v)
            end

无状态的迭代器
    无状态的迭代器是指不保留任何状态的迭代器，因此在循环中我们可以利用无状态迭代器避免创建闭包花费额外的代价。
    每一次迭代，迭代函数都是用两个变量（状态常量和控制变量）的值作为参数被调用，一个无状态的迭代器只利用这两个值可以获取下一个元素。
        这种无状态迭代器的典型的简单的例子是 ipairs，它遍历数组的每一个元素。
        以下实例我们使用了一个简单的函数来实现迭代器，实现 数字 n 的平方：
                function square(iteratorMaxCount,currentNumber)
                   if currentNumber<iteratorMaxCount
                   then
                      currentNumber = currentNumber+1
                   return currentNumber, currentNumber*currentNumber
                   end
                end

                for i,n in square,3,0
                do
                   print(i,n)
                end

    迭代的状态包括被遍历的表（循环过程中不会改变的状态常量）和当前的索引下标（控制变量），ipairs 和迭代函数都很简单，我们在 Lua 中可以这样实现：
        function iter (a, i)
            i = i + 1
            local v = a[i]
            if v then
               return i, v
            end
        end

        function ipairs (a)
            return iter, a, 0
        end

多状态的迭代器
    很多情况下，迭代器需要保存多个状态信息而不是简单的状态常量和控制变量，最简单的方法是使用闭包，还有一种方法就是将所有的状态信息封装到 table 内，将 table 作为迭代器的状态常量，因为这种情况下可以将所有的信息存放在 table 内，所以迭代函数通常不需要第二个参数。
    以下实例我们创建了自己的迭代器：
        array = {"Google", "Runoob"}

        function elementIterator (collection)
           local index = 0
           local count = #collection
           -- 闭包函数
           return function ()
              index = index + 1
              if index <= count
              then
                 --  返回迭代器的当前元素
                 return collection[index]
              end
           end
        end

        for element in elementIterator(array)
        do
           print(element)
        end



Lua 流程控制
    Lua if 语句语法格式如下：
        if(布尔表达式)
        then
           --[ 在布尔表达式为 true 时执行的语句 --]
        end

    Lua if...else 语句语法格式如下：
        if(布尔表达式)
        then
           --[ 布尔表达式为 true 时执行该语句块 --]
        else
           --[ 布尔表达式为 false 时执行该语句块 --]
        end
     if...elseif...else 语句
        Lua if 语句可以与 elseif...else 语句搭配使用, 在 if 条件表达式为 false 时执行 elseif...else 语句代码块，用于检测多个条件语句。
        Lua if...elseif...else 语句语法格式如下：
            if( 布尔表达式 1)
            then
               --[ 在布尔表达式 1 为 true 时执行该语句块 --]

            elseif( 布尔表达式 2)
            then
               --[ 在布尔表达式 2 为 true 时执行该语句块 --]

            elseif( 布尔表达式 3)
            then
               --[ 在布尔表达式 3 为 true 时执行该语句块 --]
            else
               --[ 如果以上布尔表达式都不为 true 则执行该语句块 --]
            end

    Lua if 嵌套语句语法格式如下：
        if( 布尔表达式 1)
        then
           --[ 布尔表达式 1 为 true 时执行该语句块 --]
           if(布尔表达式 2)
           then
              --[ 布尔表达式 2 为 true 时执行该语句块 --]
           end
        end


Lua 函数
    Lua 函数主要有两种用途：
        1.完成指定的任务，这种情况下函数作为调用语句使用；
        2.计算并返回值，这种情况下函数作为赋值语句的表达式使用。

函数定义
    Lua 编程语言函数定义格式如下：
        optional_function_scope function function_name( argument1, argument2, argument3..., argumentn)
            function_body
            return result_params_comma_separated
        end
    解析：
        optional_function_scope: 该参数是可选的制定函数是全局函数还是局部函数，未设置该参数默认为全局函数，如果你需要设置函数为局部函数需要使用关键字 local。

        function_name: 指定函数名称。

        argument1, argument2, argument3..., argumentn: 函数参数，多个参数以逗号隔开，函数也可以不带参数。

        function_body: 函数体，函数中需要执行的代码语句块。

        result_params_comma_separated: 函数返回值，Lua语言函数可以返回多个值，每个值以逗号隔开。

多返回值
    Lua函数可以返回多个结果值，比如string.find，其返回匹配串"开始和结束的下标"（如果不存在匹配串返回nil）。
         s, e = string.find("www.runoob.com", "runoob")  --  5    10

可变参数
    Lua 函数可以接受可变数目的参数，和 C 语言类似，在函数参数列表中使用三点 ... 表示函数有可变的参数。
    例子1：
            function add(...)
            local s = 0
              for i, v in ipairs{...} do   --> {...} 表示一个由所有变长参数构成的数组
                s = s + v
              end
              return s
            end
            print(add(3,4,5,6,7))  --->25

    例子2：我们可以将可变参数赋值给一个变量。
        function average(...)
           result = 0
           local arg={...}    --> arg 为一个表，局部变量
           for i,v in ipairs(arg) do
              result = result + v
           end
           print("总共传入 " .. #arg .. " 个数")
           return result/#arg
        end
        print("平均值为",average(10,5,3,4,5,6))

    例子3： 我们也可以通过 select("#",...) 来获取可变参数的数量:
        function average(...)
           result = 0
           local arg={...}
           for i,v in ipairs(arg) do
              result = result + v
           end
           print("总共传入 " .. select("#",...) .. " 个数")
           return result/select("#",...)
        end

        print("平均值为",average(10,5,3,4,5,6))
--]]


--[[
运算符是一个特殊的符号，用于告诉解释器执行特定的数学或逻辑运算。Lua提供了以下几种运算符类型：
        算术运算符
        关系运算符
        逻辑运算符
        其他运算符

算术运算符
    下表列出了 Lua 语言中的常用算术运算符，设定 A 的值为10，B 的值为 20：
        操作符	描述	实例
        +	加法	A + B 输出结果 30
        -	减法	A - B 输出结果 -10
        *	乘法	A * B 输出结果 200
        /	除法	B / A w输出结果 2
        %	取余	B % A 输出结果 0
        ^	乘幂	A^2 输出结果 100
        -	负号	-A 输出结果 -10

关系运算符
    下表列出了 Lua 语言中的常用关系运算符，设定 A 的值为10，B 的值为 20：
    操作符	描述	实例
    ==	等于，检测两个值是否相等，相等返回 true，否则返回 false	(A == B) 为 false。
    ~=	不等于，检测两个值是否相等，相等返回 false，否则返回 true	(A ~= B) 为 true。
    >	大于，如果左边的值大于右边的值，返回 true，否则返回 false	(A > B) 为 false。
    <	小于，如果左边的值大于右边的值，返回 false，否则返回 true	(A < B) 为 true。
    >=	大于等于，如果左边的值大于等于右边的值，返回 true，否则返回 false	(A >= B) 返回 false。
    <=	小于等于， 如果左边的值小于等于右边的值，返回 true，否则返回 false	(A <= B) 返回 true。

逻辑运算符
    下表列出了 Lua 语言中的常用逻辑运算符，设定 A 的值为 true，B 的值为 false：
    操作符	描述	实例
    and	逻辑与操作符。 若 A 为 false，则返回 A，否则返回 B。	(A and B) 为 false。
    or	逻辑或操作符。 若 A 为 true，则返回 A，否则返回 B。	(A or B) 为 true。
    not	逻辑非操作符。与逻辑运算结果相反，如果条件为 true，逻辑非为 false。	not(A and B) 为 true。

其他运算符
    下表列出了 Lua 语言中的连接运算符与计算表或字符串长度的运算符：
    操作符	描述	实例
    ..	连接两个字符串	a..b ，其中 a 为 "Hello " ， b 为 "World", 输出结果为 "Hello World"。
    #	一元运算符，返回字符串或表的长度。	#"Hello" 返回 5

运算符优先级
    从高到低的顺序：
    ^
    not    - (unary)字符串
    *      /
    +      -
    ..
    <      >      <=     >=     ~=     ==
    and
    or

    除了 ^ 和 .. 外所有的二元运算符都是左连接的。
        a+i < b/2+1          <-->       (a+i) < ((b/2)+1)
        5+x^2*8              <-->       5+((x^2)*8)
        a < y and y <= z     <-->       (a < y) and (y <= z)
        -x^2                 <-->       -(x^2)
        x^y^z                <-->       x^(y^z)


Lua 模块与包
    模块类似于一个封装库，从 Lua 5.1 开始，Lua 加入了标准的模块管理机制，可以把一些公用的代码放在一个文件里，以 API 接口的形式在其他地方调用，有利于代码的重用和降低代码耦合度。

    Lua 的模块是由变量、函数等已知元素组成的 table，因此创建一个模块很简单，就是创建一个 table，然后把需要导出的常量、函数放入其中，最后返回这个 table 就行。
        以下为创建自定义模块 module.lua，文件代码格式如下：
            -- 文件名为 module.lua
            -- 定义一个名为 module 的模块
            module = {}

            -- 定义一个常量
            module.constant = "这是一个常量"

            -- 定义一个函数
            function module.func1()
                io.write("这是一个公有函数！\n")
            end

            local function func2()
                print("这是一个私有函数！")
            end

            function module.func3()
                func2()
            end

            return module


require 函数
    Lua提供了一个名为require的函数用来加载模块。要加载一个模块，只需要简单地调用就可以了。
        require("<模块名>")
        require "<模块名>"

    执行 require 后会返回一个由模块常量或函数组成的 table，并且还会定义一个包含该 table 的全局变量。
        -- test_module.lua 文件
        -- module 模块为上文提到到 module.lua
        require("module")
        print(module.constant)
        module.func3()

加载机制
    对于自定义的模块，模块文件不是放在哪个文件目录都行，函数 require 有它自己的文件路径加载策略，它会尝试从 Lua 文件或 C 程序库中加载模块。
    require 用于搜索 Lua 文件的路径是存放在全局变量 package.path 中，当 Lua 启动后，会以环境变量 LUA_PATH 的值来初始这个环境变量。如果没有找到该环境变量，则使用一个编译时定义的默认路径来初始化。
    当然，如果没有 LUA_PATH 这个环境变量，也可以自定义设置，在当前用户根目录下打开 .profile 文件（没有则创建，打开 .bashrc 文件也可以），例如把 "~/lua/" 路径加入 LUA_PATH 环境变量里：

C 包
    Lua和C是很容易结合的，使用 C 为 Lua 写包。
    与Lua中写包不同，C包在使用以前必须首先加载并连接，在大多数系统中最容易的实现方式是通过动态连接库机制。
    Lua在一个叫loadlib的函数内提供了所有的动态连接的功能。这个函数有两个参数:库的绝对路径和初始化函数。所以典型的调用的例子如下:
        local path = "/usr/local/lua/lib/libluasocket.so"
        local f = loadlib(path, "luaopen_socket")


Lua 元表(Metatable)
    存在缘由
        在 Lua table 中我们可以访问对应的key来得到value值，但是却无法对两个 table 进行操作。
        因此 Lua 提供了元表(Metatable)，允许我们改变table的行为，每个行为关联了对应的元方法。
            例如，使用元表我们可以定义Lua如何计算两个table的相加操作a+b。
            当Lua试图对两个表进行相加时，先检查两者之一是否有元表，之后检查是否有一个叫"__add"的字段，若找到，则调用对应的值。
            "__add"等即时字段，其对应的值（往往是一个函数或是table）就是"元方法"。

    有两个很重要的函数来处理元表：
        setmetatable(table,metatable): 对指定 table 设置元表(metatable)，如果元表(metatable)中存在 __metatable 键值，setmetatable 会失败。
        getmetatable(table): 返回对象的元表(metatable)。


    __index 元方法:
            mytable = setmetatable({key1 = "value1"}, {
                __index = function(mytable, key)
                    if key == "key2" then
                        return "metatablevalue"
                    else
                        return nil
                    end
                end
            })

            print(mytable.key1,mytable.key2)


    Lua 查找一个表元素时的规则，其实就是如下 3 个步骤:
        1.在表中查找，如果找到，返回该元素，找不到则继续
        2.判断该表是否有元表，如果没有元表，返回 nil，有元表则继续。
        3.判断元表有没有 __index 方法，如果 __index 方法为 nil，则返回 nil；如果 __index 方法是一个表，则重复 1、2、3；如果 __index 方法是一个函数，则返回该函数的返回值。

    __newindex 元方法
        __newindex 元方法用来对表更新，__index则用来对表访问 。
        当你给表的一个缺少的索引赋值，解释器就会查找__newindex 元方法：如果存在则调用这个函数而不进行赋值操作。


Lua 协同程序(coroutine)
    线程和协同程序区别
        线程与协同程序的主要区别在于，一个具有多个线程的程序可以同时运行几个线程，而协同程序却需要彼此协作的运行。
        在任一指定时刻只有一个协同程序在运行，并且这个正在运行的协同程序只有在明确的被要求挂起的时候才会被挂起。
        协同程序有点类似同步的多线程，在等待同一个线程锁的几个线程有点类似协同。

        基本语法
            coroutine.create()	创建 coroutine，返回 coroutine， 参数是一个函数，当和 resume 配合使用的时候就唤醒函数调用
            coroutine.resume()	重启 coroutine，和 create 配合使用
            coroutine.yield()	挂起 coroutine，将 coroutine 设置为挂起状态，这个和 resume 配合使用能有很多有用的效果
            coroutine.status()	查看 coroutine 的状态
            注：coroutine 的状态有三种：dead，suspended，running，具体什么时候有这样的状态请参考下面的程序
            coroutine.wrap（）	创建 coroutine，返回一个函数，一旦你调用这个函数，就进入 coroutine，和 create 功能重复
            coroutine.running()	返回正在跑的 coroutine，一个 coroutine 就是一个线程，当使用running的时候，就是返回一个 corouting 的线程号

                    local newProductor

                    function productor()
                         local i = 0
                         while true do
                              i = i + 1
                              send(i)     -- 将生产的物品发送给消费者
                         end
                    end

                    function consumer()
                         while true do
                              local i = receive()     -- 从生产者那里得到物品
                              print(i)
                         end
                    end

                    function receive()
                         local status, value = coroutine.resume(newProductor)
                         return value
                    end

                    function send(x)
                         coroutine.yield(x)     -- x表示需要发送的值，值返回以后，就挂起该协同程序
                    end

                    -- 启动程序
                    newProductor = coroutine.create(productor)
                    consumer()


Lua 文件 I/O
    Lua I/O 库用于读取和处理文件。分为简单模式（和C一样）、完全模式。

        简单模式（simple model）拥有一个当前输入文件和一个当前输出文件，并且提供针对这些文件相关的操作。
        完全模式（complete model） 使用外部的文件句柄来实现。它以一种面对对象的形式，将所有的文件操作定义为文件句柄的方法

简单模式
    简单模式使用标准的 I/O 或使用一个当前输入文件和一个当前输出文件。
    以下为 file.lua 文件代码，操作的文件为test.lua(如果没有你需要创建该文件)，代码如下：
            -- 以只读方式打开文件
            file = io.open("test.lua", "r")

            -- 设置默认输入文件为 test.lua
            io.input(file)

            -- 输出文件第一行
            print(io.read())

            -- 关闭打开的文件
            io.close(file)

            -- 以附加的方式打开只写文件
            file = io.open("test.lua", "a")

            -- 设置默认输出文件为 test.lua
            io.output(file)

            -- 在文件最后一行添加 Lua 注释
            io.write("--  test.lua 文件末尾注释")

            -- 关闭打开的文件
            io.close(file)


完全模式
    通常我们需要在同一时间处理多个文件。我们需要使用 file:function_name 来代替 io.function_name 方法。以下实例演示了如何同时处理同一个文件:
        -- 以只读方式打开文件
        file = io.open("test.lua", "r")

        -- 输出文件第一行
        print(file:read())

        -- 关闭打开的文件
        file:close()

        -- 以附加的方式打开只写文件
        file = io.open("test.lua", "a")

        -- 在文件最后一行添加 Lua 注释
        file:write("--test")

        -- 关闭打开的文件
        file:close()

--]]



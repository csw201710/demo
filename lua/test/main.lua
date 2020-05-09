
local function testDataType()
    a = nil
    b = true
    c = 1.0
    d = '123'
    e = function()
        printf("this is e function!")
    end
    -- f = userdata;
    -- g = thread
    h = {"abc",1}
    print(type("Hello world"))      --> string
    print(type(10.4*3))             --> number
    print(type(print))              --> function
    print(type(type))               --> function
    print(type(true))               --> boolean
    print(type(nil))                --> nil
    print(type(type(X)))            --> string


    -- boolean
    print(type(true))
    print(type(false))
    print(type(nil))

    if false or nil then
        print("至少有一个是 true")
    else
        print("false 和 nil 都为 false")
    end

    if 0 then
        print("数字 0 是 true")
    else
        print("数字 0 为 false")
    end


    --[[   string   --]]
    html = [[
<html>
<head></head>
<body>
    <a href="http://www.runoob.com/">菜鸟</a>
</body>
</html>
]]
    print(html)


    --在对一个数字字符串上进行算术操作时，Lua 会尝试将这个数字字符串转成一个数字
    print("2" + 6)


    -- Lua 里，table 的创建是通过"构造表达式"来完成，最简单构造表达式是{}，用来创建一个空表。也可以在表里添加一些数据，直接初始化表
    a = {}
    a["key"] = "value"
    key = 10
    a[key] = 22
    a[key] = a[key] + 11
    for k, v in pairs(a) do
        print(k .. " : " .. v)
    end




    --  Lua 中，函数是被看作是"第一类值（First-Class Value）"，函数可以存在变量里
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




    a = "Hello "
    b = "World"

    print("连接字符串 a 和 b ", a..b )

    print("b 字符串长度 ",#b )

    print("字符串 Test 长度 ",#"Test" )

    a = 20
    b = 10
    c = 15
    d = 5

    e = (a + b) * c / d;-- ( 30 * 15 ) / 5
    print("(a + b) * c / d 运算值为  :",e )

    e = ((a + b) * c) / d; -- (30 * 15 ) / 5
    print("((a + b) * c) / d 运算值为 :",e )

    e = (a + b) * (c / d);-- (30) * (15/5)
    print("(a + b) * (c / d) 运算值为 :",e )

    e = a + (b * c) / d;  -- 20 + (150/5)
    print("a + (b * c) / d 运算值为   :",e )

end

local function testPairsAndIpairs()
    local tabFiles = {
        [1] = "test2",
        [6] = "test3",
        [4] = "test1"
    }
    print("ipairs : \n")
    for k, v in ipairs(tabFiles) do    --输出"test2",在key等于2处断开
        print(k, v)
    end
    print("pairs : \n")
    for k, v in pairs(tabFiles) do  --[[什么都没输出，为什么？因为控制变量初始值是按升序来遍历的，当key为1时，value为nil，此时便停止了遍历， 所有什么结果都没输出]]--
        print(k, v)
    end
end



local function testCycle()

    -- while
    i = 1
    while(i<3)
    do
        print("while i = " .. i)
        i = i + 1

    end



    -- for

    for i=3,1,-1 do
        print(i)
    end

    a = {"one", "two", "three"}
    for i, v in ipairs(a) do
        print(i, v)
    end

    --[ 变量定义 --]
    a = 0
    --[ 执行循环 --]
    repeat
        print("a的值为:", a)
        a = a + 1
    until( a > 3 )

    --goto Label
    --print("not to be shown")
	--:: Label ::
    --print("goto here");

end

local function testControl()
    --[ 定义变量 --]
    a = 100

    --[ 检查布尔条件 --]
    if( a == 10 )
    then
        --[ 如果条件为 true 打印以下信息 --]
        print("a 的值为 10" )
    elseif( a == 20 )
    then
        --[ if else if 条件为 true 时打印以下信息 --]
        print("a 的值为 20" )
    elseif( a == 30 )
    then
        --[ if else if condition 条件为 true 时打印以下信息 --]
        print("a 的值为 30" )
    else
        --[ 以上条件语句没有一个为 true 时打印以下信息 --]
        print("没有匹配 a 的值" )
    end
    print("a 的真实值为: ", a )


end


local function testSelect()

    function average(...)
        result = 0
        local arg={...}    --> arg 为一个表，局部变量
        for i,v in ipairs(arg) do
            result = result + v
        end
        print("总共传入 " .. #arg .. " 个数")
        return result/#arg
    end

    function average_select(...)
        --[[
        select('#', …) 返回可变参数的长度
        select(n, …)
         --]]
        result = 0
        local arg={...}
        for i,v in ipairs(arg) do
            result = result + v
        end
        print("总共传入 " .. select("#",...) .. " 个数")
        return result/select("#",...)
    end

    print("average平均值为",average(10,5,3,4,5,6))
    print("average_select平均值为",average_select(10,5,3,4,5,6))


    do
        function foo(...)
            for i = 1, select('#', ...) do  -->获取参数总数
                local arg = select(i, ...); -->读取参数
                print("arg", arg);
            end
        end

        foo(1, 2, 3, 4);
    end

end



local function testLogic()
    a = true
    b = true

    if ( a and b )
    then
        print("a and b - 条件为 true" )
    end

    if ( a or b )
    then
        print("a or b - 条件为 true" )
    end

    print("---------分割线---------" )

    -- 修改 a 和 b 的值
    a = false
    b = true

    if ( a and b )
    then
        print("a and b - 条件为 true" )
    else
        print("a and b - 条件为 false" )
    end

    if ( not( a and b) )
    then
        print("not( a and b) - 条件为 true" )
    else
        print("not( a and b) - 条件为 false" )
    end
end


local function testLocal()
    function test0001()
        test000001 = "local__test000001"
        local test000002 = "local__test000002"

    end

    test0001()
    print(test000001)
    print(test000002)
end

local function testFunction()
    local shape = {side = 4}
    function shape.set_side(shape, side)
        shape.side = side
    end

    function shape.print_area(shape)
        print(shape.side * shape.side)
    end

    shape:set_side(5)
    print(shape.side)
    shape:print_area()

    shape.set_side(shape,10)
    print(shape.side)
    shape.print_area(shape)
end

local function testFunction2()
    local shape = {side = 4}
    function shape:set_side(side)
        self.side = side
    end

    function shape:print_area()
        print(self.side * self.side)
    end

    print(shape.side)

    shape:set_side(5)
    print(shape.side)
    shape:print_area()


end
require("io")
require("string")

local function testPrint()
    function eraseTerminal()
        io.write("\27[2J")
    end
    -- writes an '*' at column 'x' , 'row y'
    function mark (x,y)
        local s = string.format("\27[%d;%dH*",y,x)
        io.write(s)
    end
    -- Terminal size
    TermSize = {w = 80, h = 24}
    -- plot a function
    -- (assume that domain and image are in the range [-1,1])
    function plot (f)
        eraseTerminal()
        for i=1,TermSize.w do
            local x = (i/TermSize.w)*2 - 1
            local y = (f(x) + 1)/2 * TermSize.h
            mark(i, y)
        end
        io.read() -- wait before spoiling the screen
    end
    plot(function (x) return math.sin(x*2*math.pi) end)
end

local function testIter()
    function list_iter (t)
        local i = 0
        return function ()
            i = i + 1
            if i <= #t  then return t[i] end
        end
    end
    t = {10, 20, 30}
    iter = list_iter(t) -- creates the iterator
    while true do
        local element = iter() -- calls the iterator
        if element == nil then break end
        print(element)

    end
    t = {10, 20, 30}
    for element in list_iter(t) do
        print(element)
    end
end

local function testIO()
	print "enter a number:"
	n = io.read("*number")
	if not n then error("invalid inputxxxx") end
	print("read number:", n)
end

--[[
multiple Lua VMs detected: 需要使用dll lua库的形式 
--]]
local function testLoadLib()
	local path = 'D:\\systemp\\t123456789\\lua\\LTest\\Debug\\luaDll.dll'
	--local f = package.loadlib(path, "luaopen_mylib")
	--print("f:",type(f)); f()
	local f = package.loadlib(path, "luaopen_ldll")
	print("f:",type(f)); f()
	
	local ldll = require("ldll")
	print("ldll:", ldll.version())
	ldll.show()
end



local function testCall()
	local  status = xpcall(testLoadLib, function() print(debug.traceback()) end)
	if(status)
	then
		print("testCall ok")
	else
		print("testCall failed")
	end
end


local function testCoroutine()
	local co = coroutine.create(function ()
		for i = 1, 2 do
			print("co",i)
			coroutine.yield(co)
		end
	end)
	print('---------------------------------1')
	print(coroutine.resume(co))--1
	print(coroutine.status(co))
	
	print('---------------------------------2')
	print(coroutine.resume(co))--2
	print(coroutine.status(co))
	
	print('---------------------------------3')
	print(coroutine.resume(co))
	print(coroutine.status(co))
	
	print('---------------------------------4')
	print(coroutine.resume(co))
	print(coroutine.status(co))
	
	print('---------------------------------')
	co = coroutine.create(function (a,b,c)
		print("co2", a,b,c)
	end)
	coroutine.resume(co, 1, 2, 3)
	
	
	print('---------------------------------')
	co = coroutine.create(function (a,b)
		coroutine.yield(a + b, a - b)
	end)
	print(coroutine.resume(co, 20, 10)) --> true 30 10
	print('---------------------------------')
	co = coroutine.create(function ()
		return 6,7
	end)	
	print(coroutine.resume(co))
	
	print('---------------------------------')
	function receive (prod)
		local status, value = coroutine.resume(prod)
		return value
	end
	

	
	function send (x)
		coroutine.yield(x)
	end
	
	function producer ()
		return coroutine.create( function ()
			while true do
				local x = io.read() -- produce new value
				send(x)
			end
		end)
	end	
	
	function filter (prod)
		return coroutine.create(function ()
				local line = 1
				while true do
					local x = receive(prod) -- get new value
					x = string.format("%5d %s", line, x)
					send(x) -- send it to consumer
					line = line + 1
				end
		end)
	end
	
	
	function consumer (prod)
		while true do
			local x = receive(prod) -- get new value
			--io.write(x, "\n") -- consume new value
			print("x:",x)
		end
	end
	--consumer(filter(producer()))
	
	
	
	
	function permgen (a, n)
		if n == 0 then
			coroutine.yield(a)
		else
			for i=1,n do
				-- put i-th element as the last one
				a[n], a[i] = a[i], a[n]
				-- generate all permutations of the other elements
				permgen(a, n - 1)
				-- restore i-th element
				a[n], a[i] = a[i], a[n]
			end
		end
	end
	function printResult (a)
		for i,v in ipairs(a) do
			io.write(v, " ")
		end
		io.write("\n")
	end
	function perm (a)
		local n = #a
		--local co = coroutine.create(function () permgen(a, n) end)
		--return function () -- iterator
		--	local code, res = coroutine.resume(co)
		--	return res
		--end
		--create相同的是，wrap创建一个协同程序；不同的是wrap不返回协同本身，而是返回一个函数，当这个函数被调用时将resume协同
		--wrap函数被调用时将resume协同。wrap中resume协同的时候不会返回错误代码作为第一个返回结果，一旦有错误发生，将抛出错误
		return coroutine.wrap(function () permgen(a, n) end)
	end
	
	for p in perm{"1", "2", "3"} do
		printResult(p)
	end

	
	
	
end
	
	

--[[
require用于使用模块，module(5.1版本)用于创建模块
另一个跟 lua 的 module 模块相关需要注意的点是，当 lua_code_cache on 开启时， require 加载的模块是会被缓存下来的，这样我们的模块就会以最高效的方式运行，直到被显式地调用如下语句：
package.loaded["square"] = nil


冒号和点号的区别:
	冒号操作会带入一个self参数，用来代表自己。而逗号操作，只是内容的展开。
	冒号的操作，只有当变量是类对象时才需要。有关如何使用 Lua 构造类，大家可参考相关章节。
--]]
local function testModule()
	print("-------------------------------")
	local jctest = require("MT01")
	jctest:Func1()
	jctest.num = 100;local jctest2 = require("MT01");print(jctest2.num)
	jctest.Func2();
	print(_G.JCTest.Func1())
	print(jctest.print)
	print("-------------------------------")
	
	
	--module
	if(_VERSION == "Lua 5.2" or _VERSION == "Lua 5.1")
	then
		
		local mt02 = require("MT02")
		mt02.printB()
		mt02.print("mt02.print(...)")
		print("-------------------------------")
	else
		print("[warning] not test module function",_VERSION)
	end
	
	
	local square = require "square" 
	
	local s1 = square:new(1, 2)
	print(s1:get_square())          --output: 2
	print(s1:get_circumference())   --output: 6
end

--[[
setmetatable(table, metatable)
Lua中的这个函数只能设置所给表的元表，如果要想改变其他类型的元表只能在C语言中实现，不能通过lua函数设置，如果第二个参数matetable为nil，则将table的元表设置为空。如果原来table的元表中包含一个"__metatable"字段，那么函数将会抛出一个错误。

--]]
local function testMetalable()
	mytable = setmetatable({key1 = "value1"}, {
	  __index = function(mytable, key)
	    if key == "key2" then
	      return "metatablevalue"
	    else
	      return nil
	    end
	  end
	})
	
	--print(mytable.key1,mytable.key2)
	--local a={}
	--print(getmetatable(a), getmetatable(mytable))
	
	
	
	Set = {}
	Set.mt = {}
	function Set.new (t)
		local set = {}
		setmetatable(set, Set.mt)
		for _, l in ipairs(t) do set[l] = true end
		return set
	end
	
	function Set.union (a,b)
		local res = Set.new{}
		for k in pairs(a) do res[k] = true end
		for k in pairs(b) do res[k] = true end
		return res
	end
	Set.mt.__add = Set.union
	
	function Set.intersection (a,b)
		local res = Set.new{}
		for k in pairs(a) do
			res[k] = b[k]
		end
		return res
	end
	Set.mt.__mul = Set.intersection
	
	
	Set.mt.__le = function (a,b) -- set containment
		for k in pairs(a) do
			if not b[k] then return false end
		end
		return true
	end
	Set.mt.__lt = function (a,b)
		return a <= b and not (b <= a)
	end
	Set.mt.__eq = function (a,b)
		return a <= b and b <= a
	end
	
	function Set.tostring (set)
		local s = "{"
		local sep = ""
		for e in pairs(set) do
			s = s .. sep .. e
			sep = ", "
		end
		return s .. "}"
	end
	Set.mt.__tostring = Set.tostring
		

	s1 = Set.new{10, 20, 30, 50}
	s2 = Set.new{30, 1}
	--print(getmetatable(s1)) --> table: 00672B60
	--print(getmetatable(s2))	
	
	s3 = s1 + s2
	print(s3)
	
	print((s1 + s2)*s1)
	
	s1 = Set.new{2, 4}
	s2 = Set.new{4, 10, 2}
	print("s1 <= s2", s1 <= s2) --> true
	print("s1 < s2", s1 < s2) --> true
	print("s1 >= s1", s1 >= s1) --> true
	print("s1 > s1", s1 > s1) --> false
	print("s1 == s2", s1 == s2) --> true
	print("s1 == s2 * s1", s1 == s2 * s1) --> true
	
	
	local Window = {}
	-- create the prototype with default values
	Window.prototype = {x=0, y=0, width=100, height=100, }
	-- create a metatable
	Window.mt = {}
	Window.mt.__index = Window.prototype
	-- declare the constructor function
	function Window.new (o)
		setmetatable(o, Window.mt)
		return o
	end
	local w = Window.new{x = 20, y = 199}
	print(w["y"])
	
	-- __newindex metamethod用来对表更新，__index则用来对表访问
	-- 当你给表的一个缺少的域赋值，解释器就会查找__newindex metamethod：如果存在则调用这个函数而不进行赋值操作
	-- 像__index一样，如果metamethod是一个表，解释器对指定的那个表，而不是原始的表进行赋值操作。
	-- 有一个raw函数可以绕过metamethod：调用rawset(t,k,v)不掉用任何metamethod对表t的k域赋值为v
	
	
	local t = {} -- original table (created somewhere)
	-- keep a private access to original table
	local _t = t
	-- create proxy
	local t = {}
	-- create metatable
	local mt = {
		__index = function (t,k)
			print("*access to element " .. tostring(k))
			return _t[k] -- access the original table
			end,
			__newindex = function (t,k,v)
			print("*update of element " .. tostring(k) ..
			" to " .. tostring(v))
			_t[k] = v -- update original table
		end
	}
	setmetatable(t, mt)
	
	t["aabbcc"] = "1"
	t["aabbcc"] = "12"
	print(t["aabbcc"])
	
	
	-- create private index
	local index = {}
	-- create metatable
	local mt = {
		__index = function (t,k)
			print("*access to element " .. tostring(k))
			return t[index][k] -- access the original table
		end,
		__newindex = function (t,k,v)
			print("*update of element " .. tostring(k) .. " to " .. tostring(v))
			t[index][k] = v -- update original table
		end
	}
	function track (t)
		local proxy = {}
		proxy[index] = t
		setmetatable(proxy, mt)
		return proxy
	end
	 
	b = track({})
	b["ssdd"] = "2"
	b["ssddff"] = "23"
	print(b["ssddff"])
	
	
	--只读表
	local function readOnly (t)
		local proxy = {}
		local mt = { -- create metatable
			__index = t,
			__newindex = function (t,k,v)
				error("attempt to update a read-only table", 2)
			end
			}
		setmetatable(proxy, mt)
		return proxy
	end
	
	local days = readOnly{"Sunday", "Monday", "Tuesday", "Wednesday",
	"Thursday", "Friday", "Saturday"}
	print(days[1]) --> Sunday
	--days[2] = "Noday" -- error
	
	
	
end

local function testString()
	varname = "[[" .. type(os.execute('dir')) .. "]]"
	print(varname)
end

local function testEnv()
	--for n in pairs(_G) do print(n) end
	
	
	if false
	then
		function declare (name, initval)
			rawset(_G, name, initval or false)
		end
		setmetatable(_G, {
			__newindex = function (_, n)
			error("attempt to write to undeclared variable "..n, 2)
			end,
			__index = function (_, n)
			error("attempt to read undeclared variable "..n, 2)
			end,
		})
	
		declare("a")
		a=1
		print(a)
	end
	
	
	if(_VERSION == "Lua 5.1")
	then
		a = 1 -- create a global variable
		
		--setfenv(1, {_G = _G}) -- change current environment
		--_G.print(a) --> nil
		--_G.print(_G.a) --> 1
		
		
		local newgt = {} -- create new environment
		setmetatable(newgt, {__index = _G})
		setfenv(1, newgt) -- set it
		print(a) --> 1
		
		print("-----------------------------")
		a = 10
		print(a) --> 10
		print(_G.a) --> 1
		_G.a = 20
		print(_G.a) --> 20
		
	end
end

local function testPackage()
	-- 方法1
	local function p1()
		local complex = {}
		function complex.new (r, i) return {r=r, i=i} end
		
		-- defines a constant `i'
		complex.i = complex.new(0, 1)
		
		function complex.add (c1, c2)
			return complex.new(c1.r + c2.r, c1.i + c2.i)
		end
		function complex.sub (c1, c2)
			return complex.new(c1.r - c2.r, c1.i - c2.i)
		end
		function complex.mul (c1, c2)
			return complex.new(c1.r*c2.r - c1.i*c2.i,c1.r*c2.i + c1.i*c2.r)
		end
		function complex.inv (c)
			local n = c.r^2 + c.i^2
			return complex.new(c.r/n, -c.i/n)
		end
		return complex
	end

	
	local function p2()
		local function checkComplex (c)
			if not ((type(c) == "table")
				and tonumber(c.r) and tonumber(c.i)) then
				error("bad complex number", 3)
			end
		end
		
		local function new (r, i) return {r=r, i=i} end
		
		
		local function add (c1, c2)
			checkComplex(c1);
			checkComplex(c2);
			return new(c1.r + c2.r, c1.i + c2.i)
		end

		local function sub (c1, c2)
			return new(c1.r - c2.r, c1.i - c2.i)
		end
		local function mul (c1, c2)
			return new(c1.r*c2.r - c1.i*c2.i,c1.r*c2.i + c1.i*c2.r)
		end
		local function div (c)
			local n = c.r^2 + c.i^2
			return new(c.r/n, -c.i/n)
		end
		
		local complex = {
			new = new,
			add = add,
			sub = sub,
			mul = mul,
			div = div,
		}
		complex.i = complex.new(0, 2)
		return complex
	end
	c = p2()
	print(c.r, c.i , c.i.r, c.i.i)
	c = c.add(c.i, c.new(10, 20))
	print(c.r, c.i)
	
	function openpackage (ns)
		for n,v in pairs(ns) do
			if _G[n] ~= nil then
				error("name clash: " .. n .. " is already defined")
			end
			_G[n] = v
		end
	end
	
	local ABC = {}
	ABC.show = function()
		print("ABC.show()") 
	end
	
	--openpackage(ABC)
	--openpackage(ABC)  -- 第二次定义抛出错误
	local function p3()
		local location = {
			foo = "/usr/local/lua/lib/pack1_1.lua",
			goo = "/usr/local/lua/lib/pack1_1.lua",
			foo1 = "/usr/local/lua/lib/pack1_2.lua",
			goo1 = "/usr/local/lua/lib/pack1_3.lua",
		}
		pack1 = {}
		
		setmetatable(pack1, {
			__index = function (t, funcname)
				local file = location[funcname]
				if not file then
					error("package pack1 does not define " .. funcname)
				end
				assert(loadfile(file))() -- load and run definition
				return t[funcname] -- return the function
			end
		})
		return pack1
	end
end


--[[
Lua查找一个表元素的3个步骤
	到这里，总结一下Lua查找一个表元素时的规则，其实就是如下3个步骤:
	1.在表中查找，如果找到，返回该元素，找不到则继续
	2.判断该表是否有元表（操作指南），如果没有元表，返回nil，有元表则继续
	3.判断元表（操作指南）中有没有关于索引失败的指南（即__index方法），如果没有（即__index方法为nil），则返回nil；如果__index方法是一个表，则重复1、2、3；如果__index方法是一个函数，则返回该函数的返回值
--]]
local function testOBJ()

		
	father = {
		house=1
	}
	--father.__index = father
	son = {
		car=1
	}
	setmetatable(son, father) --把son的metatable设置为father
	print(son.house)
	--[[
	在上述例子中，访问son.house时，son中没有house这个成员，但Lua接着发现son有元表father，
	注意：此时，Lua并不是直接在father中找名为house的成员，而是调用father的__index方法，如果__index方法为nil，则返回nil，如果是一个表（上例中father的__index方法等于自己，就是这种情况），那么就到__index方法所指的这个表中查找名为house的成员，于是，最终找到了house成员。
	注：__index方法除了可以是一个表，还可以是一个函数，如果是一个函数，__index方法被调用时将返回该函数的返回值。	
	--]]
		
	local Account = {balance = 0}
	function Account:new (o)
		o = o or {}
		setmetatable(o, self)
		print("Account:", Account)     --            table: 0000000019C8E180
		print("self", self) -- 当前self为Account table: 0000000019C8E180
		self.__index = self
		return o
	end
	function Account:deposit (v)
		self.balance = self.balance + v
	end
	function Account:withdraw (v)
		if v > self.balance then error"insufficient funds" end
			self.balance = self.balance - v
	end
	
	SpecialAccount = Account:new()
	print(SpecialAccount) --table: 0000000019C8DF00
	--s = SpecialAccount:new{limit=1000.00}
	--s:deposit(100.00)
	
	-- 多重继承
	Named = {}
	function Named:getname ()
		return self.name
	end
	
	function Named:setname (n)
		self.name = n
	end
	
	
	-- look up for `k' in list of tables 'plist'
	local function search (k, plist)
		for i=1, #plist do
			local v = plist[i][k] -- try 'i'-th superclass
			if v then return v end
		end
	end
	function createClass (...)
		local c = {} -- new class
		-- class will search for each method in the list of its
		-- parents (`arg' is the list of parents)
		setmetatable(c, {__index = function (t, k)
			local v = search(k, arg)
			t[k] = v -- save for next access
			return v
		end})
		--应用这个技巧，访问继承的方法和访问局部方法一样快.缺点是系统运行之后，很难改变方法的定义，因为这种改变不能影响继承链的下端。
		-- prepare `c' to be the metatable of its instances
		c.__index = c
		-- define a new constructor for this new class
		function c:new (o)
			o = o or {}
			setmetatable(o, c)
			return o
		end
		-- return new class
		return c
	end
	
	
	NamedAccount = createClass(Account, Named)
	account = NamedAccount:new{name = "Paul"}
	print(account:getname()) --> Paul
	
	
	
	function newAccount (initialBalance)
		local self = {balance = initialBalance}
		local withdraw = function (v)
			self.balance = self.balance - v
		end
		local deposit = function (v)
			self.balance = self.balance + v
		end
		local getBalance = function () 
			return self.balance 
		end
		
		return {
			withdraw = withdraw,
			deposit = deposit,
			getBalance = getBalance
		}
	end


	acc1 = newAccount(100.00)
	acc1.withdraw(40.00)
	--acc1:withdraw(40.00) -- error
	print(acc1.getBalance()) --> 60
		
end



local function testWeak()
	a = {}
	b = {}
	setmetatable(a, b)
	b.__mode = "v" -- now 'a' has weak keys
	key = {} -- creates first key
	a[key] = 1
	key = {} -- creates second key
	a[key] = 2
	collectgarbage() -- forces a garbage collection cycle
	for k, v in pairs(a) do print(v) end
end

local function testTable()
	print(#{10,2,4}) --> 3
	print(#{10,2,nil}) --> 2
	print(#{10,2,nil; n=3}) --> 2
	print(#{n=1000})  --> 0
	print(#{n=1000,n2=2000})  --> 0
end

local function testString()
	print(string.lower("a?b??c"))
	print(string.upper("A?B??C"))
	s = "[in brackets]"
	print(string.sub(s, 2, -2))
	
	print(string.char(97)) --> a
	i = 99; print(string.char(i, i+1, i+2)) --> cde
	print(string.byte("abc")) --> 97
	print(string.byte("abc", 2)) --> 98
	print(string.byte("abc", -1)) --> 99
	
	
	print(string.format("pi = %.4f", math.pi))
	--> pi = 3.1416
	d = 5; m = 11; y = 1990
	print(string.format("%02d/%02d/%04d", d, m, y))
	--> 05/11/1990
	tag, title = "h1", "a title"
	print(string.format("<%s>%s</%s>", tag, title, tag))
	--> <h1>a title</h1>
	
	
	s = "hello world"
	i, j = string.find(s, "hello")
	print(i, j) --> 1 5
	print(string.sub(s, i, j)) --> hello
	print(string.find(s, "world")) --> 7 11
	i, j = string.find(s, "l")
	print(i, j) --> 3 3
	print(string.find(s, "lll")) --> nil
	
	local t = {} -- table to store the indices
	local i = 0
	while true do
		i = string.find(s, "\n", i+1) -- find 'next' newline
		if i == nil then break end
		table.insert(t, i)
	end
	
	--string.gsub函数有三个参数：目标串，模式串，替换串
	s = string.gsub("Lua is cute", "cute", "great")
	print(s) --> Lua is great
	s = string.gsub("all lii", "l", "x")
	print(s) --> axx xii
	s = string.gsub("Lua is great", "perl", "tcl")
	print(s) --> Lua is great
	
	
	s = "Deadline is 30/05/1999, firm"
	date = "%d%d/%d%d/%d%d%d%d"
	print(string.sub(s, string.find(s, date))) --> 30/05/1999
	
	-- %类似于转义标识
	--'.*' 进行的是最长匹配
	--'.-' 进行的是最短匹配
	--'%d' 表示 '[0-9]'
	--'%x' 表示'[0-9a-fA-F]'
	--'-' 与 '*' 一样，都匹配一个字符的0次或多次出现
	test = "int x; /* x */ int y; /* y */"
	print(string.gsub(test, "/%*.*%*/", "<COMMENT>"))
	--> int x; <COMMENT>
	test = "int x; /* x */ int y; /* y */"
	print(string.gsub(test, "/%*.-%*/", "<COMMENT>"))
	--> int x; <COMMENT> int y; <COMMENT>
	
	-- %b' 用来匹配对称的字符。常写为 '%bxy' ，x和y是任意两个不同的字符；x作为匹配的开始，y作为匹配的结束。比如，'%b()' 匹配以 '(' 开始，以 ')' 结束的字符串：
	print(string.gsub("a (enclosed (in) parentheses) line", "%b()", ""))
	
	pair = "name = Anna"
	_, _, key, value = string.find(pair, "(%a+)%s*=%s*(%a+)")
	print(key, value) --> name Anna
	
	
	date = "17/7/1990"
	_, _, d, m, y = string.find(date, "(%d+)/(%d+)/(%d+)")
	print(d, m, y) --> 17 7 1990
	
	s = [[then he said: "it's all right"!]]
	a, b, c, quotedPart = string.find(s, "([\"'])(.-)%1")
	print(a,b,c,quotedPart) --> it's all right
	print(c) --> "
	
	print(string.gsub("hello Lua!", "(%a)", "%1-%1"))
	--> h-he-el-ll-lo-o L-Lu-ua-a!
	
	print(string.gsub("hello Lua", "(.)(.)", "%2%1"))
	--> ehll ouLa
	
	function trim (s)
		return (string.gsub(s, "^%s*(.-)%s*$", "%1"))
	end
	print(trim("   [123 xx  ff]  "))
	
	
	function expand (s)
		s = string.gsub(s, "$(%w+)", function (n) return _G[n] end)
		return s
	end
	name = "Lua"; status = "great"
	print(expand("$name is $status, isn't it?"))
		
	
	function expand (s)
		return (string.gsub(s, "$(%w+)", function (n)
			return tostring(_G[n])
		end))
	end
	print(expand("print = $print; a = $a"))
		
	
	s = "sin(3) = $[math.sin(3)]; 2^5 = $[2^5]"
	print((string.gsub(s, "$(%b[])", function (x)
		x = "return " .. string.sub(x, 2, -2)
		local f = loadstring(x)
		return f()
	end)))
	
	
	words = {}
	for w in string.gfind(s, "%a") do
		table.insert(words, w)
	end
	
	--TODO more...
	
end

local function testIO()
	io.write(string.format("sin (3) = %.4f\n", math.sin(3)))
	--t = io.read("*line") -- read the whole file
	--t = string.gsub(t, "abc","123") -- do the job
	--io.write(t) -- write the file
	function fsize (file)
		local current = file:seek() -- get current position
		local size = file:seek("end") -- get file size
		file:seek("set", current) -- restore position
		return size
	end
end

local function testFile()
	local f = assert(io.open("./src/MT01.lua", "rb"))
	local block = 10
	while true do
		local bytes = f:read(block)
		if not bytes then break end
		-- .(点): 与任何字符配对
		-- string.gmatch (s, pattern)
		-- 返回一个迭代器函数。每次调用这个函数都会继续以 pattern （参见　§6.4.1）对 s 做匹配，并返回所有捕获到的值。如果 pattern 中没有指定捕获，则每次捕获整个 pattern
		for b in string.gfind(bytes, ".") do
			io.write(string.format("%02X ", string.byte(b)))
		end
		-- string.rep (s, n [, sep])
		-- 返回 n 个字符串 s 以字符串 sep为分割符连在一起的字符串。默认的 sep 值为空字符串（即没有分割符）。如果 n 不是正数则返回空串。
		io.write(string.rep(" ", (block - string.len(bytes))*3 + 1))
		
		--string.gsub (s, pattern, repl [, n])
		--将字符串 s 中，所有的（或是在 n 给出时的前 n 个）pattern （参见 §6.4.1）都替换成 repl ，并返回其副本。
		--%c: 与任何控制符配对(例如\n)
		io.write(string.gsub(bytes, "%c", "."), "\n")
	end

end

local function testOS()
	print(os.time{year=1970, month=1, day=1, hour=0})
	--> 10800
	print(os.time{year=1970, month=1, day=1, hour=0, sec=1})
	--> 10801
	print(os.time{year=1970, month=1, day=1})
	
	temp = os.date("*t", 906000490)
	for i, v in pairs(temp)
	do
		print(i,v)
	end
	
	print(os.date("today is %A, in %B"))
	--> today is Tuesday, in May
	print(os.date("%x", 906000490))
	--> 09/17/98
	
	local x = os.clock()
	local s = 0
	for i=1,10000000 do s = s + i end
	print(string.format("elapsed time: %.2f\n", os.clock() - x))
	
	print(os.getenv("HOME"))
	
	local function createDir (dirname)
		os.execute("mkdir " .. dirname)
	end
	
	print(os.setlocale("ISO-8859-1", "collate"))
	
	-- 设置区域为葡萄牙语系巴西
	print(os.setlocale('pt_BR')) --> pt_BR
	print(3,4) --> 3 4
	print(3.4) --> stdin:1: malformed number near `3.4'
	
end

local function testDebug()
	--[[
		Debug库中的一个重要的思想是栈级别(stack level)。
		一个栈级别就是一个指向在当前时刻正在活动的特殊函数的数字，
		也就是说，这个函数正在被调用但还没有返回。调用debug库的函数级别为1，
		调用他(他指调用debug库的函数)的函数级别为2，以此类推。
	--]]
	local function traceback ()
		local level = 1
		while true do
			--S  返回 source short_src linedefined lastlinedefined what
			--l  返回currentline
			local info = debug.getinfo(level, "Sl")
			if not info then break end
			
			--if info.what == "C" then -- is a C function?
			--	print(level, "C function")
			--else -- a Lua function
			--	print(string.format("[%s]:%d",info.short_src, info.currentline))
			--end
			for i,v in pairs(info)
			do
				print(i,v)
			end
			print('\n')
			level = level + 1
		end
	end
	traceback()
	
	
	function foo (a,b)
		local x
		do local c = a - b end
		local a = 1
		while true do
			local name, value = debug.getlocal(1, a)
			if not name then break end
			print(name, value)
			a = a + 1
		end
	end
	foo(10, 20)
	
	local abc='123456'
	function getvarvalue (name)
		local value, found
		-- try local variables
		local i = 1
		while true do
			local n, v = debug.getlocal(2, i)
			if not n then break end
				if n == name then
					value = v
					found = true
				end
				i = i + 1
			end
			if found then return value end
			-- try upvalues
			local func = debug.getinfo(2).func
			i = 1
			while true do
				local n, v = debug.getupvalue(func, i)
				if not n then break end
				if n == name then return v end
				i = i + 1
			end
		-- not found; get global
		print(func,name)
		return getfenv(func)[name]
	end
	print(getvarvalue("abc"))
	
	
	--[[
	debug.sethook ([thread,] hook, mask [, count])
		将一个函数作为钩子函数设入。字符串 mask 以及数字 count 决定了钩子将在何时调用。掩码是由下列字符组合成的字符串，每个字符有其含义：
		    'c': 每当 Lua 调用一个函数时，调用钩子；
		    'r': 每当 Lua 从一个函数内返回时，调用钩子；
		    'l': 每当 Lua 进入新的一行时，调用钩子。 此外，传入一个不为零的 count ，钩子将在每运行 count 条指令时调用。
		如果不传入参数，debug.sethook 关闭钩子。
	--]]
	function trace (event, line)
		--[[
		debug.getinfo ([thread,] f [, what])
			返回关于一个函数信息的表。你可以直接提供该函数，也可以用一个数字 f 表示该函数。数字 f 表示运行在指定线程的调用栈对应层次上的函数：0 层表示当前函数（getinfo 自身）；1 层表示调用 getinfo 的函数（除非是尾调用，这种情况不计入栈）；等等。如果 f 是一个比活动函数数量还大的数字，getinfo 返回 nil。
			只有字符串 what 中有描述要填充哪些项，返回的表可以包含 lua_getinfo 能返回的所有项。what 默认是返回提供的除合法行号表外的所有信息。对于选项 'f' ，会在可能的情况下，增加func 域保存函数自身。对于选项 'L' ，会在可能的情况下，增加activelines 域保存合法行号表。
			例如,表达式 debug.getinfo(1,"n")返回带有当前函数名字信息的表（如果找的到名字的话），表达式 debug.getinfo(print) 返回关于 print 函数的包含有所有能提供信息的表。
		--]]
		local s = debug.getinfo(2).short_src
		print(s .. ":" .. line)
	end
	--debug.sethook(trace, "l")
	--print("hooka")
	
	
	-- 测试函数调用次数
	local Counters = {}
	local Names = {}
	
	local function hook ()
		local f = debug.getinfo(2, "f").func
		if Counters[f] == nil then -- first time `f' is called?
			Counters[f] = 1
			--[[
				  if (strchr(options, 'S')) {
				    settabss(L, "source", ar.source);
				    settabss(L, "short_src", ar.short_src);
				    settabsi(L, "linedefined", ar.linedefined);
				    settabsi(L, "lastlinedefined", ar.lastlinedefined);
				    settabss(L, "what", ar.what);
				  }
				  if (strchr(options, 'l'))
				    settabsi(L, "currentline", ar.currentline);
				  if (strchr(options, 'u')) {
				    settabsi(L, "nups", ar.nups);
				    settabsi(L, "nparams", ar.nparams);
				    settabsb(L, "isvararg", ar.isvararg);
				  }
				  if (strchr(options, 'n')) {
				    settabss(L, "name", ar.name);
				    settabss(L, "namewhat", ar.namewhat);
				  }
				  if (strchr(options, 't'))
				    settabsb(L, "istailcall", ar.istailcall);
				  if (strchr(options, 'L'))
				    treatstackoption(L, L1, "activelines");
				  if (strchr(options, 'f'))
				    treatstackoption(L, L1, "func");
			--]]
			Names[f] = debug.getinfo(2, "Sn")
		else -- only increment the counter
			Counters[f] = Counters[f] + 1
		end
	end
	debug.sethook(hook, "c")
	testFunction()
	debug.sethook() ---- turn off the hook
	
	
	function getname (func)
		local n = Names[func]
		if n.what == "C" then
			return n.name
		end
		local loc = string.format("[%s]:%s", n.short_src, n.linedefined)
		if n.namewhat ~= "" then
			return string.format("%s (%s)", loc, n.name)
		else
			return string.format("%s", loc)
		end
	end
	for func, count in pairs(Counters) do
		print(getname(func), count)
	end
	
end


--C和Lua中间有两种交互方式。
--第一种，C作为应用程序语言，Lua作为一个库使用；
--第二种，反过来，Lua作为程序语言，C作为库使用。这两种方式，C语言都使用相同的API与Lua通信
--因此C和Lua交互这部分称为C API。
local function testCAPI()
--[[
压入元素
	void lua_pushnil (lua_State *L);
	void lua_pushboolean (lua_State *L, int bool);
	void lua_pushnumber (lua_State *L, double n);
	void lua_pushlstring (lua_State *L, const char *s,
	size_t length);
	void lua_pushstring (lua_State *L, const char *s);
	
检测栈上是否有足够你需要的空间
	int lua_checkstack (lua_State *L, int sz);
	
	
查询元素
	API用索引来访问栈中的元素。在栈中的第一个元素（也就是第一个被压入栈的）有索引1，下一个有索引2，以此类推。我们也可以用栈顶作为参照来存取元素，利用负索引。在这种情况下，-1指出栈顶元素（也就是最后被压入的），-2指出它的前一个元素，以此类推。例如，调用lua_tostring(L, -1)以字符串的形式返回栈顶的值。我们下面将看到，在某些场合使用正索引访问栈比较方便，另外一些情况下，使用负索引访问栈更方便。
	lua_isnumber和lua_isstring函数不检查这个值是否是指定的类型，而是看它是否能被转换成指定的那种类型。例如，任何数字类型都满足lua_isstring。

	int lua_toboolean (lua_State *L, int index);
	double lua_tonumber (lua_State *L, int index);
	const char * lua_tostring (lua_State *L, int index);
	size_t lua_strlen (lua_State *L, int index);
	

其他堆栈操作
	int lua_gettop (lua_State *L);
		函数lua_gettop返回堆栈中的元素个数，它也是栈顶元素的索引。注意一个负数索引-x对应于正数索引gettop-x+1
	void lua_settop (lua_State *L, int index);
	void lua_pushvalue (lua_State *L, int index);
	void lua_remove (lua_State *L, int index);
	void lua_insert (lua_State *L, int index);
	void lua_replace (lua_State *L, int index);

从堆栈中弹出n个元素：
	#define lua_pop(L,n) lua_settop(L, -(n)-1)
	
	
函数lua_pushvalue压入堆栈上指定索引的一个抟贝到栈顶；
	lua_remove移除指定索引位置的元素，并将其上面所有的元素下移来填补这个位置的空白；
	lua_insert移动栈顶元素到指定索引的位置，并将这个索引位置上面的元素全部上移至栈顶被移动留下的空隔；
	lua_replace从栈顶弹出元素值并将其设置到指定索引位置，没有任何移动操作。注意到下面的操作对堆栈没有任何影响
		lua_settop(L, -1); /* set top to its current value */
		lua_insert(L, -1); /* move top element to the top */
	
	
API提供了下面两个数组操作函数：
	int lua_rawgeti (lua_State *L, int index, lua_Integer n);
		把 t[n] 的值压栈，这里的 t 是指给定索引处的表。这是一次直接访问；就是说，它不会触发元方法。返回入栈值的类型
	void lua_rawseti (lua_State *L, int index, lua_Integer i);
		等价于 t[i] = v ，这里的 t 是指给定索引处的表，而 v 是栈顶的值。
		这个函数会将值弹出栈。赋值是直接的；即不会触发元方法。
--]]


--[[
User-Defined Types in C
	Lua API提供了下面的函数用来创建一个userdatum：
		void *lua_newuserdata (lua_State *L, size_t size);


--]]
end


--testLocal()
--testFunction()
--testFunction2()
--testDataType()
--testSelect()
--testPairsAndIpairs()
--testCycle()
--testControl()
--testLogic()
--testPrint()
--testIter()
--testLoadLib()
--testIO()
--testCall()
--testCoroutine()
--testModule()
--testString()
--testMetalable()
--testEnv()
--testPackage()
--testOBJ()
--testWeak()
--testTable()
--testString()
--testIO()
--testFile()
--testOS()
--testDebug()
testCAPI()







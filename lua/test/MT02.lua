
--调用module函数时，多传入一个 package.seeall 的参数
--相当于 setmetatable(M, {__index = _G})

--[[
抵制使用 module()函数来定义 Lua 模块
	旧式的模块定义方式是通过 *module("filename"[,package.seeall])*来显示声明一个包，
	现在官方不推荐再使用这种方式。这种方式将会返回一个由 filename 模块函数组成的 table，
	并且还会定义一个包含该 table 的全局变量。
	如果只给 module 函数一个参数（也就是文件名）的话，前面定义的全局变量就都不可用了，
	包括 print 函数等，如果要让之前的全局变量可见，
	必须在定义 module 的时候加上参数 package.seeall 。
	调用完 module 函数之后， print 这些系统函数不可使用的原因，
	是当前的整个环境被压入栈，不再可达。
	
	
module("filename", package.seeall) 这种写法仍然是不提倡的，官方给出了两点原因：
    package.seeall 这种方式破坏了模块的高内聚，原本引入"filename"模块只想调用它的 foobar() 函数，但是它却可以读写全局属性，例如 "filename.os" 。
    module 函数压栈操作引发的副作用，污染了全局环境变量。例如 module("filename") 会创建一个 filename 的 table ，并将这个 table 注入全局环境变量中，这样使得没有引用它的文件也能调用 filename 模块的方法。

--]]

module("MT02",package.seeall)
--module("MT02") -- 使用这一行会导致print无法调用

function printB()   print('B')  end

function printC()   print('C')  end  


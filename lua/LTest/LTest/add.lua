local mylib= require "mylib" 


print("========================call libName.cFunction test start========================")
mylib.stdfile2()
print("========================call libName.cFunction test end  ========================\n")

function add ( x, y )
	return x + y
end

function LuaFunc()
    return 1,2,3,4;
end

helloName = {name = "aaa"}

function f (x, y)
	return (x^2 * math.sin(y))/(1 - x)
end


print("========================arrayTest start========================")
a = array.new(1000)
print(a) --> userdata: 0x8064d48
print(array.size(a)) --> 1000
for i=1,1000 do
	array.set(a, i, 1/i)
end
print("========================arrayTest end  ========================\n")



print("========================call c function test start========================")
print("mysin ok!",mysin(100))
fun1 "lua"
print("========================call c function test end  ========================\n")
print("load ok!",add(1,2))
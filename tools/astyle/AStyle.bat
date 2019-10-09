::   转换结果中提示Cannot convert to multi-byte string...可无视  
::   依赖程序是短小精悍的 Astyle , 当前版本 Artistic Style 2.03  
::   载地址 http://sourceforge.net/projects/astyle/files/astyle/  
::参数用法  
::想缩进4个空格的话,将 -s2 改成 -s4  
::修改参数可直接不生成备份文件(也就不用删了)  
::--------------------------------------------------------------------  
@echo off  
title 代码批量整理助手V1  
echo         文件支持类型c,h,cpp ,   支持4种工作方式  
echo 1双击执行转换当前目录及子目录中文件  
echo 2拖拽任意路径的单个文件  
echo 3拖拽任意路径的目录  
echo 4右键文件或目录发送到sendto  
echo.  

echo. 

cd /d "%~dp1"
set filename=%~nx1
set pats=%~dp1
set Asuffix="--style=ansi -f -X -U -Q -s4 -S -N -L -m0 -M40 --convert-tabs --suffix=.pre"
::首次使用,请修改下面的AsPath的路径!!!!!!!!!!!!!!!!!!!!!!!!!  
set AsPath="astyle.exe"  
::%1就是表示批处理的第一个参数， %~1表示删除参数外面的引号
if /i "%~1"=="" goto :doubleClick   
IF EXIST "%~1\" GOTO :dir
if "%filename:~-4%"==".cpp" goto :single  
if "%filename:~-2%"==".c"   goto :single
if "%filename:~-3%"==".cc"   goto :single
if "%filename:~-2%"==".h"   goto :single  
cls  
color 0a  
ECHO %filename%  
ECHO 是无效的后缀,当前支持的后缀类型是c,cpp,h ,要支持其他类型请修改参数  
pause  
exit  
:single  
echo --------------------singleFile mode----------------------  
ECHO 转换的单个文件:%filename%  
%AsPath%  "%Asuffix%" "%filename%"  
::上句中的参数按需修改  

REM 删除所有的备份文件  
REM del *.pre /s /q  
pause  
exit  
:dir  
echo ---------------------dir mode-----------------------------  
for /r "%~nx1" %%f in (*.cpp;*.c;*.cc;*.h) do %AsPath% "%Asuffix%" "%%f"  
REM 删除所有的备份文件  
REM for /r "%~nx1" %%a in (*.pre) do del "%%a"  
pause  
exit  
:doubleClick  
echo -------------------doubleClick mode--------------------------  
for /r . %%f in (*.cpp;*.c;*.cc;*.h) do %AsPath% "%Asuffix%" "%%f"  
REM 删除所有的备份文件  
REM del *.pre /s /q  
pause  
exit  

Name:		test
Version:	1.0
Release:	%{?dist}
Summary:	this is test!!!
License:  GPL
URL:      http://www.test.com/



AutoReqProv: no

%description
this is a stop uninstall demo!


%install


%files


# rpm安装前执行的脚本
%pre


#定义安装之后执行的脚本
%post




#定义卸载软件之前执行的脚本
%preun
myFile="/root/tmp.log"

if [ ! -f "$myFile" ]; then 
	exit 1
fi 
[[ -z $(<$myFile) ]]&& exit 2 ||echo No empty
cat "$myFile"  | while read line
do
	echo $line
	if [ "$line" = "password" ]; then
		echo "password ok! start uninstall"
	else
		exit 3
		
	fi
done


#定义卸载软件之后执行的脚本
%postun


%changelog



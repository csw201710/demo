#!/bin/bash

gHasDesktopIcon=true
gDesktopName="Desktop"
gDesktopIconFileName="qt4-linguist.desktop"


function  prerm_uninstall_wpsdaemon(){
	killall -9 xxxxxxxxxxxxxxxxxxxxxxxx &>/dev/null || true
}




function prerm_uninstall_desktop()
{
	prerm_uninstall_wpsdaemon
	gDesktopName="桌面"
	if [ -d "/root/桌面" ]; then
		gDesktopName="桌面"
	elif [ -d "/root/Desktop" ]; then
		gDesktopName="Desktop"
	fi

	#root desktop
	if [ -d "/root/${gDesktopName}" ]; then
		rm -f /root/${gDesktopName}/${gDesktopIconFileName} >/dev/null 2>&1 || true
	fi


	for FILENAME in /home/*; do
		if [ -d "${FILENAME}/桌面" ]; then
			gDesktopName="桌面"
		elif [ -d "${FILENAME}/Desktop" ]; then
			gDesktopName="Desktop"
		fi

		#desktop
		local curDesktopFilePath="${FILENAME}/${gDesktopName}/${gDesktopIconFileName}"
		if [ -f  ${curDesktopFilePath} ]; then
			rm -f ${curDesktopFilePath}
		fi


	done

}





# 第一个参数用户所有目录   
# 第二个参数是desktop文件名
# 第三个参数用户名
function copy_install_to_desktop()
{
	if [ $# -ne 3 ] ; then
		return;
	fi
	local curFileName=$1
	local wpsDesktopName=$2
	local curUserName=$3
	if [ -e "/usr/share/applications/${wpsDesktopName}" ]; then
		cp -f /usr/share/applications/${wpsDesktopName} "${curFileName}/${gDesktopName}/${wpsDesktopName}"
		if test $? -eq 0; then
			chmod +x "${curFileName}/${gDesktopName}/${wpsDesktopName}"
			chown ${curUserName} "${curFileName}/${gDesktopName}/${wpsDesktopName}"
		fi
	fi
	
}

# 第一个参数用户所有目录   
# 第二个参数用户名
function deploy_desktop_icon()
{
	if [ $# -ne 2 ] ; then
		return;
	fi
	local curFileName=$1
	local curUserName=$2
	copy_install_to_desktop ${curFileName} ${gDesktopIconFileName} ${curUserName}
}


# 第一个参数用户所有目录   
# 第二个参数用户名
function to_deploy_desktop_icon()
{
	if [ $# -ne 2 ] ; then
		return;
	fi
	local curFileName=${1}
	local curUserName=${2}
	
	deploy_desktop_icon ${curFileName}  ${curUserName}
}


function config_desktop_icon()
{
	set -e
	gDesktopName="桌面"
	if [ -d "/root/桌面" ]; then
		gDesktopName="桌面"
	elif [ -d "/root/Desktop" ]; then
		gDesktopName="Desktop"
	fi
		
# 发布图标到root桌面
	if [ -d "/root/${gDesktopName}" ] && $gHasDesktopIcon; then
		to_deploy_desktop_icon "/root" "root"
	fi

# 发布图标到所有用户桌面
	for FILENAME in /home/*; do
        # 截取用户名
		if id -u `echo ${FILENAME} | awk '{print substr($FILENAME, 7, 32)}'` >/dev/null 2>&1; then
			:
		else
			continue
		fi
		
		if [ -d "${FILENAME}/桌面" ]; then
			gDesktopName="桌面"
		elif [ -d "${FILENAME}/Desktop" ]; then
			gDesktopName="Desktop"
		fi

		if [ -d "${FILENAME}/${gDesktopName}" ]; then
			if $gHasDesktopIcon; then
				local curUserName=`echo ${FILENAME} | awk '{print substr($FILENAME, 7, 32)}'`
				to_deploy_desktop_icon ${FILENAME} ${curUserName}
			fi

		fi

	done

}
# 安装桌面图标
config_desktop_icon
# 卸载桌面图标
prerm_uninstall_desktop






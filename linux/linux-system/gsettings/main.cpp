/*
	desc  :  gsetting demo
	author:  csw
	date  :  2019-3-4
*/

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>          /* For uid_t, gid_t */



#define SESSION_SETTINGS_SCHEMA "org.mate.session"

#define GSETTINGS_SCHEMA "org.mate.screensaver"

#define KEY_IDLE_DELAY "idle-delay"

#define KEY_LOCK "lock-enabled"   //屏保启动时是否锁屏

#define KEY_IDLE_ACTIVATION_ENABLED "idle-activation-enabled"  //空闲时激活屏保


#define LOCKDOWN_SETTINGS_SCHEMA "org.mate.lockdown"
#define KEY_LOCK_DISABLE "disable-lock-screen"


//电源管理
#define GPM_SETTINGS_SCHEMA				"org.mate.power-manager"
#define GPM_SETTINGS_SLEEP_COMPUTER_AC			"sleep-computer-ac" //未操作，休眠时间
#define GPM_SETTINGS_SLEEP_DISPLAY_AC			"sleep-display-ac"   //未操作，电脑黑屏

#include <gio/gio.h>

int main(){
	if(0)
	{
		GSettings      *session_settings = NULL;
		session_settings = g_settings_new (SESSION_SETTINGS_SCHEMA);

		//获取
		gint32 delay = g_settings_get_int (session_settings, KEY_IDLE_DELAY);
		fprintf(stdout,"delay : %d\n",delay);
		//修改
		//g_settings_set_int (session_settings, KEY_IDLE_DELAY, 5);

		g_settings_sync ();
		
		//获取
		delay = g_settings_get_int (session_settings, KEY_IDLE_DELAY);
		fprintf(stdout,"delay : %d\n",delay);	
		
		
		g_object_unref (session_settings);
	}
	if(0)
	{
			GSettings      *screensaver_settings = NULL;
			gboolean lock,enabled;
			screensaver_settings = g_settings_new (GSETTINGS_SCHEMA);
			
			//获取
			lock = g_settings_get_boolean (screensaver_settings, KEY_LOCK);
			fprintf(stdout, KEY_LOCK " is : %d\n",lock);
			
			//获取
			enabled = g_settings_get_boolean (screensaver_settings, KEY_IDLE_ACTIVATION_ENABLED);
			fprintf(stdout,KEY_IDLE_ACTIVATION_ENABLED " is : %d\n",enabled);
			
			
			//修改
			//g_settings_set_boolean (screensaver_settings, KEY_IDLE_ACTIVATION_ENABLED, enabled);
			
			
			g_object_unref (screensaver_settings);
			
			
	}
	
	if(0)
	{
		GSettings      *lockdown_settings = NULL;
		lockdown_settings = g_settings_new (LOCKDOWN_SETTINGS_SCHEMA);
		//修改
		g_settings_set_boolean (lockdown_settings, KEY_LOCK_DISABLE, 0);
		g_settings_sync ();
		g_object_unref (lockdown_settings);		
	}
	
	{
		GSettings      *pm_settings = NULL;
		pm_settings = g_settings_new (GPM_SETTINGS_SCHEMA);
		//修改
		g_settings_set_int (pm_settings, GPM_SETTINGS_SLEEP_COMPUTER_AC, 0);
		g_settings_set_int (pm_settings, GPM_SETTINGS_SLEEP_DISPLAY_AC, 0);
		g_settings_sync ();
		g_object_unref (pm_settings);		
	}
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <dbus/dbus.h>
#include <stdlib.h>
#include <unistd.h>
#include <shadow.h>
#include <pthread.h>

#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <stdio.h>
#include <pwd.h>
#include <unistd.h>

#include <gio/gio.h>

#include <errno.h>  
#include <string.h>  

#include <time.h>  

//无参数，有返回值调用
int  IsSystemLocked(){
	DBusError  err;
	DBusConnection * connection;
	DBusPendingCall *pending;
	DBusMessageIter arg;
  int __value_int;
  int  __value_bool = 0;
	

	dbus_error_init(&err);


	connection = dbus_bus_get(DBUS_BUS_SESSION, &err);
	if(connection == NULL){
	    printf ("Failed to open connection to bus : %s",err.message);
	    dbus_error_free(&err);
	    return 0;
	}

	DBusMessage * msg = dbus_message_new_method_call("org.mate.ScreenSaver","/"
																,"org.mate.ScreenSaver","GetActive");
	if(msg == NULL)	{
		printf("dbus_message_new_method_call failed!");
		return 0;
	}
 //发送消息并获得reply的handle 。Queues a message to send, as with dbus_connection_send() , but also returns a DBusPendingCall used to receive a reply to the message.
  if (!dbus_connection_send_with_reply(connection, msg, &pending, -1))
  {
      printf("Out of Memory! \n");
      return 0;
  }
  if (pending == NULL)
  {
      printf("Pending Call NULL   connection is disconnected ");
      return 0;
  }
  dbus_connection_flush(connection);
  dbus_message_unref(msg);
  dbus_pending_call_block(pending);
	msg = dbus_pending_call_steal_reply(pending);
	if (msg == NULL)
	{
	    printf("Reply Null");
	    return 0;
	}
  // free the pending message handle
  dbus_pending_call_unref(pending);
	// read the Arguments
  if (!dbus_message_iter_init(msg, &arg))
  {
      printf("Message has no Argument!");
      dbus_message_unref(msg);
      return 0;
  }
	

	
	do
	{
	    int ret = dbus_message_iter_get_arg_type(&arg);
	    if (DBUS_TYPE_BOOLEAN == ret)
	    {
	        dbus_message_iter_get_basic(&arg, &__value_bool);
	    }
	    else if (DBUS_TYPE_INT32 == ret)
	    {
	        dbus_message_iter_get_basic(&arg, &__value_int);
	        printf("get Method return INT32: %d\n",
	                __value_int);
	    }
	    else
	    {
	        printf("Argument Type ERROR\n");
	    }

	} while (dbus_message_iter_next(&arg));
	
	dbus_message_unref(msg);
	return __value_bool ? 1: 0;

}


//无参数调用
int  LockSystem(){
	DBusError  err;
	DBusConnection * connection;

	dbus_error_init(&err);

	connection = dbus_bus_get(DBUS_BUS_SESSION, &err);
	if(connection == NULL){
	    printf ("Failed to open connection to bus ");
	    dbus_error_free(&err);
	    return -1;
	}


	DBusMessage * msg = dbus_message_new_method_call("org.mate.ScreenSaver","/"
																,"org.mate.ScreenSaver","Lock");
	if(msg == NULL)	{
		printf("dbus_message_new_method_call failed!");

		return -1;
	}
	if(!dbus_connection_send(connection,msg,0)){
		printf("dbus_connection_send failed!");

		return -1;
	}
	dbus_connection_flush(connection);
	dbus_message_unref(msg);
	return 0;
}

//有参数调用，无返回值
int  UnLockSystem(){
	DBusError  err;
	DBusConnection * connection;
	DBusMessageIter arg;
	int  arg1 = 0;	
	int __type;
	dbus_error_init(&err);

	connection = dbus_bus_get(DBUS_BUS_SESSION, &err);
	if(connection == NULL){
	    printf ("Failed to open connection to bus: %s ",err.message);
	    dbus_error_free(&err);
	    
	    return -1;
	}


	DBusMessage * msg = dbus_message_new_method_call("org.mate.ScreenSaver","/"
																,"org.mate.ScreenSaver","SetActive");
	if(msg == NULL)	{
		printf("dbus_message_new_method_call failed!");
		
		return -1;
	}
	
	dbus_message_iter_init_append(msg, &arg);
	if (!dbus_message_iter_append_basic(&arg, 	DBUS_TYPE_BOOLEAN , (void*)&arg1))
	{
	    printf("Out of Memory!");
	    
	    return -1;
	}
	
	if(!dbus_connection_send(connection,msg,0)){
		printf("dbus_connection_send failed!");
		
		return -1;
	}
	dbus_connection_flush(connection);
	dbus_message_unref(msg);
	
	
	
	return 0;
}

int  WakeUpSystem(){
	DBusError  err;
	DBusConnection * connection;
	dbus_error_init(&err);

	connection = dbus_bus_get(DBUS_BUS_SESSION, &err);
	if(connection == NULL){
	    printf ("Failed to open connection to bus : %s",err.message);
	    
	    return -1;
	}


	DBusMessage * msg = dbus_message_new_method_call("org.mate.ScreenSaver","/"
																,"org.mate.ScreenSaver","SimulateUserActivity");
	if(msg == NULL)	{
		printf("dbus_message_new_method_call failed!");
	    return -1;
	}
	if(!dbus_connection_send(connection,msg,0)){
		printf("dbus_connection_send failed!");
	    return -1;
	}
	dbus_connection_flush(connection);
	dbus_message_unref(msg);

	
	return 0;
}



static void get_salt(char *salt,char *passwd)
{
    int i,j;

    //取出salt,i记录密码字符下标,j记录$出现次数
    for(i=0,j=0;passwd[i] && j != 3;++i)
    {
        if(passwd[i] == '$')
            ++j;
    }


    strncpy(salt,passwd,i-1);
}

static const char* g_password;


static int myconv(int num_msg, const struct pam_message **msg,
		struct pam_response **resp, void *appdata_ptr){
			
		struct pam_response *reply;	
    reply = (struct pam_response *) calloc(num_msg,
					   sizeof(struct pam_response));
	
	
	if(g_password != NULL && strlen(g_password) > 0){
		char *p = (char*)malloc(strlen(g_password));
		strcpy(p, g_password);
	
		reply[0].resp_retcode = 0;
		reply[0].resp = p;
		
	}else{
		reply[0].resp = "";
		reply[0].resp_retcode = 0;
	}
	*resp = reply;	
	return 0;
}
static struct pam_conv conv = {
    myconv,
    NULL  //appdata_ptr
};



int  LoginSystem(const char* szDomain, const char*  szUser, const char*  szPassword)
{
	#if 0
    struct spwd *sp;
    char salt[512]={0};

    if((sp=getspnam(szUser)) == NULL){
    	return 0;
    }

    get_salt(salt,sp->sp_pwdp);


    //进行密码验证
    if(strcmp(sp->sp_pwdp,crypt(szPassword,salt)) != 0){
        printf("验证失败!\n");
        return 0;
    }
    printf("验证通过!\n");
    
  #else
  	
  	
  
    pam_handle_t *pamh=NULL;
    int retval;
		const char *service = "system-auth";
		g_password = szPassword;
    retval = pam_start(service, szUser, &conv, &pamh);

    if (retval == PAM_SUCCESS){
    	retval = pam_authenticate(pamh, 0);    /* is user really user? */
	    if (retval == PAM_SUCCESS){
	    	retval = pam_acct_mgmt(pamh, 0);       /* permitted access? */
	    	
		    if (retval == PAM_SUCCESS) {
					printf( "pam_authenticate Authenticated\n");
					pam_end(pamh,retval) ;
		
					return 1;
		    } else {
		    	printf("pam_acct_mgmt failed!");
					printf( "pam_authenticate Not Authenticated\n");
					pam_end(pamh,retval);
		
					return 0;
		    }
	    }else{
	    	printf("pam_authenticate failed! passwd:[%s]",g_password);
	  
	    	return 0;
	    }
		}else{
	    	printf("pam_start failed service Name:[%s]!",service);
	  
	    	return 0;
		}

  #endif

}
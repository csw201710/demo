#include <string.h>  
#include <errno.h>  
#include <sys/socket.h>  
#include <resolv.h>  
#include <stdlib.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <unistd.h>  
#include <openssl/ssl.h>  
#include <openssl/err.h>  
#include <openssl/engine.h>  
#define MAXBUF 1024  

typedef void (*CALLINFO)(const SSL *ssl,int type,int val);
static void info_callback (SSL * s, int where, int ret)
{
	if (where & SSL_CB_LOOP)
	{
		printf ("SSL state (%s): %s\n",
		where & SSL_ST_CONNECT ? "connect" :
		where & SSL_ST_ACCEPT ? "accept" :
			"undefined", SSL_state_string_long (s));
	}
	else if (where & SSL_CB_ALERT)
	{
		printf ("SSL alert (%s): %s: %s\n",
		where & SSL_CB_READ ? "read" : "write",
			SSL_alert_type_string_long (ret),
			SSL_alert_desc_string_long (ret));
	}
	else if (where & SSL_CB_EXIT)
	{
		printf ("SSL state (%s): %s\n",
		where & SSL_ST_CONNECT ? "connect" :
		where & SSL_ST_ACCEPT ? "accept" :
			"undefined", SSL_state_string_long (s));
	}
}
  
ENGINE *setup_engine(const char *engine, int debug)
{
	ENGINE *e = NULL;

	if (engine)
	{
		if(strcmp(engine, "auto") == 0)
		{
			printf("enabling auto ENGINE support\n");
			ENGINE_register_all_complete();
			return NULL;
		}
		if((e = ENGINE_by_id(engine)) == NULL)
		{
			printf("invalid engine \"%s\"\n", engine);
			return NULL;
		}
		if (debug)
		{
			ENGINE_ctrl(e, ENGINE_CTRL_SET_LOGSTREAM,0, NULL, 0);
		}
    ENGINE_ctrl_cmd(e, "SET_USER_INTERFACE", 0, NULL, 0, 1);
		if(!ENGINE_set_default(e, ENGINE_METHOD_ALL))
		{
			printf("can't use that engine\n");
			ENGINE_free(e);
			return NULL;
		}

		printf("engine \"%s\" set.\n", ENGINE_get_id(e));

		/* Free our "structural" reference. */
		ENGINE_free(e);
	}
	return e;
}

void ShowCerts(SSL * ssl)  
{  
    X509 *cert;  
    char *line;  
  
    cert = SSL_get_peer_certificate(ssl);  
    if (cert != NULL) {  
        printf("数字证书信息:\n");  
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);  
        printf("证书: %s\n", line);  
        free(line);  
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);  
        printf("颁发者: %s\n", line);  
        free(line);  
        X509_free(cert);  
    } else  
        printf("无证书信息！\n");  
}  


int verify_callback(int ok, X509_STORE_CTX *ctx){
	printf("[verify_callback] start ...\n");
	return 1;
}



int main(int argc, char **argv)  
{  
    int sockfd, len;  
    struct sockaddr_in dest;  
    char buffer[MAXBUF + 1];  
    SSL_CTX *ctx;  
    SSL *ssl;  
  
  
    SSL_library_init();  
    OpenSSL_add_all_algorithms();  
    SSL_load_error_strings();  

    ENGINE_load_builtin_engines();
    ENGINE *e=NULL;
    e = setup_engine("sm2", 1);

    ctx = SSL_CTX_new(TLSv1_client_method());  
    if (ctx == NULL) {  
        ERR_print_errors_fp(stdout);  
        exit(1);  
    }  
	if (SSL_CTX_use_certificate_file(ctx, "client01.crt", SSL_FILETYPE_PEM) <= 0) {
		/*ERR_print_errors_fp(stderr);*/
		exit(-2);
	}
	
	if (SSL_CTX_use_PrivateKey_file(ctx, "client01.key", SSL_FILETYPE_PEM) <= 0) {
		/*ERR_print_errors_fp(stderr);*/
		exit(-3);
	}

	if (!SSL_CTX_check_private_key(ctx)) {
		printf("Private key does not match the certificate public key\n");
		exit(-4);
	}  

	SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
	SSL_CTX_load_verify_locations(ctx, "root.cer", "/root");
	SSL_CTX_set_verify(ctx,SSL_VERIFY_PEER|SSL_VERIFY_FAIL_IF_NO_PEER_CERT,verify_callback);

    SSL_CTX_set_cipher_list(ctx,"RSA-SM4-SM3:ECDHE-SM4-SM3");
	int off = 0;
	  off |= SSL_OP_GMVPN_PROTOCOL;
	  off |= SSL_OP_EPHEMERAL_RSA; 
	  SSL_CTX_set_options(ctx,off);
    /* 创建一个 socket 用于 tcp 通信 */  
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {  
        perror("Socket");  
        exit(errno);  
    }  
    printf("socket created\n");  
  
    /* 初始化服务器端（对方）的地址和端口信息 */  
    bzero(&dest, sizeof(dest));  
    dest.sin_family = AF_INET;  
    dest.sin_port = htons(atoi("443"));  
	char *remoteIp="127.0.0.1";
    if (inet_aton(remoteIp, (struct in_addr *) &dest.sin_addr.s_addr) == 0) {  
        perror(argv[1]);  
        exit(errno);  
    }  
    printf("address created\n");  
  
    /* 连接服务器 */  
    if (connect(sockfd, (struct sockaddr *) &dest, sizeof(dest)) != 0) {  
        perror("Connect ");  
        exit(errno);  
    }  
    printf("server connected\n");  
	send(sockfd,"xxxxx",5,0);
    /* 基于 ctx 产生一个新的 SSL */  
    ssl = SSL_new(ctx);  
    SSL_set_fd(ssl, sockfd);  

	SSL_set_info_callback (ssl, (CALLINFO)info_callback); 

    /* 建立 SSL 连接 */  
    if (SSL_connect(ssl) == -1)  
        ERR_print_errors_fp(stderr);  
    else {  
        printf("Connected with %s encryption\n", SSL_get_cipher(ssl));  
        ShowCerts(ssl);  
    }  

    /* 接收对方发过来的消息，最多接收 MAXBUF 个字节 */  
    bzero(buffer, MAXBUF + 1);  
    /* 接收服务器来的消息 */  
    /*len = SSL_read(ssl, buffer, MAXBUF);  
    if (len > 0)  
        printf("接收消息成功:'%s'，共%d个字节的数据\n",  
               buffer, len);  
    else {  
        printf  
            ("消息接收失败！错误代码是%d，错误信息是'%s'\n",  
             errno, strerror(errno));  
        goto finish;  
    } */ 
    bzero(buffer, MAXBUF + 1);  
    strcpy(buffer, "from client->server");  
    /* 发消息给服务器 */  
    len = SSL_write(ssl, buffer, strlen(buffer));  
    if (len < 0)  
        printf  
            ("消息'%s'发送失败！错误代码是%d，错误信息是'%s'\n",  
             buffer, errno, strerror(errno));  
    else  
        printf("消息'%s'发送成功，共发送了%d个字节！\n",  
               buffer, len);  
  
  finish:  
    /* 关闭连接 */  
    SSL_shutdown(ssl);  
    SSL_free(ssl);  
    close(sockfd);  
    SSL_CTX_free(ctx);  
    return 0;  
}

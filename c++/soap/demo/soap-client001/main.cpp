#include "calc.nsmap"

#include "soapcalcProxy.h"


/*
//gsoap ns service name:	calc Simple calculator service described at https://www.genivia.com/dev.html
//gsoap ns service protocol:	SOAP
//gsoap ns service style:	rpc
//gsoap ns service encoding:	encoded
//gsoap ns service namespace:	http://websrv.cs.fsu.edu/~engelen/calc.wsdl
//gsoap ns service location:	http://localhost:80

//gsoap ns schema namespace:	urn:calc

//gsoap ns service method: add Sums two values
int ns__add(double a, double b, double *result);

//gsoap ns service method: sub Subtracts two values
int ns__sub(double a, double b, double *result);

//gsoap ns service method: mul Multiplies two values
int ns__mul(double a, double b, double *result);

//gsoap ns service method: div Divides two values
int ns__div(double a, double b, double *result);

//gsoap ns service method: pow Raises a to b
int ns__pow(double a, double b, double *result);

	soapcpp2.exe -w -x -L  -C -e  calc.h
		-w      don't generate WSDL and schema files
		-x      don't generate sample XML message files
		-e      generate SOAP RPC encoding style bindings
		-L      don't generate soapClientLib/soapServerLib
		-C      generate client-side code only
*/

int add(const char* ip,int port , double num1, double num2, double *sum)
{
	int result = 0;
	calc c;
	soap* pServer_soap = c.soap;
	int ret = 0;
	//    soap_set_namespaces(&add_soap, add_namespaces);
	char endpoint[1024] = { 0 };
	//init
	//soap_init(c.soap);
	soap_init2(c.soap, SOAP_IO_KEEPALIVE, SOAP_IO_KEEPALIVE);
	soap_set_mode(c.soap, SOAP_C_UTFSTRING);
	c.soap->connect_timeout = 3;
	c.soap->accept_timeout = 5;
	c.soap->send_timeout = c.soap->recv_timeout = 120;
	
	sprintf(endpoint,"http://%s:%d", ip, port);
	c.endpoint = endpoint;
	//soap_set_endpoint(c.soap, endpoint);


	ret = c.ns__add(num1, num2, sum);
	if (pServer_soap->error || ret != SOAP_OK)
	{
		printf("soap error:%d,%s,%s\n", pServer_soap->error, *soap_faultcode(pServer_soap), *soap_faultstring(pServer_soap));
		result = pServer_soap->error;
	}
	return result;
}

int main() {
	double sum = 0;
	add("127.0.1.1" , 8081, 1, 2, &sum);
	printf("sum : %.2f\n", sum);
	return 0;
}

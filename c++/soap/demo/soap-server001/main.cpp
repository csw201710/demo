#include "calc.nsmap"

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

soapcpp2.exe -w -x -L  -S -e  calc.h
	-w      don't generate WSDL and schema files
	-x      don't generate sample XML message files
	-e      generate SOAP RPC encoding style bindings
	-L      don't generate soapClientLib/soapServerLib
	-S      generate server-side code only
*/


SOAP_FMAC5 int SOAP_FMAC6 ns__add(struct soap*, double a, double b, double *result) {
	*result = a + b;
	printf("%s\n", __FUNCTION__);
	return 0;
}

SOAP_FMAC5 int SOAP_FMAC6 ns__sub(struct soap*, double a, double b, double *result) {
	*result = a - b;
	printf("%s\n", __FUNCTION__);
	return 0;
}

SOAP_FMAC5 int SOAP_FMAC6 ns__mul(struct soap*, double a, double b, double *result) {
	*result = a * b;
	printf("%s\n", __FUNCTION__);
	return 0;
}

SOAP_FMAC5 int SOAP_FMAC6 ns__div(struct soap*, double a, double b, double *result) {
	*result = a / b;
	printf("%s\n", __FUNCTION__);
	return 0;
}

SOAP_FMAC5 int SOAP_FMAC6 ns__pow(struct soap*, double a, double b, double *result) {
	printf("%s\n", __FUNCTION__);
	return 0;
}

int main(int argc, char* argv[])
{

	int m, s; /* master and slave sockets */
	struct soap add_soap;
	soap_init(&add_soap);
	
	
	//soap_set_namespaces(&add_soap, add_namespaces);
	m = soap_bind(&add_soap, NULL, 8081, 100);
	if (m < 0)
	{
		soap_print_fault(&add_soap, stderr);
		exit(-1);
	}

	fprintf(stderr, "Socket connection successful: master socket = %d\n", m);
	for (; ; )
	{
		s = soap_accept(&add_soap);
		if (s < 0)
		{
			soap_print_fault(&add_soap, stderr);
			exit(-1);
		}
		fprintf(stderr, "Socket connection successful: socket = %d\n", s);

		soap_serve(&add_soap);//该句说明该server的服务
		soap_end(&add_soap);
	}
	return 0;
}

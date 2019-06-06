soap test:
	//ns__  命名空间
	add.h:
		int ns__add( int num1, int num2, int* sum );
	soapcpp2  add.h
		add.h           ns.add.res.xml  ns.wsdl  soapC.cpp       soapClientLib.cpp  soapServer.cpp     soapStub.h
		ns.add.req.xml  ns.nsmap        ns.xsd   soapClient.cpp  soapH.h            soapServerLib.cpp

	add.cpp 作为server:
		#include "soapH.h"  // include the generated source code headers
		#include "ns.nsmap" // include XML namespaces
		int main(int argc, char **argv)   
		{  
			int m, s;  
			struct soap add_soap;  
			soap_init(&add_soap);  
			soap_set_namespaces(&add_soap, namespaces);  


			m = soap_bind(&add_soap, NULL, atoi("1234"), 100);  
			if (m < 0) {  
				soap_print_fault(&add_soap, stderr);  
				exit(-1);  
			}  
			fprintf(stderr, "Socket connection successful: master socket = %d\n", m);  
			for (;;) {  
				s = soap_accept(&add_soap);  
				if (s < 0) {  
					soap_print_fault(&add_soap, stderr);  
					exit(-1);  
				}  
				fprintf(stderr, "Socket connection successful: slave socket = %d\n", s);  
				soap_serve(&add_soap);  
				soap_end(&add_soap);  
			}  
			return 0;  
		}  
		int ns__add(struct soap *soap, int num1, int num2, int* sum )
		{
		   *sum = num1 + num2;
		  return SOAP_OK;
		} 
	编译add.cpp: (stdsoap2.cpp 在soap源代码中)
		g++ -o add add.cpp soapC.cpp soapServer.cpp stdsoap2.cpp

	client.cpp 客户端:
		#include "soapH.h"
		#include "ns.nsmap" // include XML namespaces

		int main(){
		  struct soap add_soap;
		  int result = 0;
		  int num1 = 1;
		  int num2 = 2;
		  int sum = 0;
		  soap_init(&add_soap);
		  soap_set_namespaces(&add_soap, namespaces);
		  soap_call_ns__add(&add_soap, "127.0.0.1:1234", NULL, num1, num2, &sum);
		  printf("server is %s, num1 is %d, num2 is %d/n", "127.0.0.1:1234", num1, num2, sum);

		  if (add_soap.error) {
			  printf("soap error: %d, %s, %s\n", add_soap.error, *soap_faultcode(&add_soap), *soap_faultstring(&add_soap));
			  result = add_soap.error;
		  }
		  soap_end(&add_soap);
		  soap_done(&add_soap);

		  return 0;
		}
	编译client.cpp:
		g++ -o client client.cpp soapC.cpp soapClient.cpp stdsoap2.cpp

		
		
		
		
	


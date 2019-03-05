#include "soap/soapService.h"
#include "soap/ns.nsmap"
#include <iostream>

using namespace std; 
int main(int argc, char **argv) {
	 Service calc;
	 int port = 65531; 
	 if (calc.run(port)) 
	 { 
	 		cerr << "Calc service run failed" << endl; exit(0); 
	 } 
	return 0; 
}




int Service::add(double a, double b, double* result) { *result = a + b; return SOAP_OK; }
int Service::sub(double a, double b, double* result) { *result = a - b; return SOAP_OK; }
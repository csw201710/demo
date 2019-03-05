#include "soap/soapProxy.h" 
#include "soap/ns.nsmap" 
#include <iostream> 
using namespace std; 
int main(int argc, char **argv) { 
	double a = 4; double b = 2; 
	double result; 
	Proxy calc; 
	calc.soap_endpoint = "http://localhost:65531"; 
	calc.add(a,b,&result); cout<<a<<"+"<<b<<"="<<result<<endl; 
	calc.sub(a,b,&result); cout<<a<<"-"<<b<<"="<<result<<endl; 
	return 0; 
}
#include "msg.pb.h"
#include <fstream>
#include <iostream>
using namespace std;
int main(void) 
{ 
 
    lm::helloworld msg1; 
    msg1.set_id(0x11223344); 
    msg1.set_str("a"); 
    fstream output("./log.bin", ios::out | ios::trunc | ios::binary); 
 
    if (!msg1.SerializeToOstream(&output)) { 
        cerr << "Failed to write msg." << endl; 
        return -1; 
    }        
    return 0; 
}
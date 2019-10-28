
#include "uWS.h"
#include <iostream>
#include <chrono>
#include <cmath>
#include <thread>
#include <fstream>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <atomic>



int main(int argc, char *argv[])
{
	 	uWS::Hub h;
	 	//接收到的数据
    h.onMessage([](uWS::WebSocket<uWS::CLIENT> *ws, char *message, size_t length, uWS::OpCode opCode) {
        char buf[100] ={0};
        memcpy(buf, message, length);
        
        std::cout << "from server: "<< std::string(message, length) << std::endl;
        std::string res = std::to_string(atoi(buf)+1);
        ws->send(res.data(), res.length(), opCode); 
    });
		h.connect("ws://localhost:3000", nullptr);
		h.run();
    return 0;
}

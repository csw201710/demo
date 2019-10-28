
#include "uWS.h"
#include <iostream>
#include <chrono>
#include <cmath>
#include <thread>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <atomic>


int main(int argc, char *argv[])
{
	  uWS::Hub h;
		
    h.onConnection([&h](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {
        ws->send("1234", 4, uWS::OpCode::TEXT, [](uWS::WebSocket<uWS::SERVER> *ws, void *data, bool cancelled, void *reserved) {
            if (data) {
                if (data != (void *) 13) {
                    std::cout << "FAILURE: invalid data passed to send callback!" << std::endl;
                    exit(-1);
                } else {
                    std::cout << "Correct data was sent to send callback" << std::endl;
                }
            } else {
                std::cout << "FAILURE: No data was passed along!" << std::endl;
                exit(-1);
            }
        }, (void *) 13);
        //h.getDefaultGroup<uWS::SERVER>().close();
    });
    //接收到的消息
    h.onMessage([](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
        char buf[100] ={0};
        memcpy(buf, message, length);
        
        std::cout << "from client: "<< std::string(message, length) << std::endl;
        std::string res = std::to_string(atoi(buf)+1);
        sleep(1);
        ws->send(res.data(), res.length(), opCode); 
    });

    h.listen(3000);
    h.run();
	
    
    return 0;
}

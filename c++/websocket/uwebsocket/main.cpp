
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
#include <time.h>

void serveHttp() {
    uWS::Hub h;
    	
		int connections=0;
		
    std::string document = "<h2>Well hello there, this is a basic test!</h2>";

    h.onHttpRequest([&document](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t length, size_t remainingBytes) {
        res->end(document.data(), document.length());
    });
    
    
    h.onHttpConnection([&] (uWS::HttpSocket<uWS::SERVER>  *ws) {
        std::cout << "[Connection] clients: " << ++connections << std::endl;
    });
    h.onHttpDisconnection([&] (uWS::HttpSocket<uWS::SERVER>  *ws) {
        std::cout << "[Disconnection] clients: " << --connections << std::endl;
    });
   
    h.listen(3000);
    h.run();
}

void testConnections() {
    uWS::Hub h;

    h.onError([](void *user) {
        switch ((long) user) {
        case 1:
            std::cout << "1 Client emitted error on invalid URI" << std::endl;
            break;
        case 2:
            std::cout << "2 Client emitted error on resolve failure" << std::endl;
            break;
        case 3:
            std::cout << "3 Client emitted error on connection timeout (non-SSL)" << std::endl;
            break;
        case 5:
            std::cout << "5 Client emitted error on connection timeout (SSL)" << std::endl;
            break;
        case 6:
            std::cout << "6 Client emitted error on HTTP response without upgrade (non-SSL)" << std::endl;
            break;
        case 7:
            std::cout << "7 Client emitted error on HTTP response without upgrade (SSL)" << std::endl;
            break;
        case 10:
            std::cout << "10 Client emitted error on poll error" << std::endl;
            break;
        case 11:
            static int protocolErrorCount = 0;
            protocolErrorCount++;
            std::cout << "11 Client emitted error on invalid protocol" << std::endl;
            if (protocolErrorCount > 1) {
                std::cout << "FAILURE:  " << protocolErrorCount << " errors emitted for one connection!" << std::endl;
                exit(-1);
            }
            break;
        default:
            std::cout << "FAILURE: " << user << " should not emit error!" << std::endl;
            exit(-1);
        }
    });

    h.onConnection([](uWS::WebSocket<uWS::CLIENT> *ws, uWS::HttpRequest req) {
    		//HTTPSocket.cpp:164 webSocket->setUserData(httpSocket->httpUser);
        switch ((long) ws->getUserData()) {
        case 8:
            std::cout << "8 Client established a remote connection over non-SSL" << std::endl;
            ws->close(1000);
            break;
        case 9:
            std::cout << "9 Client established a remote connection over SSL" << std::endl;
            ws->close(1000);
            break;
        default:
            std::cout << "FAILURE: " << ws->getUserData() << " should not connect!" << std::endl;
            exit(-1);
        }
    });

    h.onDisconnection([](uWS::WebSocket<uWS::CLIENT> *ws, int code, char *message, size_t length) {
        std::cout << "Client got disconnected with data: " << ws->getUserData() << ", code: " << code << ", message: <" << std::string(message, length) << ">" << std::endl;
    });

    h.connect("invalid URI", (void *) 1);
    h.connect("invalid://validButUnknown.yolo", (void *) 11);
    h.connect("ws://validButUnknown.yolo", (void *) 2);
    h.connect("ws://echo.websocket.org", (void *) 3, {}, 10);
    h.connect("ws://echo.websocket.org", (void *) 8);
    h.connect("wss://echo.websocket.org", (void *) 5, {}, 10);
    h.connect("wss://echo.websocket.org", (void *) 9);
    h.connect("ws://google.com", (void *) 6);
    h.connect("wss://google.com", (void *) 7);
    h.connect("ws://127.0.0.1:6000", (void *) 10, {}, 60000);

    h.run();
    std::cout << "Falling through testConnections" << std::endl;
}

void testReceivePerformance() {
    // Binary "Rock it with HTML5 WebSocket"
    unsigned char webSocketMessage[] = {0x82, 0x9c, 0x37, 0x22, 0x79, 0xa5, 0x65, 0x4d, 0x1a, 0xce, 0x17, 0x4b, 0x0d, 0x85, 0x40, 0x4b
    ,0x0d, 0xcd, 0x17, 0x6a, 0x2d, 0xe8, 0x7b, 0x17, 0x59, 0xf2, 0x52, 0x40, 0x2a, 0xca, 0x54, 0x49
    ,0x1c, 0xd1};

//    // Text "Rock it with HTML5 WebSocket"
//    unsigned char webSocketMessage[] = {0x81, 0x9c, 0x37, 0x22, 0x79, 0xa5, 0x65, 0x4d, 0x1a, 0xce, 0x17, 0x4b, 0x0d, 0x85, 0x40, 0x4b
//    ,0x0d, 0xcd, 0x17, 0x6a, 0x2d, 0xe8, 0x7b, 0x17, 0x59, 0xf2, 0x52, 0x40, 0x2a, 0xca, 0x54, 0x49
//    ,0x1c, 0xd1};

//    // Pong "Rock it with HTML5 WebSocket"
//    unsigned char webSocketMessage[] = {0x8a, 0x9c, 0x37, 0x22, 0x79, 0xa5, 0x65, 0x4d, 0x1a, 0xce, 0x17, 0x4b, 0x0d, 0x85, 0x40, 0x4b
//    ,0x0d, 0xcd, 0x17, 0x6a, 0x2d, 0xe8, 0x7b, 0x17, 0x59, 0xf2, 0x52, 0x40, 0x2a, 0xca, 0x54, 0x49
//    ,0x1c, 0xd1};

    // time this!
    int messages = 1000000;
    size_t bufferLength = sizeof(webSocketMessage) * messages;
    char *buffer = new char[bufferLength + 4];
    char *originalBuffer = new char[bufferLength];
    for (int i = 0; i < messages; i++) {
        memcpy(originalBuffer + sizeof(webSocketMessage) * i, webSocketMessage, sizeof(webSocketMessage));
    }

    uWS::Hub h;

    h.onConnection([originalBuffer, buffer, bufferLength, messages, &h](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {
        for (int i = 0; i < 100; i++) {
            memcpy(buffer, originalBuffer, bufferLength);

            auto now = std::chrono::high_resolution_clock::now();
            uWS::WebSocketProtocol<uWS::SERVER, uWS::WebSocket<uWS::SERVER>>::consume(buffer, bufferLength, ws);
            int us = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - now).count();

            std::cout << "Messages per microsecond: " << (double(messages) / double(us)) << std::endl;
        }

        h.getDefaultGroup<uWS::SERVER>().close();
    });

    h.onMessage([](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {

    });

    h.listen(3000);
    h.connect("ws://localhost:3000", nullptr);
    h.run();

    delete [] buffer;
    delete [] originalBuffer;
}
void testSendCallback() {
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
        h.getDefaultGroup<uWS::SERVER>().close();
    });

    h.listen(3000);
    h.connect("ws://localhost:3000", nullptr);
    h.run();
}
void serveEventSource() {
    uWS::Hub h;

    std::string document = "<script>var es = new EventSource('/eventSource'); es.onmessage = function(message) {document.write('<p><b>Server sent event:</b> ' + message.data + '</p>');};</script>";
    std::string header = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";

    // stop and delete the libuv timer on http disconnection
    h.onHttpDisconnection([](uWS::HttpSocket<uWS::SERVER> *s) {
        uS::Timer *timer = (uS::Timer *) s->getUserData();
        if (timer) {
            timer->stop();
            timer->close();
        }
    });

    // terminate any upgrade attempt, this is http only
    h.onHttpUpgrade([](uWS::HttpSocket<uWS::SERVER> *s, uWS::HttpRequest req) {
        s->terminate();
    });

    // httpRequest borde vara defaultsatt till att hantera upgrades, ta bort onupgrade! (s盲tter man request avs盲tts upgrade handlern)
    h.onHttpRequest([&h, &document, &header](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t length, size_t remainingBytes) {
        std::string url = req.getUrl().toString();

        if (url == "/") {
            // respond with the document
            res->end((char *) document.data(), document.length());
            return;
        } else if (url == "/eventSource") {

            if (!res->getUserData()) {
                // establish a text/event-stream connection where we can send messages server -> client at any point in time
                res->write((char *) header.data(), header.length());

                // create and attach a libuv timer to the socket and let it send messages to the client each second
                uS::Timer *timer = new uS::Timer(h.getLoop());
                timer->setData(res);
                timer->start([](uS::Timer *timer) {
                    // send a message to the browser
                    std::string message = "data: Clock sent from the server: " + std::to_string(time(0)) + "\n\n";
                    ((uWS::HttpResponse *) timer->getData())->write((char *) message.data(), message.length());
                }, 1000, 1000);
                res->setUserData(timer);
            } else {
                // why would the client send a new request at this point?
                res->getHttpSocket()->terminate();
            }
        } else {
            res->getHttpSocket()->terminate();
        }
    });

    h.listen(3000);
    h.run();
}

void cb(uS::Timer *){
	std::cout << "timer activited \r\n";
}
void test(){
		std::vector<int> a;
		int t = 10;
    a.insert(
        std::upper_bound(a.begin(), a.end(), t, [](const int &a, const int &b) {
            return a < b;
        }),
        t
    );
    a.push_back(1);
    
    
    uS::Loop * r = uS::Loop::createLoop();
   	
    uS::Timer *timer = new uS::Timer(r);
    timer->start([](uS::Timer *timer) { std::cout << "timer activited \r\n"; }, 1000, 1000);
    //timer->start(cb, 100, 2);
    r->run();
		std::cout << "ok\n";
		sleep(1000* 10);
}
void serveBenchmark() {
    uWS::Hub h;

    h.onMessage([&h](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
        ws->send(message, length, opCode);
    });

    //h.getDefaultGroup<uWS::SERVER>().startAutoPing(1000);
    h.listen(3000);
    h.run();
}
void measureInternalThroughput(unsigned int payloadLength, int echoes, bool ssl) {
    uWS::Hub h;

    char *payload = new char[payloadLength];
    for (unsigned int i = 0; i < payloadLength; i++) {
        payload[i] = rand();
    }

    const char *closeMessage = "I'm closing now";
    size_t closeMessageLength = strlen(closeMessage);

    uS::TLS::Context c = uS::TLS::createContext("misc/ssl/cert.pem",
                                                "misc/ssl/key.pem",
                                                "1234");

    h.onConnection([payload, payloadLength, echoes](uWS::WebSocket<uWS::CLIENT> *ws, uWS::HttpRequest req) {
        for (int i = 0; i < echoes; i++) {
            ws->send(payload, payloadLength, uWS::OpCode::BINARY);
        }
    });

    h.onError([](void *user) {
        std::cout << "FAILURE: Connection failed! Timeout?" << std::endl;
        exit(-1);
    });

    h.onMessage([payload, payloadLength, closeMessage, closeMessageLength, echoes](uWS::WebSocket<uWS::CLIENT> *ws, char *message, size_t length, uWS::OpCode opCode) {
        if (strncmp(message, payload, payloadLength) || payloadLength != length) {
            std::cout << "FAIL: Messages differ!" << std::endl;
            exit(-1);
        }

        static int echoesPerformed = 0;
        if (echoesPerformed++ == echoes) {
            echoesPerformed = 0;
            ws->close(1000, closeMessage, closeMessageLength);
        } else {
            ws->send(payload, payloadLength, opCode);
        }
    });

    h.onDisconnection([](uWS::WebSocket<uWS::CLIENT> *ws, int code, char *message, size_t length) {
        std::cout << "CLIENT CLOSE: " << code << std::endl;
    });

    h.onMessage([](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
        ws->send(message, length, opCode);
    });

    h.onDisconnection([&h, closeMessage, closeMessageLength](uWS::WebSocket<uWS::SERVER> *ws, int code, char *message, size_t length) {
        std::cout << "SERVER CLOSE: " << code << std::endl;
        if (!length) {
            std::cout << "FAIL: No close message! Code: " << code << std::endl;
            exit(-1);
        }

        if (strncmp(message, closeMessage, closeMessageLength) || closeMessageLength != length) {
            std::cout << "FAIL: Close message differ!" << std::endl;
            exit(-1);
        }

        if (code != 1000) {
            std::cout << "FAIL: Close code differ!" << std::endl;
            exit(-1);
        }

        h.getDefaultGroup<uWS::SERVER>().close();
    });

    // we need to update libuv internal timepoint!
#ifndef USE_ASIO
    h.run();
#endif

    if (ssl) {
        if (!h.listen(3000, c)) {
            std::cout << "FAIL: ERR_LISTEN" << std::endl;
            exit(-1);
        }
        h.connect("wss://localhost:3000", nullptr);
    } else {
        if (!h.listen(3000)) {
            std::cout << "FAIL: ERR_LISTEN" << std::endl;
            exit(-1);
        }
        h.connect("ws://localhost:3000", nullptr);
    }

    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    start = std::chrono::high_resolution_clock::now();
    h.run();
    end = std::chrono::high_resolution_clock::now();
    int ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    if (!ms) {
        std::cout << "Too small load!" << std::endl;
    } else {
        std::cout << "Throughput: " << (float(echoes) / float(ms)) << " echoes/ms (" << ms << " ms elapsed)" << std::endl;
    }

    delete [] payload;
}
void testStress() {
    for (int i = 0; i < 25; i++) {
        int payloadLength = std::pow(2, i);
        int echoes = 1;//std::max<int>(std::pow(2, 24 - i) / 50, 1);

        std::cout << "[Size: " << payloadLength << ", echoes: " << echoes << "]" << std::endl;
        for (int ssl = 0; ssl < 2; ssl++) {
            std::cout << "SSL: " << bool(ssl) << std::endl;
            measureInternalThroughput(payloadLength, echoes, ssl);
        }
    }
}

void testAutoPing() {
    uWS::Hub h;

    int pongs = 0, pings = 0;

    h.onPing([&pings](uWS::WebSocket<uWS::CLIENT> *ws, char *message, size_t length) {
        std::cout << "PING" << std::endl;
        pings++;
    });

    h.onMessage([](uWS::WebSocket<uWS::CLIENT> *ws, char *message, size_t length, uWS::OpCode opCode) {
        std::cout << std::string(message, length) << std::endl;
        ws->send(message, length, opCode);
    });

    h.onPong([&pings, &pongs, &h](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length) {
        std::cout << "PONG" << std::endl;
        pongs++;

        if (pongs == 3) {
            if (pings != pongs) {
                std::cout << "FAILURE: mismatching ping/pongs" << std::endl;
                exit(-1);
            }
            h.getDefaultGroup<uWS::SERVER>().close();
        }
    });

    h.getDefaultGroup<uWS::SERVER>().startAutoPing(1000);
    h.listen(3000);
    h.connect("ws://localhost:3000", nullptr);
    h.run();
}

void testSmallSends() {
    uWS::Hub h;

    int length = 0;
    h.onConnection([&h, &length](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {
        std::cout << "Connected, will send small messages now" << std::endl;
        while (length < 2048) {
            char *message = new char[length];
            memset(message, 0, length);
            ws->send(message, length, uWS::OpCode::TEXT);
            delete [] message;
            length++;
        }
        h.getDefaultGroup<uWS::SERVER>().close();
    });

    h.listen(3000);
    h.connect("ws://localhost:3000", nullptr);

    h.run();
    std::cout << "Falling through testSmallSends" << std::endl;
}
int testEcho()
{
    uWS::Hub h;

    h.onMessage([](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
    	std::cout << "this is a out message\n";
        ws->send(message, length, opCode);
    });

    if (h.listen(3000)) {
        h.run();
    }
}

void testTransfers() {
    for (int ssl = 0; ssl < 2; ssl++) {
        uWS::Group<uWS::SERVER> *tServerGroup = nullptr;
        uWS::Group<uWS::CLIENT> *clientGroup = nullptr;

        int receivedMessages = 0;

        std::mutex m;
        uWS::WebSocket<uWS::CLIENT> *client;

        std::thread t([&tServerGroup, &client, &receivedMessages, &clientGroup, &m]{
            uWS::Hub th;
            tServerGroup = &th.getDefaultGroup<uWS::SERVER>();

            bool transferred = false;

            th.onTransfer([&transferred](uWS::WebSocket<uWS::SERVER> *ws) {
                if (ws->getUserData() != (void *) 12345) {
                    std::cout << "onTransfer called with websocket with invalid user data set!" << std::endl;
                    exit(-1);
                }

                transferred = true;
            });

            th.onMessage([&tServerGroup, &client, &receivedMessages, &clientGroup, &m, &transferred](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
                if (!transferred) {
                    std::cout << "FAILURE: onTransfer was not triggered in time" << std::endl;
                    exit(-1);
                }

                switch(++receivedMessages) {
                case 1:
                    m.lock();
                    client->send("second message");
                    m.unlock();
                    break;
                case 2: {
                    const char *str = "some broadcast here";
                    clientGroup->broadcast(str, strlen(str), uWS::OpCode::TEXT);
                    break;
                }
                case 3: {
                    usleep(10000);
                    char *tmp = new char[1024 * 1024 * 16];
                    memset(tmp, 0, 1024 * 1024 * 16);
                    client->send(tmp, 1024 * 1024 * 16, uWS::OpCode::BINARY);
                    delete [] tmp;
                    break;
                }
                case 4:
                    tServerGroup->close();
                    break;
                }

                if (opCode != uWS::OpCode::BINARY) {
                    std::cout << "Message " << receivedMessages << ": " << std::string(message, length) << std::endl;
                } else {
                    std::cout << "Message " << receivedMessages << ": binary" << std::endl;
                }
            });

            th.getDefaultGroup<uWS::SERVER>().listen(uWS::TRANSFERS);
            th.run();
        });

        // we do not care about correctness here!
        sleep(1);

        uWS::Hub h;

        clientGroup = &h.getDefaultGroup<uWS::CLIENT>();

        clientGroup->listen(uWS::TRANSFERS);

        h.onConnection([&tServerGroup](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {
            ws->setUserData((void *) 12345);
            ws->transfer(tServerGroup);
        });

        h.onConnection([&client, &m](uWS::WebSocket<uWS::CLIENT> *ws, uWS::HttpRequest req) {
            m.lock();
            client = ws;
            ws->send("first message here");
            m.unlock();
        });

        h.onDisconnection([&h](uWS::WebSocket<uWS::CLIENT> *ws, int code, char *message, size_t length) {
            h.getDefaultGroup<uWS::SERVER>().close();
            h.getDefaultGroup<uWS::CLIENT>().close();
        });

        if (ssl) {
            if (!h.listen(3000,
                          uS::TLS::createContext("misc/ssl/cert.pem",
                          "misc/ssl/key.pem", "1234"))) {
                std::cerr << "FAILURE: Cannot listen!" << std::endl;
                exit(-1);
            }
            h.connect("wss://localhost:3000", nullptr);
        } else {
            if (!h.listen(3000)) {
                std::cerr << "FAILURE: Cannot listen!" << std::endl;
                exit(-1);
            }
            h.connect("ws://localhost:3000", nullptr);
        }

        h.run();
        t.join();
    }
    std::cout << "Falling through testMultithreading" << std::endl;
}

int main(int argc, char *argv[])
{
		//test();
		
		//serveEventSource();
		serveHttp();
		//serveBenchmark();
		
		//testReceivePerformance();
		//testStress();
		//testAutoPing();
		//testSmallSends();
		//testEcho();
		//testTransfers();
	//	testConnections();
		
		//testSendCallback();
		
		
    return 0;
}



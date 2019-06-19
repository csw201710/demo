all: write read
clean:
	rm -f write reader msg.*.cc msg.*.h *.o  log

CFLAGS +=  -Wl,-Bstatic `pkg-config --cflags --libs protobuf `  -Wl,-Bdynamic

proto_msg:
	protoc --cpp_out=. msg.proto


write:  proto_msg write.cpp
	g++  msg.pb.cc write.cpp -o write   $(CFLAGS) -std=c++11

read: proto_msg read.cpp
	g++  msg.pb.cc read.cpp -o reader  $(CFLAGS) -std=c++11
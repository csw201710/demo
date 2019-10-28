
TARGET := main
DIRS := ./ 
SOURCES := $(foreach dir,$(DIRS),$(wildcard $(dir)*.cpp))


SOURCES := $(filter-out ./client.cpp ./server.cpp, $(SOURCES))


OBJS := $(notdir $(SOURCES:.cpp=.o))
OBJ_PATH := ./objs/
OBJS_O := $(addprefix $(OBJ_PATH), $(OBJS))




CC := g++ -g -std=c++11

LIBPATH :=  -L./  

LIBS := -lpthread -L. -luWS -lssl -lcrypto -lz -luv -lboost_system -lboost_thread -lboost_filesystem

INCLUDE:= -I./ -I/root/uWebSockets-0.14.8/src -I/usr/local/ssl/include/

CFLAGS :=  $(INCLUDE)  -DUSE_ASIO

all: $(OBJ_PATH) $(TARGET)

$(OBJ_PATH):
	mkdir -p ./$@


$(TARGET): $(OBJS_O)	
	@echo 'Linking......'	
	$(CC) -o $@ $^ $(LIBPATH) $(LIBS)
	@echo 'Finished Linking: $@'	
	@echo 'success!'

$(OBJS_O): $(SOURCES)	
	@echo 'Building file: $(SOURCES)'	
	$(CC) $(CFLAGS) -o $@ -c $(filter %$(*F).cpp,$(SOURCES))	
	@echo 'Finished building: $@'	
	@echo ''
clean:
	rm -rf $(OBJ_PATH)/*  $(TARGET) 
	rm -rf client server

cs : clean client server
.PHONY: client server
client:
	$(CC) client.cpp -o client $(CFLAGS) $(LIBS) $(LIBPATH)

server:
	$(CC) server.cpp -o server $(CFLAGS) $(LIBS) $(LIBPATH)


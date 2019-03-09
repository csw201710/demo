TARGET := main
DIRS := ./ 
SOURCES := $(foreach dir,$(DIRS),$(wildcard $(dir)*.cpp))


SOURCES := $(filter-out ./test.cpp, $(SOURCES))


OBJS := $(notdir $(SOURCES:.cpp=.o))
OBJ_PATH := ./objs/
OBJS_O := $(addprefix $(OBJ_PATH), $(OBJS))




CC := g++  -g

CFLAGS :=  -std=c++11  -D_DEBUG 

INCLUDE:= -I./ 


LIBPATH :=  -L./  


LIBS :=  -ldl  -Wl,-Bstatic  -lcppunit -Wl,-Bdynamic 


all: $(OBJ_PATH)  $(TARGET) 

$(OBJ_PATH):
	mkdir -p ./$@


$(TARGET): $(OBJS_O) 
	@echo 'Linking......'	
	$(CC)  -o $@ $^ $(LIBPATH) $(LIBS) $(CFLAGS)
	@echo 'Finished Linking: $@'	
	@echo 'success!'

$(OBJS_O): $(SOURCES)	
	@echo 'Building file: $(SOURCES)'	
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $(filter %$(*F).cpp,$(SOURCES))
	@echo 'Finished building: $@'	
	@echo ''
clean:
	rm -rf $(OBJ_PATH)/*  $(TARGET) *.o



cs : clean-test test
.PHONY: test test-gmssl
test:
	$(CC)  test.cpp  -o $@ -std=c++11  -Wl,-rpath=./ -fPIC  $(INCLUDE) $(LIBS) $(LIBPATH) -lz -ldl -lcrypt 

clean-test:
	rm -rf test -rf

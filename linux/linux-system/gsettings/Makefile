TARGET := main
DIRS := ./
SOURCES := $(foreach dir,$(DIRS),$(wildcard $(dir)*.cpp))

OBJS := $(notdir $(SOURCES:.cpp=.o))
OBJ_PATH := ./objs/
OBJS_O := $(addprefix $(OBJ_PATH), $(OBJS))




CC := gcc

LIBPATH := -L./

LIBS := -lpthread  `pkg-config --libs --cflags gio-2.0`

INCLUDE:= -I./

CFLAGS :=  $(INCLUDE) 

all: $(OBJ_PATH) $(TARGET)

$(OBJ_PATH):
	mkdir -p ./$@


$(TARGET): $(OBJS_O)	
	@echo 'Linking......'	
	$(CC) -o $@ $^ $(LIBS) $(LIBPATH)	
	@echo 'Finished Linking: $@'	
	@echo ' '

$(OBJS_O): $(SOURCES)	
	@echo 'Building file: $(filter %$(*F).cpp,$(SOURCES))'	
	$(CC) $(CFLAGS) -o $@ -c $(filter %$(*F).cpp,$(SOURCES))	$(LIBS)
	@echo 'Finished building: $@'	
	@echo ' '


clean:
	rm -f $(OBJS_O)

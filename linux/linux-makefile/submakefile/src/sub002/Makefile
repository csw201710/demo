#./src/main/Makefile
OBJ = main.o

$(OBJ):$(OBJ:.o=.c)
	$(CC) -c $^

clean:
	-rm $(OBJ)
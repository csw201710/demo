cc = g++ -g
prom = main
obj = main.o thread.o Log.o
cflags = -levent -pthread
$(prom): $(obj)
	$(cc) -o $(prom) $(obj) $(cflags)

%.o: %.cpp
	$(cc) -D_DEBUG -c $< -o $@
clean:
	rm main *.o -f

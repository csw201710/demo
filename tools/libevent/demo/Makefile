cc = gcc
prom = main
obj = main.o
cflags = -levent -pthread
$(prom): $(obj)
	$(cc) -o $(prom) $(obj) $(cflags)

%.o: %.c $(deps)
	$(cc) -c $< -o $@

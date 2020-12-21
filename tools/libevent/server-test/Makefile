cc = g++ -g
prom = main
obj = main.o thread.o Log.o

cflags = -levent -pthread
include_path =

#ssl
ssl_cflags = -Wl,-Bstatic -lssl -lcrypto -Wl,-Bdynamic -ldl
ssl_include_path = -I/root/code/GmSSL/include
ssl_library_path = -L/root/code/GmSSL/

ifdef TLS
include_path = $(ssl_include_path)
cflags += $(ssl_library_path) $(ssl_cflags)
endif

$(prom): $(obj)
	$(cc) -o $(prom) $(obj) $(cflags) $(library_path)
%.o: %.cpp
	$(cc) -D_DEBUG  $(include_path) -c $< -o $@
clean:
	rm main *.o -f

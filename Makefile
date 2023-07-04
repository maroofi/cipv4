CC := gcc
CFLAGS := -I./include
SHELL = /bin/bash


OUTDIR = bin
DEPS = $(wildcard ./src/*.c)
HDEPS = $(wildcard ./include/*.h)
TESTDEPS = test/test_1.c
TESTDEPS2 = test/test_ip.c
LIBNAME = libcipv4.so.1
OBJS = util_string.o cipv4.o

cipv4: dummy $(OBJS) $(HDEPS)
	$(CC) -shared $(CFLAGS) $(addprefix bin/, $(OBJS)) -Wl,-soname,$(LIBNAME) $ -o bin/$(LIBNAME)

util_string.o: ./src/util_string.c ./include/util_string.h
	$(CC) $(CFLAGS) -fPIC -c $< -o bin/$@

cipv4.o: ./src/cipv4.c ./include/util_string.h
	$(CC) $(CFLAGS) -fPIC -c $< -o bin/$@

dummy:
	mkdir -p bin

.PHONY: test
test: $(TESTDEPS) $(TESTDEPS2) $(DEPS) $(HDEPS)
	$(CC) $(CFLAGS) $(DEPS) $(TESTDEPS) -o test/test_1
	$(CC) $(CFLAGS) $(DEPS) $(TESTDEPS2) -o test/test_ip

.PHONY: clean
clean:
	rm -f bin/$(LIBNAME) test/test_1 test/test_ip bin/*.o


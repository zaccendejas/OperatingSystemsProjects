CC=       	gcc
CFLAGS= 	-g -gdwarf-2 -std=gnu99 -Wall
LDFLAGS=
LIBRARIES=      lib/libmalloc-ff.so \
		lib/libmalloc-nf.so \
		lib/libmalloc-bf.so \
		lib/libmalloc-wf.so
TESTS= bin/Benchmark

all:    $(LIBRARIES) $(TESTS)

lib/libmalloc-ff.so:     src/malloc.c
	$(CC) -shared -fPIC $(CFLAGS) -DFIT=0 -o $@ $< $(LDFLAGS)

lib/libmalloc-nf.so:	src/malloc.c
	$(CC) -shared -fPIC $(CFLAGS) -DFIT=1 -o $@ $< $(LDFLAGS)

lib/libmalloc-bf.so:	src/malloc.c
	$(CC) -shared -fPIC $(CFLAGS) -DFIT=2 -o $@ $< $(LDFLAGS)

lib/libmalloc-wf.so:	src/malloc.c
	$(CC) -shared -fPIC $(CFLAGS) -DFIT=3 -o $@ $< $(LDFLAGS)

bin/Benchmark:	tests/Benchmark.c
	$(CC) $(CFLAGS) tests/Benchmark.c -o $@

clean:
	rm -f $(LIBRARIES) $(TESTS)

.PHONY: all clean

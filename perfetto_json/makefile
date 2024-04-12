.PHONY: clean build

# LIB=-l ...
# LIB_PATH=-L ...

CC = gcc
CFLAGS = -I -I$(PWD) -g -O0

build:
	$(CC) $(CFLAGS) -c trace_json.c trace_json.o
	$(CC) $(CFLAGS) -c test.c -o test.o
	$(CC) $(CFLAGS) -o test test.o trace_json.o $(LIB_PATH) $(LIB)

clean:
	rm -rf test *.o *.json

run:
	./test
	cat trace.json
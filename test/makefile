.PHONY : build clean all
all : clean build
build:
	g++ -o test_c test.c -I/home/pi/repo/perfetto_c -L../ -lstdc++ -lperfetto_c -lperfetto
	g++ -o test_cpp test.cpp -I/home/pi/repo/perfetto_c -L../ -lperfetto_c -lperfetto 
clean:
	rm -f test_c test_cpp
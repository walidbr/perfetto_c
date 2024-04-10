.PHONY : build clean all
all : clean build
build:
	g++ -o test_c test.c -I../ -L../ -lstdc++ -lperfetto_c -lperfetto
	g++ -o test_standalone test_standalone.c -I../ -L../ -lstdc++ -lperfetto_c -lperfetto
	g++ -o test_cpp test.cpp -I../ -L../ -lperfetto_c -lperfetto
clean:
	rm -f test_c test_cpp test_standalone

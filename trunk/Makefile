all: build run
speed: buildOpt run

build:
	gcc -c -g3 -pthread src/*.c && g++ -c part3_main.cpp  && g++ -g3 -pthread  *.o -o main

buildOpt:
	gcc -c -O3 -w src/*.c && g++ -c -O3 part3_main.cpp && g++ -O3 -w *.o -o main

run:
	./main

clean:
	rm -f main
tag:
		(test ! -s tags || rm -v tags) && ctags -R

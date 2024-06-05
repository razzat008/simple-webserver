SRC = main.cpp

server: src/${SRC}
	mkdir -p bin
	g++ src/${SRC} -o bin/server
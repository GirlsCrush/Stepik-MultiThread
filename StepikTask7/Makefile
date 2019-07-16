all : build_server

build_server : server.cpp
	g++ -std=c++11 -o server server.cpp

.PHONY = clean
clean :
	rm -fr *.o	
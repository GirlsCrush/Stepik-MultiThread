all : build_server build_client

build_server : server.cpp
	g++ -O0 -o server server.cpp
build_client : client.cpp
	g++ -O0 -o client client.cpp

.PHONY = clean
clean :
	rm -fr *.o	
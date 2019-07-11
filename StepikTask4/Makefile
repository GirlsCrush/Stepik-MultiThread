all : build_server build_client

build_server : server.cpp
	g++ -ggdb -O0 -o server server.cpp -lws2_32

build_client : client.cpp
	g++ -ggdb -O0 -o client client.cpp -lws2_32

.PHONY = clean
clean :
	rm -fr *.o	
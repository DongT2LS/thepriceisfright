all: server client

server: server.cpp model/* controller/*
	g++ -o server server.cpp model/* controller/* -pthread

client: client.cpp model/request.hpp
	g++ client.cpp model/request.hpp -o client -pthread

.PHONY: clean

clean:
	rm -f server client
CFLAG=-Wfatal-errors

all: c s

t: mfilegenerator.o merror.o api.o 
	g++ ${CFLAG} -o test_unit mfilegenerator.o merror.o api.o

c: api.o client.o mconnection.o merror.o
	g++ ${CFLAG} -o client api.o client.o mconnection.o merror.o

s: api.o mfilesystem.o server.o mconnection.o merror.o msystemtree.o goldenipcode.o
	g++ ${CFLAG} -o server api.o mfilesystem.o server.o mconnection.o merror.o msystemtree.o goldenipcode.o

client.o: client.cpp api.hpp mconnection.hpp merror.hpp
	g++ ${CFLAG} -std=c++11 client.cpp -c -o client.o

server.o: server.cpp api.hpp mfilesystem.hpp mconnection.hpp merror.hpp goldenipcode.hpp
	g++ ${CFLAG} -std=c++11 server.cpp -c -o server.o

api.o: api.cpp api.hpp
	g++ ${CFLAG} -std=c++11 api.cpp -c -o api.o

mfilesystem.o: mfilesystem.cpp mfilesystem.hpp merror.hpp msystemtree.hpp
	g++ ${CFLAG} -std=c++11 mfilesystem.cpp -c -o mfilesystem.o

mconnection.o: mconnection.cpp mconnection.hpp merror.hpp
	g++ ${CFLAG} -std=c++11 mconnection.cpp -c -o mconnection.o

msystemtree.o: msystemtree.cpp msystemtree.hpp merror.hpp
	g++ ${CFLAG} -std=c++11 msystemtree.cpp -c -o msystemtree.o

merror.o: merror.cpp merror.hpp
	g++ ${CFLAG} -std=c++11 merror.cpp -c -o merror.o

mfilegenerator.o: mfilegenerator.cpp merror.hpp api.hpp
	g++ ${CFLAG} -std=c++11 mfilegenerator.cpp -c -o mfilegenerator.o

goldenipcode.o: goldenipcode.cpp goldenipcode.hpp
	g++ ${CFLAG} -std=c++11 goldenipcode.cpp -c -o goldenipcode.o
CFLAG=-Wfatal-errors -std=c++11

all: c s

t: mfilegenerator.o merror.o api.o 
	g++ ${CFLAG} -o test_unit mfilegenerator.o merror.o api.o

c: api.o client.o mconnection.o merror.o
	g++ ${CFLAG} -o client api.o client.o mconnection.o merror.o

s: api.o mmdslogicsystem.o server.o mconnection.o merror.o msystemtree.o goldenipcode.o mserver.o mmdtlogicsystem.o
	g++ ${CFLAG} -o server api.o mmdslogicsystem.o server.o mconnection.o merror.o msystemtree.o goldenipcode.o mserver.o mmdtlogicsystem.o

client.o: client.cpp api.hpp mconnection.hpp merror.hpp
	g++ ${CFLAG} client.cpp -c -o client.o

server.o: server.cpp api.hpp mmdslogicsystem.hpp mconnection.hpp merror.hpp goldenipcode.hpp mserver.hpp
	g++ ${CFLAG} server.cpp -c -o server.o

api.o: api.cpp api.hpp
	g++ ${CFLAG} api.cpp -c -o api.o

mmdslogicsystem.o: mmdslogicsystem.cpp mmdslogicsystem.hpp merror.hpp msystemtree.hpp mserver.hpp msystem.hpp
	g++ ${CFLAG} mmdslogicsystem.cpp -c -o mmdslogicsystem.o

mconnection.o: mconnection.cpp mconnection.hpp merror.hpp
	g++ ${CFLAG} mconnection.cpp -c -o mconnection.o

msystemtree.o: msystemtree.cpp msystemtree.hpp merror.hpp mmdslogicsystem.hpp
	g++ ${CFLAG} msystemtree.cpp -c -o msystemtree.o

merror.o: merror.cpp merror.hpp
	g++ ${CFLAG} merror.cpp -c -o merror.o

mfilegenerator.o: mfilegenerator.cpp merror.hpp api.hpp
	g++ ${CFLAG} mfilegenerator.cpp -c -o mfilegenerator.o

goldenipcode.o: goldenipcode.cpp goldenipcode.hpp
	g++ ${CFLAG} goldenipcode.cpp -c -o goldenipcode.o

mserver.o: mserver.cpp mserver.hpp mmdtlogicsystem.hpp mconnection.hpp api.hpp mmdslogicsystem.hpp mmdtlogicsystem.hpp merror.hpp
	g++ ${CFLAG} mserver.cpp -c -o mserver.o

mmdtlogicsystem.o: mmdtlogicsystem.cpp mmdtlogicsystem.hpp merror.hpp msystem.hpp mmetadata.hpp mserver.hpp
	g++ ${CFLAG} mmdtlogicsystem.cpp -c -o mmdtlogicsystem.o

clean: 
	rm *.o client server
all:client proxy

client:client.o simpleHttpProxy.o
	g++ client.o simpleHttpProxy.o -o client

proxy:proxy.o simpleHttpProxy.o
	g++ proxy.o simpleHttpProxy.o -o proxy

client.o:client.cpp
	g++ -c client.cpp

proxy.o:proxy.cpp
	g++ -c proxy.cpp

simpleHttpProxy.o:simpleHttpProxy.cpp
	g++ -c simpleHttpProxy.cpp

clean:
	rm -rf *.o client proxy client

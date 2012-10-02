
CCC = g++
CCFLAGS = -Wall -I. -O3 -ggdb -std=c++0x -DUNIX -DLINUX -pthread
LDFLAGS = -pthread
LIBS = -lpthread
OBJS = helper.o pipeclient.o pipeserver.o pipechannel.o cpthread.o threadmanager.o abstractserverreceiver.o abstractclientreceiver.o serverreceiver.o clientreceiver.o abstractreceiver.o bufferprocessor.o


cppl:: client server

client: client.o $(OBJS)
	$(CCC) client.o $(OBJS) -o client $(LIBS) $(LDFLAGS)
	
server: server.o $(OBJS)
	$(CCC) server.o $(OBJS) -o server $(LIBS) $(LDFLAGS)

		
#tells how to make an *.o object file from an *.c file
%.o: %.cpp
	$(CCC) -c $(CCFLAGS) $< -o $@
	
	
clean::
	rm -f *.o
	rm -f cppl
	rm -f server
	rm -f client
	


CCC = g++
CCFLAGS = -Wall -I. -O3 -ggdb -std=c++0x -DUNIX -DLINUX -pthread
LDFLAGS = -pthread
LIBS = -lpthread

OBJS-CPPL = cppl/helper.o \
            cppl/pipeclient.o \
            cppl/pipeserver.o \
            cppl/pipechannel.o \
            cppl/cpthread.o \
            cppl/threadmanager.o \
            cppl/abstractserverreceiver.o \
            cppl/abstractclientreceiver.o \
            cppl/serverreceiver.o \
            cppl/clientreceiver.o \
            cppl/abstractreceiver.o \
            cppl/bufferprocessor.o

OBJS-UTILS = utils/cthread.o

OBJS-TEST  = test/pctest.o

OBJS-NETWORK = network/ccapsule.o

all:: cppl utils main test network

cppl: cpplclient cpplserver

utils: $(OBJS-UTILS)

test: $(OBJS-TEST) pctest

network: $(OBJS-NETWORK)

pctest: $(OBJS-TEST) utils
	$(CCC) $(OBJS-TEST) $(OBJS-UTILS) -o ./pctest $(LIBS) $(LDFLAGS)
	
cpplclient: cppl/client.o $(OBJS-CPPL)
	$(CCC) cppl/client.o $(OBJS-CPPL) -o ./client $(LIBS) $(LDFLAGS)
	
cpplserver: cppl/server.o $(OBJS-CPPL)
	$(CCC) cppl/server.o $(OBJS-CPPL) -o ./server $(LIBS) $(LDFLAGS)

main: main.o
	$(CCC) main.o $(OBJS-CPPL) -o ./main $(LIBS) $(LDFLAGS)
		
#tells how to make an *.o object file from an *.c file
%.o: %.cpp
	$(CCC) -c $(CCFLAGS) $< -o $@
	
	
clean::
	rm --recursive -f *.o
	rm -f $(BINDIR)/server
	rm -f $(BINDIR)/client
	cd test
	rm -f test/*.o
	rm -f utils/*.o
	rm -f cppl/*.o

	

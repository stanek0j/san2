
CCC = g++
CCFLAGS = -Wall -I$(CURDIR) -O3 -ggdb -std=c++0x -DUNIX -DLINUX -pthread -I.
LDFLAGS = -pthread
LIBS = -lpthread

OBJS-UTILS = utils/cthread.o \
			 utils/platform/sleep.o \
			 utils/cdatapack.o

OBJS-CPPL = cppl/helper.o \
            cppl/pipeclient.o \
            cppl/pipeserver.o \
            cppl/pipechannel.o \
            cppl/bufferprocessor.o

OBJS-NETWORK = network/ccapsule.o

OBJS-RPC = rpc/cirpcchannel.o \
		   rpc/crpcexecutor.o \
		   rpc/cirpcfunction.o

OBJS-EXAMPLES-UTILS-PRODUCERCONSUMER = examples/utils/producerconsumer/pctest.o

OBJS-EXAMPLES-UTILS-VECTOR = examples/utils/vector/vector.o

OBJS-STREAM = stream/cistreamrw.o \
			  stream/clenvalueseparator.o
			  
OBJS-COMM = comm/cpplrpcchannel.o \
			comm/cpplstreamrw.o

OBJS-TCP =  tcp/ctcpserver.o \
			tcp/ctcpchannel.o \
			tcp/citcpreceiver.o \
			tcp/ctcpclient.o \
			tcp/tcphelper.o


OBJS-EXAMPLES-CPPL-SERVER = examples/cppl/server/server.o \
							examples/cppl/server/cchannel.o
					 
OBJS-EXAMPLES-CPPL-CLIENT = examples/cppl/client/client.o
							
OBJS-EXAMPLES-CPPL-TERMINAL = examples/cppl/terminal/terminal.o \
				examples/cppl/terminal/terminalreceiver.o

OBJS-EXAMPLES-RPC-SERVER = examples/rpc/server/main.o \
						   examples/rpc/server/serverreceiver.o

OBJS-EXAMPLES-RPC-CLIENT = examples/rpc/client/main.o \
						   examples/rpc/server/clientreceiver.o
						   
OBJS-TEST-FUNC = examples/rpc/testfunc.o examples/rpc/multiply.o

all:: components examples

components: utils cppl stream network rpc comm tcp

utils: $(OBJS-UTILS)
cppl: $(OBJS-CPPL)
stream: $(OBJS-STREAM)
network: $(OBJS-NETWORK)
rpc: $(OBJS-RPC)
comm: $(OBJS-COMM)
tcp: $(OBJS-TCP)

examples: examples-utils examples-cppl examples-rpc

examples-utils: examples-utils-producerconsumer examples-utils-vector
examples-cppl: examples-cppl-server examples-cppl-client examples-cppl-terminal
examples-rpc: examples-rpc-server examples-rpc-client

examples-utils-producerconsumer: utils $(OBJS-EXAMPLES-UTILS-PRODUCERCONSUMER)
	$(CCC) $(OBJS-EXAMPLES-UTILS-PRODUCERCONSUMER) $(OBJS-UTILS) -o ./producerconsumer $(LIBS) $(LDFLAGS)

examples-cppl-server: utils cppl $(OBJS-EXAMPLES-CPPL-SERVER) 
	$(CCC) $(OBJS-EXAMPLES-CPPL-SERVER) $(OBJS-UTILS) $(OBJS-CPPL) -o ./cppl_server $(LIBS) $(LDFLAGS)
	
examples-cppl-client: utils cppl $(OBJS-EXAMPLES-CPPL-CLIENT) 
	$(CCC) $(OBJS-EXAMPLES-CPPL-CLIENT) $(OBJS-UTILS) $(OBJS-CPPL) -o ./cppl_client $(LIBS) $(LDFLAGS)

examples-cppl-terminal: utils cppl $(OBJS-EXAMPLES-CPPL-TERMINAL) 
	$(CCC) $(OBJS-EXAMPLES-CPPL-TERMINAL) $(OBJS-UTILS) $(OBJS-CPPL) -o ./terminal $(LIBS) $(LDFLAGS)

examples-utils-vector: utils $(OBJS-EXAMPLES-UTILS-VECTOR)
	$(CCC) $(OBJS-EXAMPLES-UTILS-VECTOR) $(OBJS-UTILS) -o ./utils_vector $(LIBS) $(LDFLAGS)

examples-rpc-server: utils cppl stream comm rpc $(OBJS-EXAMPLES-RPC-SERVER) $(OBJS-TEST-FUNC)
	$(CCC) $(OBJS-UTILS) $(OBJS-CPPL) $(OBJS-STREAM) $(OBJS-COMM) $(OBJS-RPC) $(OBJS-EXAMPLES-RPC-SERVER) $(OBJS-TEST-FUNC)  -o ./rpc_server $(LIBS) $(LDFLAGS)
		
examples-rpc-client: utils cppl stream comm rpc $(OBJS-EXAMPLES-RPC-CLIENT) $(OBJS-TEST-FUNC)
	$(CCC) $(OBJS-UTILS) $(OBJS-CPPL) $(OBJS-STREAM) $(OBJS-COMM) $(OBJS-RPC) $(OBJS-EXAMPLES-RPC-CLIENT) $(OBJS-TEST-FUNC)  -o ./rpc_client $(LIBS) $(LDFLAGS)

#tells how to make an *.o object file from an *.c file
%.o: %.cpp
	$(CCC) -c $(CCFLAGS) $< -o $@
	
	
clean::
	rm -f *.o
	rm -f rpc/*.o
	rm -f comm/*.o
	rm -f utils/*.o
	rm -f utils/platform/*.o
	rm -f cppl/*.o
	rm -f network/*.o
	rm -f stream/*.o
	rm -f tcp/*.o
	rm -f examples/*.o
	rm -f examples/utils/*.o
	rm -f examples/utils/vector/*.o
	rm -f examples/utils/producerconsumer/*.o
	rm -f examples/cppl/*.o
	rm -f examples/rpc/*.o
	rm -f examples/rpc/server/*.o
	rm -f examples/rpc/client/*.o
	rm -f cppl_server
	rm -f cppl_client
	rm -f utils_vector
	rm -f producerconsumer
	rm -f terminal
	rm -f rpc_server
	rm -f rpc_client
	

	

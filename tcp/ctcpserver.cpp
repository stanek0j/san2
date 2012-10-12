
#include "ctcpserver.hpp"
#include "ctcpchannel.hpp"

#define CTCP_MAXCONNQUEUE 10

namespace San2 { namespace Tcp {
	CTcpServer::CTcpServer(const char *ip, const char *port, std::function<CITcpReceiver* (void)> proc, unsigned int timCON, unsigned int timRX, unsigned int timTX):
		m_ip(ip),
		m_port(port),
		m_proc(proc),
		mTimCON(timCON),
		mTimRX(timRX),
		mTimTX(timTX)
	{
		
	}
	
	void CTcpServer::run()
    {
        errcode = runProc();
    }
    
    TcpErrorCode CTcpServer::getErrorCode()
	{
		return errcode;
	}
	
	// muust be exec only once!
	TcpErrorCode CTcpServer::runProc()
	{	
		int ret;
		
		SNET_SOCKTYPE listenSocket = SNET_BADSOCKET;
		
		struct addrinfo hints, *servinfo, *p;
		
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
		
		if ((ret = getaddrinfo(m_ip, m_port, &hints, &servinfo)) != 0) 
		{
			fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
			return TcpErrorCode::FAILURE;
		}
		
		 // loop through all the results and bind to the first we can
		for(p = servinfo; p != NULL; p = p->ai_next) 
		{
			if ((listenSocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) 
			{
				perror("server: socket");
				continue;
			}

			int yes = 1;
			if (setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) 
			{
				perror("setsockopt");
				return TcpErrorCode::FAILURE;
			}

			if (bind(listenSocket, p->ai_addr, p->ai_addrlen) == -1) 
			{
				close(listenSocket);
				perror("server: bind");
				continue;
			}

			break;
		}

		if (p == NULL)  
		{
			fprintf(stderr, "server: failed to bind\n");
			return TcpErrorCode::FAILURE;
		}

		freeaddrinfo(servinfo); // all done with this structure
		
		/*
		 * The backlog argument defines the maximum length to which the queue of pending connections 
		 * for sockfd may  grow.   If  a  connection request  arrives  when  the queue is full, 
		 * the client may receive an error with an indication of ECONNREFUSED or, if the underlying
		 * protocol supports retransmission, the request may be ignored so that
		 * a later reattempt at connection succeeds.
		*/
		if (listen(listenSocket, CTCP_MAXCONNQUEUE) == -1) 
		{
			perror("listen");
			close(listenSocket);
			return TcpErrorCode::FAILURE;
		}
	
		socklen_t t;
		int acceptSocket;
		struct sockaddr_in remoteAddress;

		// int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);

		int tmperrno;

		// Set timeout on accept
        struct timeval tv;
        ms2tv(mTimCON, &tv);       
        setsockopt(listenSocket, SOL_SOCKET, SO_RCVTIMEO, (const void *) &tv, sizeof(struct timeval));

		while(1) // TODO: Termination through IPC or SIGTERM or by closing socket
		{
			t = sizeof(sockaddr_in);
			
			errno = 0;
			acceptSocket = accept(listenSocket, (struct sockaddr *)&remoteAddress, &t);
			tmperrno = errno;
			
			if (isTerminated())
			{
				close(listenSocket);
				printf("PipeServer terminating ....\n");
				manager.terminateAndJoin();
				return TcpErrorCode::TERMINATED;
			}
			
			int joined = manager.joinIfFinished();
			if (joined) printf("joined %d and remains %d\n", joined, manager.threadCount());	
			
			if (tmperrno == EAGAIN)
			{
				continue;
			}

			// TODO: if timeout continue;			
			if (acceptSocket == -1) 
			{
				perror("accept");
				close(listenSocket);
				// TODO: we should here terminate and join all threads here
				return TcpErrorCode::FAILURE;
			}
				
			unsigned long ipa = ((struct sockaddr_in) remoteAddress).sin_addr.s_addr;
				
			manager.startThread<CTcpChannel>([=](){return new CTcpChannel(acceptSocket, m_proc, ipa, mTimRX, mTimTX);});
		}
		
		return TcpErrorCode::SUCCESS;
	}
	
}} // ns

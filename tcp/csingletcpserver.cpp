
#include "csingletcpserver.hpp"

#define CTCP_MAXCONNQUEUE 5

namespace San2 { namespace Tcp {
	CSingleTcpServer::CSingleTcpServer(const std::string &ip, const std::string &port, unsigned int timCON, unsigned int timRX, unsigned int timTX):
		m_ip(ip),
		m_port(port),
		mTimCON(timCON),
		mTimRX(timRX),
		mTimTX(timTX),
		listenSocket(SNET_BADSOCKET),
		acceptSocket(SNET_BADSOCKET)
	{
		
	}
	
	void CSingleTcpServer::run()
    {
        errcode = runProc();
    }
    
    TcpErrorCode CSingleTcpServer::getErrorCode()
	{
		return errcode;
	}
	
	// muust be exec only once!
	TcpErrorCode CSingleTcpServer::runProc()
	{	
		int ret;
		
		struct addrinfo hints, *servinfo, *p;
		
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
		
		if ((ret = getaddrinfo(m_ip.c_str(), m_port.c_str(), &hints, &servinfo)) != 0) 
		{
			#ifdef WINDOWS
				DWORD gla = GetLastError();
				printf("GAI_GLA: %d\n", gla);
			#endif
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
        
        #ifdef LINUX
			if (setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) 
        #endif
        #ifdef WINDOWS
        	if (setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(int)) == -1) 
        #endif
			{
				perror("setsockopt");
				return TcpErrorCode::FAILURE;
			}

			if (bind(listenSocket, p->ai_addr, p->ai_addrlen) == -1) 
			{
				SNET_SOCKCLOSE(listenSocket);
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
            SNET_SOCKCLOSE(listenSocket);
			return TcpErrorCode::FAILURE;
		}
	

		// int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
		// Set timeout on accept
        
    #ifdef LINUX
        struct timeval tv;
        ms2tv(mTimCON, &tv);       
        setsockopt(listenSocket, SOL_SOCKET, SO_RCVTIMEO, (const void *) &tv, sizeof(struct timeval));
    #endif

    #ifdef WINDOWS
        DWORD dwTimAcc = mTimCON;
        setsockopt(listenSocket, SOL_SOCKET, SO_RCVTIMEO, (const char *) &dwTimAcc, sizeof(struct timeval));
    #endif
    
		socklen_t t = sizeof(sockaddr_in);
		
		
		// we want this to be really fault-tolerant
		
		// This will be used to implement node interface, the receiver part
		// it is important to make this fault-tolerant as the remote node
		// could temporarily disconnect and then reconnect
		while(!isTerminated())
		{
			acceptSocket = accept(listenSocket, (struct sockaddr *)&m_remoteAddress, &t);
			if (isTerminated())	break;
			if (acceptSocket == -1) continue; // fault-tolerant
			
				
			//unsigned long ipa = ((struct sockaddr_in) m_remoteAddress).sin_addr.s_addr;

			TcpErrorCode rc = receive();
			if (rc != TcpErrorCode::SUCCESS)
			{
				if (acceptSocket != SNET_BADSOCKET) SNET_SOCKCLOSE(acceptSocket);
				if (listenSocket != SNET_BADSOCKET) SNET_SOCKCLOSE(listenSocket);
				return rc;
			}
		}
		
		if (acceptSocket != SNET_BADSOCKET) SNET_SOCKCLOSE(acceptSocket);
		if (listenSocket != SNET_BADSOCKET) SNET_SOCKCLOSE(listenSocket);
		return TcpErrorCode::SUCCESS;
	}
	
	TcpErrorCode CSingleTcpServer::send(char *data, int len)
	{
		return sock_sendall_stream(acceptSocket, data, &len, mTimTX, this);
	}
	
	TcpErrorCode CSingleTcpServer::read(char *data, unsigned int dataSize, unsigned int *bytesRead)
	{
		return sock_read_stream(acceptSocket, this, data, dataSize, bytesRead, mTimRX);
	}
	
	struct sockaddr_in CSingleTcpServer::getRemoteAddress()
	{
		return m_remoteAddress;
	}
	
}} // ns

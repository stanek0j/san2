

#include "ctcpclient.hpp"

#define TCP_CTCPCLIENT_RXBUFSIZE 1500
#define TCP_CTCPCLIENT_WIN_MAXFNAMEBYTES 512

namespace San2 { namespace Tcp {	

    void CTcpClient::run()
    {
        errcode = runProc();
    }

	TcpErrorCode CTcpClient::getErrorCode()
	{
		return errcode;
	}
	
	CTcpClient::CTcpClient(const std::string &ip, const std::string &port, unsigned int timCON, unsigned int timRX, unsigned int timTX) :
		m_ip(ip),
		m_port(port),
		mTimCON(timCON),
		mTimRX(timRX),
		mTimTX(timTX),
        errcode(TcpErrorCode::SUCCESS),
        m_sock(SNET_BADSOCKET)
	{
		
	}

	CTcpClient::~CTcpClient()
	{
	    // empty
	}

    TcpErrorCode CTcpClient::runProc()
	{
	   TcpErrorCode rval = receive();    
       SNET_SOCKCLOSE(m_sock);
	   m_sock = SNET_BADSOCKET;
	   return rval;
	}

	// TODO: socket closing in open() and receive(), add cleanup() and destructor again
	TcpErrorCode CTcpClient::open()
	{
		m_sock = SNET_BADSOCKET;
		struct addrinfo hints, *servinfo, *p;
		int ret;

		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_INET; // force IPv4
		hints.ai_socktype = SOCK_STREAM;

		if ((ret = getaddrinfo(m_ip.c_str(), m_port.c_str(), &hints, &servinfo)) != 0) 
		{
			fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
			return TcpErrorCode::FAILURE;
		}

		// loop through all the results and connect to the first we can
		for(p = servinfo; p != NULL; p = p->ai_next) 
		{
			if ((m_sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) 
			{
				perror("client: socket");
				continue;
			}

			if (connect(m_sock, p->ai_addr, p->ai_addrlen) == -1) 
			{
				SNET_SOCKCLOSE(m_sock);
				perror("client: connect");
				continue;
			}

			break;
		}

		if (p == NULL) 
		{
			fprintf(stderr, "client: failed to connect\n");
			return TcpErrorCode::FAILURE;
		}

		// inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
		// printf("client: connecting to %s\n", s);

		freeaddrinfo(servinfo); // all done with this structure

		//close(m_sock);
		return TcpErrorCode::SUCCESS;
	}
	
	TcpErrorCode CTcpClient::send(char *data, int len)
	{			
		return sock_sendall_stream(m_sock, data, &len, mTimTX, this);
	}
	
	TcpErrorCode CTcpClient::read(char *data, unsigned int dataSize, unsigned int *bytesRead)
    {
        return sock_read_stream(m_sock, this, data, dataSize, bytesRead, mTimRX);
    }

}} // ns

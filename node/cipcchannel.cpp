
#include <string>
#include <string.h>

#include "cipcchannel.hpp"
#include "utils/log.h"
#include "utils/address.hpp"
#include "utils/cstringutils.hpp"
#include "utils/hex.hpp"

#define SAN2_CIPCCHANNEL_MAX_HOPCOUNT 65535

namespace San2 { namespace Node {

CIpcChannel::CIpcChannel(CPPL_PIPETYPE handle, unsigned int timRX, unsigned int timTX, CNode &node) :
	San2::Cppl::PipeChannel(handle, timRX, timTX),
	m_node(node)
{
	
}

CIpcChannel::~CIpcChannel()
{
	// empty
}

void CIpcChannel::printShell()
{
	San2::Cppl::BufferProcessor::send(m_node.getNodeName());
    San2::Cppl::BufferProcessor::send(">");
}

San2::Cppl::ErrorCode CIpcChannel::receive()
{	
	const unsigned int lineSize = 512;
	char line[lineSize];
	
	sendLine("Welcome to SAN node terminal");
	
	while (1)
	{		
		printShell();
			
		San2::Cppl::ErrorCode rval = readLine(line, lineSize);
		
		if (rval != San2::Cppl::ErrorCode::SUCCESS)
		{
			if (rval == San2::Cppl::ErrorCode::PEER_DISCONNECT)
			{
				// FILE_LOG(logDEBUG4) <<  "terminal peer disconnected";
				return rval;
			}
			FILE_LOG(logDEBUG4) <<  "CIpcChannel::receive():readLine(): failed errorcode: " << errorCodeToInt(rval);
			break;
		}
		
		if (strlen(line) == 0) continue;
	    
        std::string sline(line);
        
        std::vector<std::string> args;
        San2::Utils::CStringUtils::split(args, sline, 0x20); // space split

        if (args.size() < 1) continue;

        if (!args[0].compare("exit"))
        {
            San2::Cppl::BufferProcessor::sendLine("exit requested");
            return San2::Cppl::ErrorCode::REQUEST_DISCONNECT;
        }

        rval = lineParser(args);

        if (rval == San2::Cppl::ErrorCode::SUCCESS) continue;
        return rval;
	}
	
	return San2::Cppl::ErrorCode::SUCCESS;
}


San2::Cppl::ErrorCode CIpcChannel::lineParser(const std::vector<std::string> &args)
{
    if (!args[0].compare("send"))
    {
        std::shared_ptr<San2::Network::CCapsule> shCap(new San2::Network::CCapsule);
        *shCap = m_capsule;
        m_node.injectCapsule(shCap);
        
        CThread *thr = this;
        unsigned int timeou = 2000;

        m_node.injectCapsule(shCap, thr, timeou);
        San2::Cppl::BufferProcessor::sendLine("inject");
        return San2::Cppl::ErrorCode::SUCCESS;
    }

    if (!args[0].compare("capsule"))
    {
        if (args.size() < 2)
        {
            San2::Cppl::BufferProcessor::sendLine("usage: capsule <default|src|dst|hop|hash|hexdata|dx>");
            return San2::Cppl::ErrorCode::SUCCESS;
        }

        if (!args[1].compare("default"))
        {
             
        }

        if (!args[1].compare("src"))
        {
            switch(args.size())
            {
                case 2:
                    // print
                    San2::Cppl::BufferProcessor::sendLine(San2::Utils::address2string(m_capsule.getSourceAddress()).c_str());
                    return San2::Cppl::ErrorCode::SUCCESS;
                case 3:
                    // set
                    San2::Network::SanAddress srcAddress;
                    if (San2::Utils::string2address(args[2], srcAddress))
                    {   
                        m_capsule.setSourceAddress(srcAddress);
                        San2::Cppl::BufferProcessor::sendLine("source address set OK");
                    }
                    else
                    {
                        San2::Cppl::BufferProcessor::sendLine("setting source address failed - Wrong format?");
                        return San2::Cppl::ErrorCode::SUCCESS;
                    }

                    return San2::Cppl::ErrorCode::SUCCESS;

                default:
                    San2::Cppl::BufferProcessor::sendLine("usage: capsule src  |  capsule src <SanAddress>");
                    return San2::Cppl::ErrorCode::SUCCESS;
            }
        }

        if (!args[1].compare("dst"))
        {
            switch(args.size())
            {
                case 2:
                    // print
                    San2::Cppl::BufferProcessor::sendLine(San2::Utils::address2string(m_capsule.getDestinationAddress()).c_str());
                    return San2::Cppl::ErrorCode::SUCCESS;

                case 3:
                    // set
                    San2::Network::SanAddress dstAddress;
                    if (San2::Utils::string2address(args[2], dstAddress))
                    {   
                        m_capsule.setDestinationAddress(dstAddress);
                        San2::Cppl::BufferProcessor::sendLine("destination address set OK");
                    }
                    else
                    {
                        San2::Cppl::BufferProcessor::sendLine("setting destination address failed - Wrong format?");
                        return San2::Cppl::ErrorCode::SUCCESS;
                    }

                    return San2::Cppl::ErrorCode::SUCCESS;

                default:
                    San2::Cppl::BufferProcessor::sendLine("usage: capsule dst  |  capsule dst <SanAddress>");
                    return San2::Cppl::ErrorCode::SUCCESS;
            }
        }

        if (!args[1].compare("hop"))
        {
            switch(args.size())
            {
                case 2:
                    San2::Cppl::BufferProcessor::sendLine(San2::Utils::CStringUtils::uintToString(m_capsule.getHop()).c_str());
                    return San2::Cppl::ErrorCode::SUCCESS;
                case 3:
                    unsigned int ihop;
                    SAN_UINT16 hopCount;

                    if (!San2::Utils::CStringUtils::stringToUint(args[2], ihop))
                    {
                        San2::Cppl::BufferProcessor::sendLine("cannot parse hop count number");
                        return San2::Cppl::ErrorCode::SUCCESS;
                    }
                      
                    if (ihop > SAN2_CIPCCHANNEL_MAX_HOPCOUNT)
                    {
                        San2::Cppl::BufferProcessor::sendLine("failed: hop count must be less or equal than 65535");
                        return San2::Cppl::ErrorCode::SUCCESS;
                    }
                    
                    hopCount = ihop;

                    m_capsule.setHop(hopCount);

                    San2::Cppl::BufferProcessor::sendLine("hop count set OK");

                    return San2::Cppl::ErrorCode::SUCCESS;
                default:
                    San2::Cppl::BufferProcessor::sendLine("usage: capsule dst  |  capsule dst <SanAddress>");
                    return San2::Cppl::ErrorCode::SUCCESS;

            }
        }

        if (!args[1].compare("hash"))
        {
            switch(args.size())
            {
                case 2:
                    // print
                    San2::Cppl::BufferProcessor::sendLine(San2::Utils::appid2string(m_capsule.getAppId()).c_str());
                    return San2::Cppl::ErrorCode::SUCCESS;

                case 3:
                    // set
                    San2::Network::SanApplicationId appId;
                    if (San2::Utils::string2appid(args[2], appId))
                    {   
                        m_capsule.setApplicationId(appId);
                        San2::Cppl::BufferProcessor::sendLine("hash/applicationId set OK");
                    }
                    else
                    {
                        San2::Cppl::BufferProcessor::sendLine("setting hash/applicationId failed - Wrong format?");
                        return San2::Cppl::ErrorCode::SUCCESS;
                    }

                    return San2::Cppl::ErrorCode::SUCCESS;

                default:
                    San2::Cppl::BufferProcessor::sendLine("usage: capsule hash  |  capsule hash <SHA-1 hash in hex digits>");
                    return San2::Cppl::ErrorCode::SUCCESS;
            }
        }

        if (!args[1].compare("hexdata"))
        {
            San2::Utils::bytes data;
            std::string result;

            switch(args.size())
            {
                case 2:
                    // print
                    data = m_capsule.getData();
                    for(auto& s: data) result += San2::Utils::byteToString(s);
                    San2::Cppl::BufferProcessor::sendLine(result.c_str());
                    return San2::Cppl::ErrorCode::SUCCESS;

                case 3:
                    // set
                    
                    if (San2::Utils::hexToBytes(args[2], data))
                    {   
                        m_capsule.setData(data);
                        San2::Cppl::BufferProcessor::sendLine("data set OK");
                        return San2::Cppl::ErrorCode::SUCCESS;
                    }
                    else
                    {
                        San2::Cppl::BufferProcessor::sendLine("setting data failed - Wrong format?");
                        return San2::Cppl::ErrorCode::SUCCESS;
                    }

                    return San2::Cppl::ErrorCode::SUCCESS;

                default:
                    San2::Cppl::BufferProcessor::sendLine("usage: capsule hexdata  |  capsule hexdata <data in hex>");
                    return San2::Cppl::ErrorCode::SUCCESS;
            }
        }

        if (!args[1].compare("dx"))
        {
            switch(args.size())
            {
            case 2:
                if (m_capsule.getDX())
                {
                    San2::Cppl::BufferProcessor::sendLine("dx is on");
                }
                else
                {
                    San2::Cppl::BufferProcessor::sendLine("dx is off");
                }
                return San2::Cppl::ErrorCode::SUCCESS;
            case 3:
                if (!args[2].compare("on"))
                {
                    m_capsule.setDX(true);
                    San2::Cppl::BufferProcessor::sendLine("dx enabled OK");
                    return San2::Cppl::ErrorCode::SUCCESS;
                }

                if (!args[2].compare("off"))
                {
                    m_capsule.setDX(true);
                    San2::Cppl::BufferProcessor::sendLine("dx disabled OK");
                    return San2::Cppl::ErrorCode::SUCCESS;
                }

                San2::Cppl::BufferProcessor::sendLine("usage: capsule dx on | capsule dx off");
                return San2::Cppl::ErrorCode::SUCCESS;
            default:
                San2::Cppl::BufferProcessor::sendLine("usage: capsule dx on | capsule dx off | capsule dx");
                return San2::Cppl::ErrorCode::SUCCESS;
            }
        }

        return San2::Cppl::ErrorCode::SUCCESS;
    }

    if (!args[0].compare("help"))
    {
        San2::Cppl::BufferProcessor::sendLine("capsule - get/set capsule fields");
        San2::Cppl::BufferProcessor::sendLine("send    - send capsule with fileds previously set in the capsule command");
        San2::Cppl::BufferProcessor::sendLine("peers   - show interfaces addresses");
        San2::Cppl::BufferProcessor::sendLine("exit    - close terminal connection");
        return San2::Cppl::ErrorCode::SUCCESS;
    }

    San2::Cppl::BufferProcessor::sendLine("unknown command, type help for list of commands");

    return San2::Cppl::ErrorCode::SUCCESS;
}

}} // ns

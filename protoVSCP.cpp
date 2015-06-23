

#include "SctpConnection.hpp"
#include "M3UAmessage.hpp"
#include "TcapMessage.hpp"

#include "TCMessage.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define LOG(x) \
    std::cout << __FILE__ << "::" << __FUNCTION__ << ":" << __LINE__ << ": " << x << std::endl;

int main (int argc, char *argv[])
{

	int portNum = 2905;
	const char* remoteAdd = "192.168.1.154";

	SctpConnection sc;
	LOG ( "connecting to: " << remoteAdd << ", port: " << portNum  );

    if (sc.connectTo(remoteAdd, portNum) != 0)
        return 1;

    LOG ( "SCTP stream up! ..." );

    //TODO: M3UA implementation
    M3UAmessage m3uamsg;
    m3uamsg.aspUP(sc);

	LOG("ASP UP sent");


	m3uamsg.aspACTIVE(sc);


    fd_set rfds;
    int retval;
    unsigned char dummybuf[1024];

    while(true)
    {
    	LOG("Waiting for msgs...");
    	FD_ZERO(&rfds);
    	FD_SET(0, &rfds);
    	FD_SET(sc.getSocket(), &rfds);

    	retval = select(5, &rfds, NULL, NULL, NULL);

    	if (FD_ISSET(0,&rfds))
    	{
    		read(0, &dummybuf, 1024);
    	}
    	else
    	{
    		ByteStream incomingMsg;
    		int ret = sc.waitForMsg(incomingMsg);
    		if (ret < 1) break;

    		M3UAmessage incomingM3uaMsg;
    		if (incomingM3uaMsg.valid() == false)
    		{
    			LOG("incomingM3uaMsg is of UNKNOWN TYPE");
    			continue;
    		}

    		switch (incomingM3uaMsg.M3UAMsgType())
    		{
    		case M3UAmessage::ASPSM_ASPUP_ACK:
    			LOG("M3uaMsgType: ASPSM_ASPUP_ACK");

    		case M3UAmessage::ASPTM_ASPAC_ACK:
    			LOG("M3uaMsgType: ASPTM_ASPAC_ACK");

    		case M3UAmessage::MGMT_NTFY:
    			LOG("M3uaMsgType: MGMT_NTFY");

    		case M3UAmessage::TM_DATA:
    			LOG("M3uaMsgType: TM_DATA");

    		default:
    			LOG("M3uaMsgType : Unknown");
    			break;
    		}
    	}

    }


	//TODO: TCAP implementation



    //TODO: INAP implementation

    LOG ("program end")
    return 0;

}

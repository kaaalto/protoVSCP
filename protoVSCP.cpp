

#include "SctpConnection.hpp"
#include "M3UAmessage.hpp"
#include "TcapMessage.hpp"
#include "InapMessage.hpp"
#include "SccpMessage.hpp"


#include "TCMessage.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>





int main (int argc, char *argv[])
{

	int portNum = 2905;
	const char* remoteAdd = "192.168.1.154";

	// establish SCTP connection

	SctpConnection sc;
	LOG ( "connecting to: " << remoteAdd << ", port: " << portNum  );

    if (sc.connectTo(remoteAdd, portNum) != 0)
        return 1;

    LOG ( "SCTP stream up! ..." );

    // M3UA

    M3UAmessage m3uamsg;
    m3uamsg.aspUP(sc);

	LOG("ASP UP sent");

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

    		M3UAmessage incomingM3uaMsg(incomingMsg);
    		if (incomingM3uaMsg.valid() == false)
    		{
    			LOG("incomingM3uaMsg is of UNKNOWN TYPE");
    			continue;
    		}

    		switch (incomingM3uaMsg.M3UAMsgType())
    		{
    		case M3UAmessage::ASPSM_ASPUP_ACK:
    		{
    			LOG("M3uaMsgType: ASPSM_ASPUP_ACK");
    			m3uamsg.aspACTIVE(sc);
    			break;
    		}

    		case M3UAmessage::ASPTM_ASPAC_ACK:
    		{
    			LOG("M3uaMsgType: ASPTM_ASPAC_ACK");
    			break;
    		}
    		case M3UAmessage::MGMT_NTFY:
    		{
    			LOG("M3uaMsgType: MGMT_NTFY");
    			break;
    		}
    		case M3UAmessage::TM_DATA:
    		{
    			LOG("M3uaMsgType: TM_DATA");

    			ByteStream M3uaData;
    			incomingM3uaMsg.decodePayload();
    			M3uaData = incomingM3uaMsg.getPayload();

    			// SCCP

    			ByteStream SccpData;
    			SccpMessage incomingSccp(M3uaData);

    			if (incomingSccp.valid())
    			{
    				incomingSccp.decodeSccp();
    				LOG("SCCP decoded");
    				SccpData = incomingSccp.getData();
    				LOG("SccpData size: " << SccpData.size());



    			// TCAP

    			TcapMessage incomingTcap(SccpData);

    			// get data from TcapMessage
    			int localCode 					= incomingTcap.operationLocalCode();
    			int otid 						= incomingTcap.transactionId();
    			int invokeID 					= incomingTcap.invokeId();
    			ByteStream TcapParameterData 	= incomingTcap.parameterData();

    			LOG("localCode: " << localCode << " otid: " << otid << " invokeID: " << invokeID);

    			// INAP
    			InapMessage inapMsg(localCode, TcapParameterData);
    			string _calledPartyNum = inapMsg.getCalledPartyNumber();
    			LOG("CPN: " << _calledPartyNum);

    			// TODO searchDB(_calledPartyNum);


    			}

    			break;
    		}
    		default:
    		{
    			LOG("M3uaMsgType : UNKNOWN ");
    			break;
				}
    		}
    	}
    }

    LOG ("program end")
    return 0;

}

/**
 *   Prototype VSCP
 *   Copyright (C) 2012, 2013  Kalle Aalto <kalle.aalto@vmail.me>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "SccpMessage.hpp"
#include "Common.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdlib.h>


SccpMessage::SccpMessage() {
	return;
}

SccpMessage::SccpMessage(ByteStream &_incoming) : valChk(VALID)
{
	LOG("SccpMessage");


	if(_incoming[0] != 0x09)
	{
		LOG("ERROR  -  SCCP IS NOT UDT");
		valChk = INVALID;
		return;
	}


	LOG("SCCP valid");
	m_msg = _incoming;
	return;
}

void SccpMessage::decodeSccp()
{
	LOG("Decoding SCCP ...");
	LOG("SCCP msg size: " << m_msg.size());
	LOG("SCCP MESSAGE : " << m_msg);



	unsigned int calledPartyStart = m_msg[2] + 3 ;
	unsigned int callingPartyStart = m_msg[3] + 4 ;
	unsigned int dataStart = m_msg[4] + 5;

	LOG("calledPartyStart: " << calledPartyStart << " callingPartyStart: " << callingPartyStart << " dataStart: " << dataStart );

	/*
	  Parameter 			Clause 		Type (F V O) 		Length (octets)
	Message type 			2.1 		F 						1
	Protocol class 			3.6 		F 						1
	Called party address 	3.4 		V 					3 minimum								// will always be 9 bytes in NGNlab network
	Calling party address 	3.5 		V 					3 minimum 	(Note 2)					// will always be 11 bytes in NGNlab network? maybe?
	Data 					3.16 		V 					2-X 		(Note 1)

	NOTE 1 − Due to the ongoing studies on the SCCP called and calling party address, the maximum length
	of this parameter needs further study. It is also noted that the transfer of up to 255 octets of user data is
	allowed when the SCCP called and calling party address do not include global title.
	NOTE 2 − The minimum length = 2 might apply in the special case of AI = X0000000 described in 3.5.
	 */



	for(size_t i = calledPartyStart; i < callingPartyStart; i++)
	{
		calledPartyAddress.push_back(m_msg[i]);
	}

	for(size_t i = callingPartyStart; i < dataStart; i++)
	{
		callingPartyAddress.push_back(m_msg[i]);
	}

	for (size_t i = dataStart; i < m_msg.size(); i++)
	{
		payload.push_back(m_msg[i]);
	}

	LOG("calledPartyAddress size: " <<  calledPartyAddress.size());
	LOG("callingPartyAddress size: " <<  callingPartyAddress.size());
	LOG("payload size: " << payload.size());
//	payload.erase(payload.end() - 3 , payload.end());

	LOG("calledPartyAddress: " << calledPartyAddress);
	LOG("callingPartyAddress: " << callingPartyAddress);
	LOG("SccpPayload: " << payload);



}
// tuleva calledAdd -> lähtevä callingAdd
ByteStream SccpMessage::encodeSccp(ByteStream &tcapMsg,const ByteStream &calledAdd,const ByteStream &callingAdd)
{
	ByteStream msg;
	LOG("Encoding SCCP");

	/*
	  Parameter 			Clause 		Type (F V O) 		Length (octets)
	Message type 			2.1 		F 						1
	Protocol class 			3.6 		F 						1
	Called party address 	3.4 		V 					3 minimum								// will always be 9 bytes in NGNlab network
	Calling party address 	3.5 		V 					3 minimum 	(Note 2)					// will always be 11 bytes in NGNlab network?
	Data 					3.16 		V 					2-X 		(Note 1)

	NOTE 1 − Due to the ongoing studies on the SCCP called and calling party address, the maximum length
	of this parameter needs further study. It is also noted that the transfer of up to 255 octets of user data is
	allowed when the SCCP called and calling party address do not include global title.
	NOTE 2 − The minimum length = 2 might apply in the special case of AI = X0000000 described in 3.5.
	 */

	msg.push_back(0x09);  // message type UDT
	msg.push_back(0x00);  // protocol class Basic Connectionless



	unsigned int calledSize = calledAdd.size() - 1;			// ignore last byte
	unsigned int callingSize = callingAdd.size() -1 ;


	unsigned int callingPointer = calledAdd.size() + 2;
	unsigned int dataPointer = callingSize + callingPointer ;

	msg.push_back(0x03);	 			// pointer to called party address
	msg.push_back(callingPointer);	 	// pointer to calling party address
	msg.push_back(dataPointer);	 		// pointer to data	(TCAP)


	msg.push_back(calledSize);
	for(unsigned int i = 0; i < calledSize; i++)
	{
		msg.push_back(calledAdd[i]);
	}

	msg.push_back(callingSize);
	for(unsigned int i = 0; i < callingSize; i++)
	{
		msg.push_back(callingAdd[i]);
	}

	unsigned int tcSize = tcapMsg.size() ;
	msg.push_back(tcSize);
	for(unsigned int i = 0; i < tcSize ; i++)
	{
		msg.push_back(tcapMsg[i]);
	}

	LOG("tcSize: " << tcSize << " \ntcapMsg: " << tcapMsg);
	LOG("Outgoing SCCP message size: " << msg.size() );
	LOG("Outgoing SCCP: " << msg)

	return msg;
}

ByteStream SccpMessage::getData()
{
	return payload;
}
ByteStream SccpMessage::getCalledPartyAddress()
{
	return calledPartyAddress;
}
ByteStream SccpMessage::getCallingPartyAddress()
{
	return callingPartyAddress;
}

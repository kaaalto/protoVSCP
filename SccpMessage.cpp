#include "SccpMessage.hpp"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdlib.h>


#define LOG(x) \
    std::cout << __FILE__ << "::" << __FUNCTION__ << ":" << __LINE__ << ": " << x << std::endl;

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

	unsigned int offset = 2;

	unsigned int calledPartyStart = m_msg[2] + offset;
	unsigned int callingPartyStart = m_msg[3] + offset;
	unsigned int dataStart = m_msg[4] + offset;

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
}

// TODO encode SCCP

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

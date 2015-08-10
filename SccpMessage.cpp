#include "SccpMessage.hpp"
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

	if(_incoming[1] != 0x81 || _incoming[1] != 0x80)
	{
		LOG("ERROR  -  INVALID SCCP PROTOCOL CLASS ");	// must be 0x80 message handling
		valChk = INVALID;								// 0x01 sequenced connectionless or 0x00 basic connectionless
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

	unsigned int calledPartyStart = 2;
	unsigned int callingPartyStart = 10;
	unsigned int dataStart = callingPartyStart + 11;

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



	for(size_t i = calledPartyStart; i < callingPartyStart && i >= calledPartyStart; i++)
	{
		calledPartyAddress.push_back(i);
	}

	LOG("calledPartyAddress size: " <<  calledPartyAddress.size());

	for(size_t i = callingPartyStart; i >= calledPartyStart && i < dataStart; i++)
	{
		callingPartyAddress.push_back(m_msg[i]);
	}

	LOG("callingPartyAddress size: " <<  callingPartyAddress.size());

	for (size_t i = dataStart; i >= dataStart && i < m_msg.size(); i++)
	{
		payload.push_back(m_msg[i]);
	}

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


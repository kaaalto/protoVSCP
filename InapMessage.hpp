

#ifndef INAPMESSAGE_HPP_
#define INAPMESSAGE_HPP_


#include "TcapMessage.hpp"
#include "OCTET_STRING.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>

class InapMessage
{
private:
	std::string strCpn;
	void parseNum();
	OCTET_STRING_t encodeDta(const char*);
	long toHex(char*);

	std::string rawNum;

public:

	InapMessage();
	InapMessage(int _localCode, const ByteStream &);
	void decodeInitialDP(const ByteStream &);
	string getCalledPartyNumber();
	ByteStream encodeConnect(std::string);




};

#endif /* INAPMESSAGE_HPP_ */

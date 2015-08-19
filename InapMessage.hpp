

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

	std::string rawNum;

public:
	InapMessage();
	InapMessage(int _localCode, const ByteStream &);

	void decodeInitialDP(const ByteStream &);
	string getCalledPartyNumber();




};

#endif /* INAPMESSAGE_HPP_ */

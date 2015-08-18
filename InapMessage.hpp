

#ifndef INAPMESSAGE_HPP_
#define INAPMESSAGE_HPP_


#include "TcapMessage.hpp"
#include "OCTET_STRING.h"
#include <iostream>
#include <sstream>

#include <fstream>

class InapMessage
{
private:
	string str_cpn;
	void parseNum();

public:
	InapMessage();
	InapMessage(int _localCode, const ByteStream &);



	int decodeInitialDP(const ByteStream &);
	string getCalledPartyNumber();




};

#endif /* INAPMESSAGE_HPP_ */

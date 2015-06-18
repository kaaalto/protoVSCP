

#ifndef INAPMESSAGE_HPP_
#define INAPMESSAGE_HPP_


#include "TcapMessage.hpp"

class InapMessage
{

public:
	InapMessage();
	InapMessage(int _localCode, const ByteStream &);

	int decodeInitialDP(const ByteStream &);

};

#endif /* INAPMESSAGE_HPP_ */

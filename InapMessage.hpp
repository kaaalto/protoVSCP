

#ifndef INAPMESSAGE_HPP_
#define INAPMESSAGE_HPP_


#include "TcapMessage.hpp"
#include "OCTET_STRING.h"

class InapMessage
{
	private:
	const char* str_cpn;


public:
	InapMessage();
	InapMessage(int _localCode, const ByteStream &);

	int decodeInitialDP(const ByteStream &);
	const char getCalledPartyNumber();

//    friend ostream& operator<<( ostream &os, const InapMessage& i )
//    {
//    	os << " " << endl;
//    	return os;
//    }

};

#endif /* INAPMESSAGE_HPP_ */

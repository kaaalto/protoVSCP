

#ifndef SCCPMESSAGE_H_
#define SCCPMESSAGE_H_

#include "M3UAmessage.hpp"

class SccpMessage {
public:

	enum valChk {
		INVALID,
		VALID
	} valChk;

	SccpMessage();
	SccpMessage(ByteStream&);

	void decodeSccp();

	ByteStream getData();
	ByteStream getCallingPartyAddress();

	bool valid(void) { if (valChk == INVALID) { return false; } return true; }


private:
	ByteStream m_msg;
	ByteStream callingPartyAddress;
	ByteStream payload;


};

#endif /* SCCPMESSAGE_H_ */

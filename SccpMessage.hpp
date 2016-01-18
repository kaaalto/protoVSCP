

#ifndef SCCPMESSAGE_H_
#define SCCPMESSAGE_H_

#include "M3UAmessage.hpp"
#include "TcapMessage.hpp"

class SccpMessage {
public:

	enum valChk {
		INVALID,
		VALID
	} valChk;

	SccpMessage();
	SccpMessage(ByteStream&);

	void decodeSccp();
	ByteStream encodeSccp(ByteStream&, const ByteStream&, const ByteStream&);

	ByteStream getData();
	ByteStream getCalledPartyAddress();
	ByteStream getCallingPartyAddress();

	bool valid(void) { if (valChk == INVALID) { return false; } return true; }


private:
	ByteStream m_msg;
	ByteStream calledPartyAddress;
	ByteStream callingPartyAddress;
	ByteStream payload;


};

#endif /* SCCPMESSAGE_H_ */

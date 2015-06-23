

#ifndef M3UAMESSAGE_HPP_
#define M3UAMESSAGE_HPP_

#include "SctpConnection.hpp"


#define SCTP_PID_M3UA 3		// SCTP Payload Protocol Identifier as specified by IANA

using namespace std;

class M3UAmessage
{
public:

	enum M3UAMessages {
		UNKNOWN,
		MGMT_NTFY,
		ASPSM_ASPUP_ACK,
		ASPTM_ASPAC_ACK,
		TM_DATA
	} M3UAtype;

	M3UAmessage();
	M3UAmessage(ByteStream&);

	int aspUP(const SctpConnection &);
	int aspACTIVE (const SctpConnection &);

	static int getSctpPpid() { return SCTP_PID_M3UA; }

	bool valid(void) { if (M3UAtype == UNKNOWN) { return false; } return true; }
	M3UAMessages M3UAMsgType(void) { return M3UAtype; }


// TODO	ByteStream getDataPayload() const;



private:
	ByteStream commonHeader(unsigned char, unsigned char,unsigned int);
	ByteStream m_msg;
};

#endif /* M3UAMESSAGE_HPP_ */

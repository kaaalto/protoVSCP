/**
 *   Prototype VSCP
 *   Copyright (C) 2012, 2013  Kalle Aalto <kalle.aalto@vmail.me>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

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

	void decodePayload();
	ByteStream getPayload() const;
	ByteStream encodeMsg(const ByteStream&, const ByteStream&,
			const ByteStream&, const ByteStream&, const ByteStream&,
			const ByteStream&, const ByteStream&);

	ByteStream opc() const { return m_opc; }
	ByteStream dpc() const { return m_dpc; }
	ByteStream si() const { return m_si; }
	ByteStream ni() const { return m_ni; }
	ByteStream mp() const { return m_mp; }
	ByteStream sls() const { return m_sls; }



private:
	ByteStream commonHeader(unsigned char, unsigned char,unsigned int);
	ByteStream m_msg;
	ByteStream payload;

	ByteStream m_opc;
	ByteStream m_dpc;
	ByteStream m_si;
	ByteStream m_ni;
	ByteStream m_mp;
	ByteStream m_sls;

};

#endif /* M3UAMESSAGE_HPP_ */

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

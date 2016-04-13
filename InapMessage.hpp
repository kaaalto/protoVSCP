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

	std::string rawNum;

public:

	InapMessage();
	InapMessage(int _localCode, const ByteStream &);
	void decodeInitialDP(const ByteStream &);
	string getCalledPartyNumber();
	ByteStream encodeConnect(std::string);



};

#endif /* INAPMESSAGE_HPP_ */

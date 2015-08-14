/**
 *   SIGTRAN SMSC (Proof of concept)
 *   Copyright (C) 2012, 2013  Hannu Johansson <hannu@hjohansson.fi>
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
#ifndef __TCAP_MESSAGE_HPP
#define __TCAP_MESSAGE_HPP 1

#include "Common.h"

#include "M3UAmessage.hpp"

class TcapMessage {

public:
    TcapMessage();
    TcapMessage(ByteStream _incoming);
    ByteStream begin(const ByteStream &);
    ByteStream end(int invokeId, int dtid, const ByteStream &_resultData);

    int operationLocalCode() const;
    int transactionId() const { return m_transactionId; }
    int invokeId() const { return m_invokeId; }
    ByteStream parameterData() const;


private:
    int m_operationLocalCode;
    int m_transactionId;
    int m_invokeId;
    ByteStream m_parameterData;
};

#endif /* TCAPMESSAGE_HPP_ */

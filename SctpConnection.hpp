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
#ifndef _SCCPCONNECTION_HPP
#define _SCCPCONNECTION_HPP

#include <vector>

#include <stdint.h>

typedef std::vector<unsigned char> ByteStream;

class SctpConnection
{

  int m_socket;
public:
  SctpConnection();
  ~SctpConnection();

  int connectTo(const char*, int);
  int sendMsg(std::vector<unsigned char> _msg, unsigned char, uint16_t _stream_no = 0) const;
  int waitForMsg(ByteStream &);
  int getSocket() const { return m_socket; }


};

#endif // _SCCPCONNECTION_HPP

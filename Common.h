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
#ifndef COMMONH
#define COMMONH

#include <vector>
#include <iostream>
#include "constr_TYPE.h"


typedef std::vector<unsigned char> ByteStream;

std::ostream& operator<<(std::ostream& os, const ByteStream& b) ;


#define LOG(x) \
    std::cout << __FILE__ << "::" << __FUNCTION__ << ":" << std::dec << __LINE__ << ": " << x << std::endl;


#endif // COMMONH

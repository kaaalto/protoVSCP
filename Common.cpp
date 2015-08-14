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
#include "Common.h"

#include <iomanip>
#include <iostream>

std::ostream& operator<<(std::ostream& os, const ByteStream& b)
{

    for (unsigned int i=0; i<b.size(); i++)
    {
        if (i%8 == 0 && i != b.size()-1)
            std::cout << "\n";

        std::cout << "0x" << std::hex
                  << std::setfill('0')
                  << std::setw(2)
                  << (int) b[i] << " ";
    }


    return os;
}

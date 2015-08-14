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

///*
// * Configuration for every protocol layer:
// *
// */
//
//// MAP message / SM-RP-OA (SMSC +3580987654321) into mtForward (poc-smsc.cpp)
//const char mtForward_sm_rp_oa_smscnumber[]
//        = { 0x91, 0x53, 0x08, 0x89, 0x67, 0x45, 0x23, 0xf1 };
//
//// SUA message / SMSC GT address, used as source address in outgoing
////  - GT 3580987654321, this application (SMSC) GT address
//// SuaMessage.cpp
//unsigned const char sua_cldt_sourceAddr[] = {
//    0x00, 0x01, 0x00, 0x05, 0x80, 0x03,
//    0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x80, 0x01,
//    0x00, 0x13, 0x00, 0x00, 0x00, 0x04, 0x0d, 0x00,
//    0x01, 0x04, 0x53, 0x08, 0x89, 0x67, 0x45, 0x23,
//    0x01, 0x00 };
//
//// SUA message / VLR address where outgoing SMS message is pointed to
//// (should be copied from the incoming moForward MSG / source address)
////  - GT +3581234, VLR address
//// SuaMessage.cpp
//const unsigned char sua_cldt_destAddr[] = { 0x00, 0x01, 0x00, 0x05, 0x80, 0x03,
//                             0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x80, 0x01,
//                             0x00, 0x10, 0x00, 0x00, 0x00, 0x04, 0x07, 0x00,
//                             0x01, 0x04, 0x53, 0x18, 0x32, 0x04 };


typedef std::vector<unsigned char> ByteStream;

std::ostream& operator<<(std::ostream& os, const ByteStream& b) ;


#define LOG(x) \
    std::cout << __FILE__ << "::" << __FUNCTION__ << ":" << std::dec << __LINE__ << ": " << x << std::endl;


#endif // COMMONH

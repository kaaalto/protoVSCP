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

#include "SctpConnection.hpp"

#include <vector>

#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>

#define LOG(x) \
    std::cout << __FILE__ << "::" << __FUNCTION__ << ":" << __LINE__ << ": " << x << std::endl;

SctpConnection::SctpConnection() :
m_socket(0)
{
    LOG("<->")
}

SctpConnection::~SctpConnection()
{
    LOG("<->")

    if (m_socket > 0) {
        LOG("socket closed: " << m_socket)

        close(m_socket);
        m_socket = 0;
    }
}

int SctpConnection::connectTo(const char* _remote_ip, int _port)
{

    // Create socket, using kernel's sockets API, one-to-one style (not
    // one-to-many where socket type would be SOCK_SEQPACKET
    //
    m_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_SCTP);
    LOG("socket open " << m_socket)

    // Bind it to the local port
    // Local
    struct sockaddr_in sa_loc;
    memset(&sa_loc, 0, sizeof(struct sockaddr_in));
    sa_loc.sin_family = AF_INET;
    sa_loc.sin_port = htons(2905);
//    sa_loc.sin_addr.s_addr = inet_addr(LO);

    int rc = bind(m_socket, (struct sockaddr *)&sa_loc, sizeof(struct sockaddr));
    if (rc == -1) {
        LOG("bind() errno: " << errno)
        return 1;
    }

    // Stcp structs are defined in netinet/sctp.h, see man 7 sctp
    // - set initial parameters for the socket
    //
    struct sctp_initmsg s_init;
    memset(&s_init, 0, sizeof(s_init));
    s_init.sinit_num_ostreams = 5;
    s_init.sinit_max_instreams = 5;
    s_init.sinit_max_attempts = 5;

    rc = setsockopt(m_socket,
                        IPPROTO_SCTP,
                        SCTP_INITMSG,
                        &s_init, sizeof(s_init));
    if (rc == -1) {
        LOG("setsockopt() errno: " << errno)
        return 1;
    }

    // Set the target addr and connect
    //
    struct sockaddr_in remote;
    memset(&remote, 0, sizeof(remote));
    remote.sin_family = AF_INET;
    remote.sin_port = htons(_port); // M3UA server default
    remote.sin_addr.s_addr = inet_addr(_remote_ip);

    rc = connect(m_socket, (struct sockaddr*) &remote, sizeof(remote));
    if (rc != 0) {
        LOG("connect() errno: " << errno)
        return 2;
    }

    return 0;
}


int SctpConnection::sendMsg(std::vector<unsigned char> _msg ,
                            unsigned char _ppid,
                            uint16_t _stream_no) const
{
    unsigned char buf[1024];

    for (unsigned int i=0;i<_msg.size(); i++)
        buf[i] = _msg[i];

    LOG("sctp_sendmsg() trying to send " << _msg.size()
        << "bytes with PPID " << (unsigned int) _ppid << ", SID " << _stream_no)

    int rc = sctp_sendmsg(m_socket,
                          (void *) buf, _msg.size(), // Buffer to be sent
                          NULL, 0, 					 // to sockaddr
                          htonl(_ppid), 0, 			 // pid + flags
                          _stream_no, 0, 0);		 // stream + timetolive + context

    if (rc == -1) {
        LOG("sctp_sendmsg() error: " << rc)
        return 1;
    } else
        LOG("sctp_sendmsg() done, bytes sent: " << rc)


    return 0;
}

int SctpConnection::waitForMsg(ByteStream &_incoming)
{
    LOG("enter <-")
    //      int sctp_recvmsg(int sd, void * msg, size_t len,
    //                  struct sockaddr * from, socklen_t * fromlen,
    //                  struct sctp_sndrcvinfo * sinfo, int * msg_flags);

    unsigned char buf[1024];
    int msg_flags = 0;
    struct sctp_sndrcvinfo sinfo;

    int rc = sctp_recvmsg(m_socket, &buf, sizeof(buf),
                 NULL, NULL,
                 &sinfo, &msg_flags);
    if (rc < 0) {
        LOG("sctp_recvmsg() ERROR rc=" << rc)
        return rc;
    } else
        LOG("sctp_recvmsg() returned " << rc << " bytes")

    for (int i=0; i < rc; i++)
        _incoming.push_back(buf[i]);


    LOG("done ->")
    return rc;
}



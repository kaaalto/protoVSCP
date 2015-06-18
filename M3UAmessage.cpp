

#include "M3UAmessage.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdlib.h>



#define LOG(x) \
    std::cout << __FILE__ << "::" << __FUNCTION__ << ":" << __LINE__ << ": " << x << std::endl;

M3UAmessage::M3UAmessage() : M3UAtype(UNKNOWN)
{
	return;
}


M3UAmessage::M3UAmessage(ByteStream &_incoming) : M3UAtype(UNKNOWN)
{

	unsigned char msgType, msgClass;

	LOG("Incoming M3UA message: " << _incoming.size() );


	if (_incoming.size() < 8)
	{
		LOG ("M3UA - message too short");
		return;
	}
	if (_incoming[0] != 0x01 || _incoming[0] != 0x00)
	{
		LOG("M3UA - Invalid protocol version");
		return;
	}

	msgClass = _incoming[2];
	msgType  = _incoming[3];


	switch(msgClass)
	{
	case 0:			// MGMT
		if (msgType == 1) M3UAtype = MGMT_NTFY;
		break;
	case 3:			// ASPSM
		if (msgType == 4) M3UAtype = ASPSM_ASPUP_ACK;
		break;
	case 4:			// ASPTM
		if (msgType == 3) M3UAtype = ASPTM_ASPAC_ACK;
		break;
	case 1:
		if (msgType == 1) M3UAtype = TM_DATA;

	default:
		break;
	}
	if (M3UAtype != UNKNOWN)
	{
		LOG("M3UAType : " << M3UAtype);
		return;
	}
	else {
		LOG("Unsupported M3UAtype");
		return;
	}



		// wot??

    int messageLength = 0;
    messageLength |= (_incoming[4] & 0xff000000);
    messageLength |= (_incoming[5] & 0x00ff0000);
    messageLength |= (_incoming[6] & 0x0000ff00);
    messageLength |= (_incoming[7] & 0x000000ff);

    LOG("M3UA message length: " << messageLength);
    m_msg = _incoming;

    return;

}


int M3UAmessage::aspUP (const SctpConnection &_sctpConn)
{

/*
	3.5.1. ASP Up

	      The ASP Up message is used to indicate to a remote M3UA peer that
	      the adaptation layer is ready to receive any ASPSM/ASPTM messages
	      for all Routing Keys that the ASP is configured to serve.

	      The ASP Up message contains the following parameters:

	         ASP Identifier                Optional
	         INFO String                   Optional

	      The format for ASP Up message parameters is as follows:

	        0                   1                   2                   3
	        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	       |         Tag = 0x0011          |           Length = 8          |
	       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	       |                         ASP Identifier                        |
	       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	       |         Tag = 0x0004          |             Length            |
	       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	       \                                                               \
	       /                          INFO String                          /
	       \                                                               \
	       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	   ASP Identifier: 32-bit unsigned integer

	      The optional ASP Identifier parameter contains a unique value that
	      is locally significant among the ASPs that support an AS.  The SGP
	      should save the ASP Identifier to be used, if necessary, with the
	      Notify message (see Section 3.8.2).

	      The format and description of the optional INFO String parameter
	      are the same as for the DUNA message (see Section 3.4.1).
*/

	std::vector <unsigned char> msg;

	int tlvLength = 0;

	msg = commonHeader(0x03, 0x01, tlvLength);
	LOG("sending ASP UP")
	_sctpConn.sendMsg(msg, SCTP_PID_M3UA, 0); // data, protocol ID, stream ID


	return 0;
}


int M3UAmessage::aspACTIVE(const SctpConnection &_scptConn)
{
	std::vector <unsigned char> msg;
	int tlvLength = 0;

/*
 * 3.7.1. ASP Active


   The ASP Active message is sent by an ASP to indicate to a remote M3UA
   peer that it is ready to process signalling traffic for a particular
   Application Server.  The ASP Active message affects only the ASP
   state for the Routing Keys identified by the Routing Contexts, if
   present.

   The ASP Active message contains the following parameters:

      Traffic Mode Type     Optional
      Routing Context       Optional
      INFO String           Optional

   The format for the ASP Active message is as follows:

       0                   1                   2                   3
       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |         Tag = 0x000b          |          Length = 8           |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                      Traffic Mode Type                        |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |         Tag = 0x0006          |            Length             |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      \                                                               \
      /                       Routing Context                         /
      \                                                               \
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |         Tag = 0x0004          |             Length            |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      \                                                               \
      /                          INFO String                          /
      \                                                               \
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	 */


	msg.push_back(0x00);
	msg.push_back(0x0B);	// tag
	msg.push_back(0x00);
	msg.push_back(0x08);	// length


	/*

   Traffic Mode Type: 32-bit (unsigned integer)

      The Traffic Mode Type parameter identifies the traffic mode of
      operation of the ASP within an AS.  The valid values for Traffic
      Mode Type are shown in the following table:

         1         Override
         2         Loadshare
         3         Broadcast

      Within a particular Routing Context, Override, Loadshare, and
      Broadcast SHOULD NOT be mixed.  The Override value indicates that
      the ASP is operating in Override mode, in which the ASP takes over
      all traffic in an Application Server (i.e., primary/backup
      operation), overriding any currently active ASPs in the AS.  In
      Loadshare mode, the ASP will share in the traffic distribution
      with any other currently active ASPs.  In Broadcast mode, the ASP
      will receive the same messages as any other currently active ASP.
      */

	msg.push_back(0x00);
	msg.push_back(0x00);
	msg.push_back(0x00);
	msg.push_back(0x02);	// Loadshare


	 /*
   Routing Context: n X 32-bit integers

      The optional Routing Context parameter contains (a list of)
      integers indexing the Application Server traffic that the sending
      ASP is configured/registered to receive.

      There is a one-to-one relationship between an index entry and an
      SGP Routing Key or AS Name.  Because an AS can only appear in one
      Network Appearance, the Network Appearance parameter is not
      required in the ASP Active message.

      An Application Server Process may be configured to process traffic
      for more than one logical Application Server.  From the
      perspective of an ASP, a Routing Context defines a range of
      signalling traffic that the ASP is currently configured to receive
      from the SGP.  For example, an ASP could be configured to support
      signalling for multiple MTP3-Users, identified by separate SS7
      DPC/OPC/SI ranges.

   The format and description of the optional INFO String parameter are
   the same as for the DUNA message (see Section 3.4.1).
 *
 */
	msg.push_back(0x00);
	msg.push_back(0x00);
	msg.push_back(0x00);
	msg.push_back(0x02);	// Routing key 2

	tlvLength = 8;

	ByteStream bsHeader = commonHeader(0x04, 0x01, tlvLength);	// ASPTM, ASPAC
	msg.insert( msg.begin(), bsHeader.begin(), bsHeader.end() );
	LOG("Sending ASP Active");
	_scptConn.sendMsg(msg, SCTP_PID_M3UA, 0); // data, protocol ID, stream ID

	//LOG("msg: "<< msg );
	LOG("SCTP_PID: "<< SCTP_PID_M3UA);


	return 0;

}

ByteStream M3UAmessage::commonHeader(unsigned char _msgClass, unsigned char _msgType, unsigned int _tlvLength)
{

	ByteStream bs;

/*	///////////////////////////////////////////////////////////////////////////////////
	3.1 Common Message Header

	   The protocol messages for MTP3-User Adaptation require a message
	   header which contains the adaptation layer version, the message type,
	   and message length.

	      0                   1                   2                   3
	      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	      |    Version    |   Reserved    | Message Class | Message Type  |
	      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	      |                        Message Length                         |
	      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	      \                                                               \
	      /                                                               /

	   All fields in an M3UA message MUST be transmitted in the network byte
	   order, unless otherwise stated.

	3.1.1 M3UA Protocol Version: 8 bits (unsigned integer)

	   The version field contains the version of the M3UA adaptation layer.

	   The supported versions are the following:

	         1      Release 1.0

*//////////////////////////////////////////////////////////////////////////////////////////

	bs.push_back(0x01);			// 0th byte, version
	bs.push_back(0x00);			// 1st byte, reserved, must be set to 0

/*/////////////////////////////////////////////////////////////////////////////////////////

	3.1.2  Message Classes and Types

	   The following list contains the valid Message Classes:

	   Message Class: 8 bits (unsigned integer)

	   The following list contains the valid Message Type Classes:

	        0     Management (MGMT) Messages
	        1     Transfer Messages
	        3     ASP State Maintenance (ASPSM) Messages
	        4     ASP Traffic Maintenance (ASPTM) Messages
	        5     Reserved for Other Sigtran Adaptation Layers
	        6     Reserved for Other Sigtran Adaptation Layers
	        7     Reserved for Other Sigtran Adaptation Layers
	        8     Reserved for Other Sigtran Adaptation Layers
	        9     Routing Key Management (RKM) Messages
	       10 to 127 Reserved by the IETF
	      128 to 255 Reserved for IETF-Defined Message Class extensions
	      Message Type: 8 bits (unsigned integer)

*//////////////////////////////////////////////////////////////////////////////////////////

	bs.push_back(_msgClass);		// 2nd byte, class

/*/////////////////////////////////////////////////////////////////////////////////////////

	           The following list contains the message types for the defined
	           messages.

	           Management (MGMT) Messages (See Section 3.8)

	             0        Error (ERR)
	             1        Notify (NTFY)
	          2 to 127    Reserved by the IETF
	        128 to 255    Reserved for IETF-Defined MGMT extensions

	           Transfer Messages (See Section 3.3)

	             0        Reserved
	             1        Payload Data (DATA)
	          2 to 127    Reserved by the IETF
	        128 to 255    Reserved for IETF-Defined Transfer extensions

	           SS7 Signalling Network Management (SSNM) Messages (See Section
	           3.4)

	             0        Reserved
	             1        Destination Unavailable (DUNA)
	             2        Destination Available (DAVA)
	             3        Destination State Audit (DAUD)
	             4        Signalling Congestion (SCON)
	             5        Destination User Part Unavailable (DUPU)
	             6        Destination Restricted (DRST)
	          7 to 127    Reserved by the IETF
	        128 to 255    Reserved for IETF-Defined SSNM extensions

	           ASP State Maintenance (ASPSM) Messages (See Section 3.5)

	             0        Reserved
	             1        ASP Up (ASPUP)
	             2        ASP Down (ASPDN)
	             3        Heartbeat (BEAT)
	             4        ASP Up Acknowledgement (ASPUP ACK)
	             5        ASP Down Acknowledgement (ASPDN ACK)
	             6        Heartbeat Acknowledgement (BEAT ACK)
	          7 to 127    Reserved by the IETF
	        128 to 255    Reserved for IETF-Defined ASPSM extensions

	        ASP Traffic Maintenance (ASPTM) Messages (See Section 3.7)

	              0        Reserved
	              1        ASP Active (ASPAC)
	              2        ASP Inactive (ASPIA)
	              3        ASP Active Acknowledgement (ASPAC ACK)
	              4        ASP Inactive Acknowledgement (ASPIA ACK)
	           5 to 127    Reserved by the IETF
	         128 to 255    Reserved for IETF-Defined ASPTM extensions

	            Routing Key Management (RKM) Messages (See Section 3.6)

	              0        Reserved
	              1        Registration Request (REG REQ)
	              2        Registration Response (REG RSP)
	              3        Deregistration Request (DEREG REQ)
	              4        Deregistration Response (DEREG RSP)
	           5 to 127    Reserved by the IETF
	         128 to 255    Reserved for IETF-Defined RKM extensions
*//////////////////////////////////////////////////////////////////////////////////////////


	bs.push_back(_msgType);		// 2rd byte, type



/*/////////////////////////////////////////////////////////////////////////////////////////

	      3.1.4  Message Length: 32-bits (unsigned integer)

	         The Message Length defines the length of the message in octets,
	         including the Common Header.  The Message Length MUST include
	         parameter padding bytes, if any.

	         Note: A receiver SHOULD accept the message whether or not the final
	         parameter padding is included in the message length.

*//////////////////////////////////////////////////////////////////////////////////////////


	// wot??

	unsigned int totLength = _tlvLength + 8;

	 bs.push_back((totLength >> 24) & 0xff);
	 bs.push_back((totLength >> 16) & 0xff);
	 bs.push_back((totLength >> 8) & 0xff);
	 bs.push_back(totLength & 0xff);

	return bs;

}
/*
 *
 * 3.3.1.  Payload Data Message (DATA)

   The DATA message contains the SS7 MTP3-User protocol data, which is
   an MTP-TRANSFER primitive, including the complete MTP3 Routing Label.
   The DATA message contains the following variable-length parameters:

        Network Appearance       Optional
        Routing Context          Conditional
        Protocol Data            Mandatory
        Correlation Id           Optional

   The following format MUST be used for the Data Message:

       0                   1                   2                   3
       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |        Tag = 0x0200           |          Length = 8           |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                       Network Appearance                      |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |        Tag = 0x0006           |          Length = 8           |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                        Routing Context                        |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |        Tag = 0x0210           |             Length            |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      \                                                               \
      /                        Protocol Data                          /
      \                                                               \
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |        Tag = 0x0013           |          Length = 8           |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                        Correlation Id                         |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

   Network Appearance: 32 bits (unsigned integer)

      The Network Appearance parameter identifies the SS7 network
      context for the message and implicitly identifies the SS7 Point
      Code format used, the SS7 Network Indicator value, and the MTP3
      and possibly the MTP3-User protocol type/variant/version used
      within the specific SS7 network.  Where an SG operates in the
      context of a single SS7 network, or if individual SCTP
      associations are dedicated to each SS7 network context, the
      Network Appearance parameter is not required.  In other cases, the
      parameter may be configured to be present for the use of the
      receiver.

      The Network Appearance parameter value is of local significance
      only, coordinated between the SGP and ASP.  Therefore, in the case
      where an ASP is connected to more than one SGP, the same SS7
      network context may be identified by different Network Appearance
      values, depending on which SGP a message is being transmitted/
      received.

      Where the optional Network Appearance parameter is present, it
      MUST be the first parameter in the message, as it defines the
      format of the Protocol Data field.

      IMPLEMENTATION NOTE: For simplicity of configuration, it may be
      desirable to use the same NA value across all nodes sharing a
      particular network context.

   Routing Context: 32 bits (unsigned integer)

      The Routing Context parameter contains the Routing Context value
      associated with the DATA message.  Where a Routing Key has not
      been coordinated between the SGP and ASP, sending of Routing
      Context is not required.  Where multiple Routing Keys and Routing
      Contexts are used across a common association, the Routing Context
      MUST be sent to identify the traffic flow, assisting in the
      internal distribution of Data messages.

   Protocol Data: variable length

      The Protocol Data parameter contains the original SS7 MTP3
      message, including the Service Information Octet and Routing
      Label.
 *
 *
 */



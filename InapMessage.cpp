#include "InapMessage.hpp"

#include "ConnectArg.h"
#include "InitialDPArg.h"
#include "ReleaseCallArg.h"
#include "CutAndPaste.h"
#include "Common.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>

/*
 * This is a custom function which writes the
 * encoded output into some FILE stream.
 * http://lionet.info/asn1c/asn1c-usage.html
 */
static int
write_out(const void *buffer, size_t size, void *app_key) {

    ByteStream *tmpbuf = static_cast<ByteStream*>(app_key);

    for (unsigned int i=0; i<size; i++) {
        unsigned char *byte = (unsigned char*) buffer;
        tmpbuf->push_back(*(byte+i));
    }

    return 0;
}
InapMessage::InapMessage()
{
	return;
}
InapMessage::InapMessage(int _localCode, const ByteStream &_incoming)
{
		LOG("INAP message - size: " << _incoming.size() );
//		LOG(" " << _incoming);
		LOG("INAP localCode: " << _localCode);

		switch(_localCode)
		{
		case 0:
			{
				// InitialDPArg
				LOG("InitialDPArg received");
				decodeInitialDP(_incoming);
				break;
			}
		case 20:
			{
				// ConnectArg
				LOG("ConnectArg received");
				break;
			}
		case 22:
		{
			// ReleaseCall
			LOG("ReleaseCall");
			break;
		}
		default:
			{
			LOG("ERROR  -  unknown localCode: " << _localCode);
			break;
			}
		}
}



void InapMessage::decodeInitialDP(const ByteStream &_msg)
{
	LOG("decoding InitialDPArg");

	InitialDPArg_t *msg = 0;
	asn_dec_rval_t rval;

	// BER DECODER
    rval = asn_DEF_InitialDPArg.ber_decoder(0,
          &asn_DEF_InitialDPArg,
          (void **)&msg,
          &_msg[0],
          _msg.size(),
          0);

    if(rval.code != RC_OK) {
    	LOG("InitialDPArg decoding failed, code: " << rval.code << ", consumed bytes: " << rval.consumed );

    	        /* Free partially decoded struct */
    	        asn_DEF_InitialDPArg.free_struct(
    	            &asn_DEF_InitialDPArg, msg, 0);
    	        return;
    } else {
    	LOG("InitialDPArg decoding successful");
    }

    asn_fprint(stdout, &asn_DEF_InitialDPArg, msg);

    if(!msg->calledPartyNumber)
    {
    	LOG("calledPartyNumber not found");
    	asn_DEF_InitialDPArg.free_struct(&asn_DEF_InitialDPArg, msg, 0);
    	return;
    }else{

		FILE *fp;
		fp = fopen("data.txt", "w");
		if(fp == NULL) perror("Error opening file");
		else
		{
			asn_fprint(fp, &asn_DEF_InitialDPArg, msg);

		}
		fclose(fp);
		asn_DEF_InitialDPArg.free_struct(&asn_DEF_InitialDPArg, msg, 0);
		parseNum();
    }

   return;
}


void InapMessage::parseNum()
{
	LOG("ParseNum()");
	std::string line;
	bool isOdd = false;
	std::ifstream is;

	is.open("data.txt");

	if(is.is_open())
	{

		while (is.good())
		{

			std::string numl = "calledPartyNumber:";
			std::string tmp;

			getline(is, tmp);
			if(tmp.find(numl) != std::string::npos )
			{
				LOG("CalledPartyNumber found");
				line = tmp;
			}
		}

		is.close();

	} else {
		LOG("FILE NOT OPEN");
		return;
	}


	 if( remove( "data.txt" ) != 0 )
	 {
	    LOG( "Error deleting file" );
	 }
	 else{
	    LOG( "File successfully deleted" );
	 }

	 if(!line.empty())
	 {

		rawNum = line;
		size_t pos = (line.find(":") + 2);

		if(line[pos] >= 8) isOdd = true;

		line.erase(std::remove_if(line.begin(), line.end(), (int(*)(int))isspace), line.end());

		line = line.substr(pos - 1);

		for(unsigned int i = 0; i < line.size(); i++)
		{
			std::swap(line[1], line[0]);
			if(i % 2 == 0)
			{
				std::swap(line[i+1], line[i]);
			}
		}
		if((isOdd = true)) line.erase(line.size() - 1);

		strCpn = line;

	 } else{
		 LOG("ERROR  -  CALLEDPARTYNUMBER NOT FOUND");
	 }
	 return;
}

string InapMessage::getCalledPartyNumber()
{
	LOG("getCalledPartyNumber()");
	return strCpn;
}

ByteStream InapMessage::encodeConnect (std::string addr)
{
	ByteStream bs;
	ConnectArg_t *inapMsg;
	asn_enc_rval_t ec;
	CalledPartyNumber_t *oct ;
	CutAndPaste_t *cap = 0;

	LOG("Encoding ConnectArg...");
	LOG("addr " << addr);

	inapMsg = (ConnectArg_t*) calloc(1, sizeof(ConnectArg_t));
	oct = (CalledPartyNumber_t*) calloc(1, sizeof (CalledPartyNumber_t));
	cap = (CutAndPaste_t*) calloc(1, sizeof(CutAndPaste_t));

//	DestinationRoutingAddress_t *dta = (DestinationRoutingAddress_t*) calloc(1, sizeof(DestinationRoutingAddress_t));

	const char * c = addr.c_str();

	// encode according to ITU-T recommendation Q.763 BCDKSI


	OCTET_STRING_t  dtaStr = encodeDta(c);

//	oct->buf = c;
//	oct->size = sizeof(c);

//	OCTET_STRING_fromString(oct, dtaStr);
//	ASN_SEQUENCE_ADD(dta, oct);

//	OCTET_STRING_t * str = OCTET_STRING_new_fromBuf(&asn_DEF_OCTET_STRING, c, strlen(c));
//	asn_fprint(stdout, &asn_DEF_OCTET_STRING, str);
//	LOG("dta size " << sizeof(dta) << " " << dta);

//	asn_fprint(stdout, &asn_DEF_DestinationRoutingAddress, dta);

	//asn_fprint(stdout, &asn_DEF_OCTET_STRING, &dtaStr);

//	ASN_SEQUENCE_ADD(&inapMsg->destinationRoutingAddress, dta);


	ASN_SEQUENCE_ADD(&inapMsg->destinationRoutingAddress.list, &dtaStr);
//	&inapMsg->destinationRoutingAddress.list = c;

	inapMsg->cutAndPaste = cap;

    ec = der_encode(&asn_DEF_ConnectArg,
                    inapMsg, write_out, &bs);

    if(ec.encoded == -1)
    {
    	LOG("ERROR - ENCODING FAILED: " << ec.failed_type);
    }
    else{
    	LOG ("der_encode.encoded=" << (int) ec.encoded );
    }

    asn_fprint(stdout, &asn_DEF_ConnectArg, inapMsg);
    return bs;

}

	// ITU-T Q.763
OCTET_STRING_t InapMessage::encodeDta(const char * str)
{
	OCTET_STRING_t ret;
	bool isOdd;
	int size = 2;

	if (sizeof(str) % 2 == 0) isOdd = true;

	if( isOdd ){ ret.buf[0] = 0x02;}
	else {ret.buf[0] = 0x82;}
	ret.buf[1] = 0x10;


	int x = 1;


	for(unsigned int i = 0; i < sizeof(str) +1; i = i+2)
	{
		char *strHex = (char*)malloc(2) ;
		x++;
		size = size + 2;

		strHex[1] = str[i];

		if(str[i+1] == ' ') {
			strHex[0] == 0;
		}
		else {

		strHex[0] = str[i+1];
		}

		LOG("strHex" << strHex);
		ret.buf[x] = toHex(strHex);

	//	if(i % 2 == 0){
	//		ret.buf[i+2] = toHex(str[i+1]);
	//	}
	//	else {ret.buf[i+2] = toHex(str[i]);};
	}


	ret.size = size;
	LOG("RET:");
	asn_fprint(stdout, &asn_DEF_OCTET_STRING, &ret);
	LOG("RET SIZE: " << ret.size)

	return ret;

}
long InapMessage::toHex(char *s)
{
	LOG("char: " << s);
	long h;

////	std::string copy = std::string("0x") + (std::string)s;
//	const char *copyS = s;
//	h = (uint8_t)atoi(copyS);

	h = strtol (s,NULL,16);

	LOG("h: " << h);

	return h;

}
// TODO InapMessage::encodeRelease(...)

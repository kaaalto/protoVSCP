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
          _msg[0],
          0);

    if(rval.code != RC_OK) {
    	LOG("InitialDPArg decoding failed, code: " << rval.code << ", consumed bytes: " << rval.consumed );

    	        /* Free partially decoded rect */
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
    }

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

   return;
}


void InapMessage::parseNum()
{
	std::string line;
	std::ifstream is("data.txt");
	bool isOdd = false;

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
	asn_dec_rval_t ec;





    ec = der_encode(&asn_DEF_ConnectArg,
                    inapMsg, write_out, &bs);

    LOG ("der_encode.encoded=" << (int) ec.encoded );
    asn_fprint(stdout, &asn_DEF_MT_ForwardSM_Arg, mapMsg);





		//cutAndPaste: 0

	return bs;
}





// TODO InapMessage::encodeRelease(...)



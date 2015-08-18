#include "InapMessage.hpp"

#include "ConnectArg.h"
#include "InitialDPArg.h"
#include "ReleaseCallArg.h"
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
//static int
//write_out(const void *buffer, size_t size, void *app_key) {
//
//    ByteStream *tmpbuf = static_cast<ByteStream*>(app_key);
//
//    for (unsigned int i=0; i<size; i++) {
//        unsigned char *byte = (unsigned char*) buffer;
//        tmpbuf->push_back(*(byte+i));
//    }
//
//    return 0;
//}

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



int InapMessage::decodeInitialDP(const ByteStream &_msg)
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
    	        return -1;
    } else {
    	LOG("InitialDPArg decoding successful");
    }

    asn_fprint(stdout, &asn_DEF_InitialDPArg, msg);

    if(!msg->calledPartyNumber)
    {
    	LOG("calledPartyNumber not found");
    	asn_DEF_InitialDPArg.free_struct(&asn_DEF_InitialDPArg, msg, 0);
    	return -1;
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



   return 0;
}


void InapMessage::parseNum()
{
	std::string line;
	std::string tmp;
	std::string numl = "calledPartyNumber:";
	std::ifstream is("data.txt");

	if(is.is_open())
	{

		while (is.good())
		{

			getline(is, tmp);
			if(tmp.find(numl) != std::string::npos )
			{
				LOG("found line");
				size_t pos = 0, prev_pos = 0;
				std::string delim = ":";

				line = tmp;

			}else LOG("CALLEDPARTYADDRESS NOT FOUND");
		}

	} else {
		LOG("FILE NOT OPEN");
		return;
	}


	 if( remove( "data.txt" ) != 0 )				// ei mee tänne
	 {
	    LOG( "Error deleting file" );
	 }
	 else{
	    LOG( "File successfully deleted" );
	 }

	 if(!line.empty())
	 {
		line.erase(std::remove_if(line.begin(), line.end(), (int(*)(int))isspace), line.end());

		size_t pos = line.find(":") + 1;
		line = line.substr(pos);
		LOG("line: " << line);
	 }

	 return;
}

string InapMessage::getCalledPartyNumber()
{
	LOG("getCalledPartyNumber()");
	return str_cpn;
}



// TODO InapMessage::encodeConnect (...)


// TODO InapMessage::encodeRelease(...)



#include "InapMessage.hpp"

#include "ConnectArg.h"
#include "InitialDPArg.h"
#include "ReleaseCallArg.h"
#include "Common.h"

#include <iostream>
#include <sstream>

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
 //   m_calledPartyNumber == msg->calledPartyNumber;

  //  str_cpn = msg->calledPartyNumber->buf;
  //  str_cpn = reinterpret_cast<const unsigned char*>(msg->calledPartyNumber->buf);

//    (asn_struct_print_f)(
//    		struct asn_TYPE_descriptor_s *type_descriptor,
//    		const void *struct_ptr,
//    		int level,	/* Indentation level */
//    		asn_app_consume_bytes_f *callback, void *app_key


//    std::stringstream ss;
 //   asn_DEF_InitialDPArg.print_struct(&asn_DEF_InitialDPArg, msg, 0, 0, (void*)&ss);
//    ss << xer_fprint(stdout, &asn_DEF_InitialDPArg, msg);
//    str_cpn = ss.str();
//    LOG("stringstream " << str_cpn);

		FILE *fp;
		fp = fopen("testi.txt", "w+");
		asn_fprint(fp, &asn_DEF_InitialDPArg, msg);
		fclose(fp);


//	LOG("calledPartyNumber: " << ss);
	asn_DEF_InitialDPArg.free_struct(&asn_DEF_InitialDPArg, msg, 0);



   return 0;
}


void InapMessage::parseNum(istream is)
{
	string line;

	while(std::getline(is,line))
	{
		std::string tmp;
		for (int i = 0; i < line.length(); i++)
		{
			if(line[i] == ' ' && tmp.size() == 0)
			{
			}
			else
			{
				tmp += line[i];
			}
		}

		if (tmp == "calledPartyNumber: ")
		{
			str_cpn = tmp;
		}
	}
	LOG("str_cpn: " << str_cpn);
}

string InapMessage::getCalledPartyNumber()
{
	LOG("getCalledPartyNumber()");
	return str_cpn;
}



// TODO InapMessage::encodeConnect (...)


// TODO InapMessage::encodeRelease(...)




#define LOG(x) \
    std::cout << __FILE__ << "::" << __FUNCTION__ << ":" << __LINE__ << ": " << x << std::endl;

#include "InapMessage.hpp"


#include "ConnectArg.h"
#include "InitialDPArg.h"
#include "ReleaseCallArg.h"

#include <iostream>


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
	LOG("decoding InitialDPArg, msg size: " << _msg.size());

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
    	return -1;
    }
 //   m_calledPartyNumber == msg->calledPartyNumber;


    	/*
	 * This function clears the previous value of the OCTET STRING (if any)
	 * and then allocates a new memory with the specified content (str/size).
	 * If size = -1, the size of the original string will be determined
	 * using strlen(str).
	 * If str equals to NULL, the function will silently clear the
	 * current contents of the OCTET STRING.
	 * Returns 0 if it was possible to perform operation, -1 otherwise.
	 *
	 * int OCTET_STRING_fromBuf(OCTET_STRING_t *s, const char *str, int size);
	 */

	int retval = OCTET_STRING_fromBuf(msg->calledPartyNumber, str_cpn, -1);

	if (retval == -1)
	{
		LOG("ERROR  -  OCTET2char conversion failed");
		return 0;
	}

	LOG("OCTET2char conversion successful, calledPartyNumber: " << str_cpn);



   return 0;
}

const char InapMessage::getCalledPartyNumber()
{
	LOG("getCalledPartyNumber()");
	return *str_cpn;
}

// TODO InapMessage::encodeConnect (...)


// TODO InapMessage::encodeRelease(...)



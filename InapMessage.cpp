
#define LOG(x) \
    std::cout << __FILE__ << "::" << __FUNCTION__ << ":" << __LINE__ << ": " << x << std::endl;

#include "InapMessage.hpp"


#include "ConnectArg.h"
#include "InitialDPArg.h"
#include "ReleaseCallArg.h"

#include <iostream>

InapMessage::InapMessage()
{
	return;
}

InapMessage::InapMessage(int _localCode, const ByteStream &_incoming)
{
		LOG("INAP message - " << _incoming.size() );
		LOG(" " << _incoming);
		LOG("INAP localCode: " << _localCode);

		switch(_localCode)
		{
		case 0:
			{
				// InitialDPArg
				break;
			}
		case 20:
			{
				// ConnectArg
				break;
			}
		case 22:
		{
			// ReleaseCall
			break;
		}
		default:
			{
			LOG("ERROR: unknown localCode: " << _localCode);
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



   return 0;
}


// TODO InapMessage::encodeConnect (...)


// TODO InapMessage::encodeRelease(...)

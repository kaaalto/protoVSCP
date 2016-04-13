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

//#include "MapMessage.hpp"

//#define LOG(x) \
    std::cout << __FILE__ << "::" << __FUNCTION__ << ":" << __LINE__ << ": " << x << std::endl;

#include "TcapMessage.hpp"
#include "Common.h"

#include "ExternalPDU.h"
//#include "DialoguePDU.h"
#include "TCMessage.h"
#include "ReturnResult.h"
#include "constr_TYPE.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <errno.h>
#include <time.h>

#include <typeinfo>


//static const int tcap_id_as_dialogue[] = { 0, 0, 17, 773, 1, 1, 1 };
//static uint8_t _dial_version1 = 0x80;
//static BIT_STRING_t dial_version1 = { &_dial_version1, 1, 7 };

static int
write_out(const void *buffer, size_t size, void *app_key) {

    ByteStream *tmpbuf = static_cast<ByteStream*>(app_key);

    for (size_t i=0; i<size; i++) {
        unsigned char *byte = (unsigned char*) buffer;

        tmpbuf->push_back(*(byte+i));
    }

    return 0;
}



TcapMessage::TcapMessage() :
    m_operationLocalCode(0),
    m_transactionId(0),
    m_invokeId(-1)
{
    return;
}

TcapMessage::TcapMessage(ByteStream _incoming) :
    m_operationLocalCode(0),
    m_transactionId(0),
    m_invokeId(-1)


{
    // decode
    LOG("len = " << _incoming.size() ) ;




//        if (_incoming.size() > 21) {
//
//			// DialoguePortion / from External -> "Single-Asn.1-Type"
//			if (_incoming[21] == 0xA0) {
//				_incoming[21] = 0x80;
//				LOG ("FIX #2 external -> asn1 type") ;
//			}
//			if (_incoming[22] == 0xA0) {
//				_incoming[22] = 0x80;
//				LOG ("FIX #2 external -> asn1 type") ;
//			}
//
//			if (_incoming[8] == 0x6b) {
//				_incoming[8] = 0x4b;
//				LOG ("APPLICATION 11 (4b) -> UNIVERSAL 11 (6b) FIX #1 applied") ;
//			}
//
//			if (_incoming[9] == 0x6b) {
//				_incoming[9] = 0x4b;
//				LOG ("APPLICATION 11 (4b) -> UNIVERSAL 11 (6b) FIX #1 applied") ;
//        }
//    }


    TCMessage_t *msg = 0;    /* Note this 0! */
    asn_dec_rval_t rval;


    rval = asn_DEF_TCMessage.ber_decoder(0,
                                         &asn_DEF_TCMessage,
                                         (void **)&msg,
                                         &_incoming[0], _incoming.size(), 0);

    asn_fprint(stdout, &asn_DEF_TCMessage, msg);


    if (rval.code == RC_OK) {
    	LOG("ASN.1 Decode ok - consumed: " << rval.consumed);
        std::cout << "ASN.1 Decode ok\n";


        switch (msg->present)
        {
        case TCMessage_PR_begin:
        {
            m_transactionId  = msg->choice.begin.otid.buf[0];
            m_transactionId |= msg->choice.begin.otid.buf[1] << 8;
            m_transactionId |= msg->choice.begin.otid.buf[2] << 16;
            m_transactionId |= msg->choice.begin.otid.buf[3] << 24;

            std::cout << "begin transaction id: " << "\n"
                      << std::hex << m_transactionId << std::dec;

            int compCount = msg->choice.begin.components->list.count;

            std::cout << "begin comp. count: " << compCount << "\n";

            if (compCount != 1) {
                asn_DEF_TCMessage.free_struct(&asn_DEF_TCMessage, msg, 0);
                return;
            }

            Component_t* component = msg->choice.begin.components->list.array[0];

            switch (component->present)
            {
            case Component_PR_invoke:
            {
                std::cout << "TCAP/BEGIN/Invoke found\n";
                std::cout << "   invokeID: "
                          << component->choice.invoke.invokeID << "\n";

                m_invokeId = component->choice.invoke.invokeID;

                if (component->choice.invoke.opCode.present == OPERATION_PR_localValue)
                {

                    std::cout << "   opcode (localValue): "
                            << (long) component->choice.invoke.opCode.choice.localValue << "\n";

                  m_operationLocalCode = (long) component->choice.invoke.opCode.choice.localValue;

                }

                if (m_operationLocalCode == 0 && component->choice.invoke.parameter)
                {
                    std::cout << "   component parameter size: "
                              << component->choice.invoke.parameter->size << "\n";


                    for (int i=0; i<component->choice.invoke.parameter->size; i++)
                        m_parameterData.push_back(component->choice.invoke.parameter->buf[i]);
                }

                break;
            }
            default:
                std::cout << "TCAP/BEGIN/unknown comp: "
                          << component->present << "\n";
                break;
            }
            break;
        }
        default:
            std::cout << "TCAP/unknown prim: "
                      << msg->present << "\n";
            break;
        }

        return;
    } else {
    	LOG("ERROR  -  ASN.1 DECODING FAILED: " << rval.code);
        asn_DEF_TCMessage.free_struct(&asn_DEF_TCMessage, msg, 0);
        return;
    }

    asn_DEF_TCMessage.free_struct(&asn_DEF_TCMessage, msg, 0);
    return;
}

int TcapMessage::operationLocalCode() const
{
    return m_operationLocalCode;
}

ByteStream TcapMessage::parameterData() const
{
    return m_parameterData;
}

ByteStream TcapMessage::end(int _invokeId,
                            int _transactionId,
                            const ByteStream &_resultData)
{
    ByteStream msg;
    LOG ("<- dtid=" << std::hex << _transactionId
         << std::dec << ", result datalen=" << _resultData.size() ) ;

    TCMessage_t *tcMsg;     /* Type to encode        */
    asn_enc_rval_t ec;      /* Encoder return value  */

    /* Allocate the tcMsg */
    tcMsg  = (TCMessage_t*) calloc(1, sizeof(TCMessage_t));

    if(!tcMsg) {
        throw ("calloc failure");
        return msg;
    }

    tcMsg->present = TCMessage_PR_end;
    tcMsg->choice.end.dtid.buf = (uint8_t*) calloc(1, 4);
    tcMsg->choice.end.dtid.size = 4;
    tcMsg->choice.end.dtid.buf[0] = _transactionId & 0xff;
    tcMsg->choice.end.dtid.buf[1] = (_transactionId >> 8) & 0xff;
    tcMsg->choice.end.dtid.buf[2] = (_transactionId >> 16) & 0xff;
    tcMsg->choice.end.dtid.buf[3] = (_transactionId >> 24) & 0xff;



//    build ExternalPDU, DialoguePDU and AARQ apdu
//    ANY_t* anyAdapter = (ANY_t*) calloc(1, sizeof(ANY_t));
//    ExternalPDU_t* externalPDU = (ExternalPDU_t*) calloc(1,sizeof(ExternalPDU_t));
//    DialoguePDU_t* dialoguePDU = (DialoguePDU_t*) calloc(1,sizeof(DialoguePDU_t));
//    AARE_apdu_t* aare = (AARE_apdu_t*) calloc(1,sizeof(AARE_apdu_t));


//    // shortMsgMO-RelayContext-v3
//    const uint8_t map_as_dialogue[] = { 0, 4, 0, 0, 1, 0, 21, 3 };
//
//    // shortMsgMT-RelayContext-v3
//    //const uint8_t map_as_dialogue[] = { 0, 4, 0, 0, 1, 0, 25, 3 };
//
//    OBJECT_IDENTIFIER_set_arcs( &aare->application_context_name, map_as_dialogue,
//                               sizeof(map_as_dialogue[0]), 8);
//
//    OBJECT_IDENTIFIER_set_arcs(&externalPDU->oid,
//                       &tcap_id_as_dialogue,
//                       sizeof(tcap_id_as_dialogue[0]),
//             sizeof(tcap_id_as_dialogue)/sizeof(tcap_id_as_dialogue[0]));
//
//    aare->protocol_version =  &dial_version1;
//    aare->user_information = 0;
//
//
//
//    INTEGER_t* intT1 = (INTEGER_t*) calloc(1,sizeof(INTEGER_t));
//    int rc = asn_long2INTEGER(intT1, Associate_result_accepted);
//    if (rc != 0)
//        throw("can't convert long to INTEGER");
//
//    aare->result = *intT1; // int?
//
//
//    INTEGER_t* intT2 = (INTEGER_t*) calloc(1,sizeof(INTEGER_t));
//    rc = asn_long2INTEGER(intT2, 0);
//    if (rc != 0)
//        throw("can't convert long to INTEGER");
//
//    aare->result_source_diagnostic.present = Associate_source_diagnostic_PR_dialogue_service_user;
//    aare->result_source_diagnostic.choice.dialogue_service_user = *intT2;
//
//    dialoguePDU->present = DialoguePDU_PR_dialogueResponse; // AARE
//    dialoguePDU->choice.dialogueResponse = *aare;
//
//
//    ANY_fromType((ANY_t *) &externalPDU->dialog, &asn_DEF_DialoguePDU, dialoguePDU);
//    ANY_fromType(anyAdapter, &asn_DEF_ExternalPDU, externalPDU);
//
//    tcMsg->choice.end.dialoguePortion = (OCTET_STRING_t*) anyAdapter;


    // component invoke
    Component_t* component;
    component = (Component_t*) calloc(1,sizeof(Component_t));
    component->present = Component_PR_invoke;
//    component->choice.returnResultLast.invokeID = _invokeId;
//
//    component->choice.returnResultLast.resultretres = (struct ReturnResult::resultretres*) calloc(1,sizeof(*component->choice.returnResultLast.resultretres));
//    if(!component->choice.returnResultLast.resultretres) {
//         throw ("calloc failure");
//         return msg;
//     }
//    component->choice.returnResultLast.resultretres->opCode.present = OPERATION_PR_localValue;
//    component->choice.returnResultLast.resultretres->opCode.choice.localValue = 0x20;



    component->choice.invoke.invokeID = _invokeId;
    component->choice.invoke.opCode.present = OPERATION_PR_localValue;
    component->choice.invoke.opCode.choice.localValue = 0x14;

    ANY_t* parameter = (ANY_t*) calloc(1,sizeof(ANY_t));
    parameter->buf = (uint8_t*) calloc(1, _resultData.size());

    for (unsigned int i=0; i<_resultData.size(); i++)
        parameter->buf[i] = _resultData[i];

    parameter->size = _resultData.size();
 //   component->choice.returnResultLast.resultretres->parameter = parameter;
    component->choice.invoke.parameter = parameter;



    // component portion
    ComponentPortion_t* componentPort;
    componentPort = (ComponentPortion_t*) calloc(1,sizeof(ComponentPortion_t));

    void* foo = &(componentPort->list);
    int rc = ASN_SEQUENCE_ADD(foo, component);
 //  rc = ASN_SEQUENCE_ADD(&componentPort->list, component);


    if (rc != 0) {
          std::cout << "asn_seq_add() = rc=" << rc << ",errno: " << errno;
          throw("can't insert sequence");
      }

 //   tcMsg->choice.end.components = (ComponentPortion_t*) calloc(1, sizeof(componentPort));
 //   ASN_SEQUENCE_ADD(&tcMsg->choice.end.components->list, componentPort);

    tcMsg->choice.end.components =  componentPort;


    /* Encode the TCMessage type as BER (DER) */
    ByteStream tmpbuf;

    ec = der_encode(&asn_DEF_TCMessage, tcMsg, write_out, &tmpbuf);

    std::cout << "TcapMessage::end() der_encode.encoded="
              << (int) ec.encoded << "\n";

    LOG("TcapMessage::End Encoded");

    if(ec.encoded == -1)
    {
    	LOG("ERROR - TCAP ENCODING FAILED: " << ec.failed_type);
    }
    else{
        	LOG ("der_encode.encoded=" << (int) ec.encoded );
        }

//    if (ec.encoded != -1) {
//       if (tmpbuf[8] == 0x4b) {
//            tmpbuf[8] = 0x6b;
//            LOG ("APPLICATION 11 (4b) -> UNIVERSAL 11 (6b) FIX #1 applied") ;
//
//        }
//
//        if (tmpbuf[21] == 0x80) {
//            tmpbuf[21] = 0xA0;
//            // DialoguePortion / from External -> "Single-Asn.1-Type"
//        LOG ("FIX #2 external -> asn1 type") ;
//
//        }

        asn_fprint(stdout, &asn_DEF_TCMessage, tcMsg);
        msg = tmpbuf;

    LOG ("-> ") ;
    return msg;
}

//ByteStream TcapMessage::begin(const ByteStream &_componentData)
//{
//    std::cout << "TcapMessage::begin()\n";
//
//    ByteStream msg;
//
//
//    TCMessage_t *tcMsg;     /* Type to encode        */
//    Begin_t *beginMsg;     /* Type to encode        */
//    asn_enc_rval_t ec;      /* Encoder return value  */
//
//    /* Allocate the tcMsg */
//    tcMsg  = (TCMessage_t*) calloc(1, sizeof(TCMessage_t));
//    beginMsg  = (Begin_t*) calloc(1, sizeof(Begin_t));
//
//    if(!tcMsg || !beginMsg) {
//        throw ("calloc failure");
//        return msg;
//    }
//
//    // otid
//    OCTET_STRING_t *otidStr;
//    otidStr = (OCTET_STRING_t*) calloc(1, sizeof(OCTET_STRING_t));
//    otidStr->buf = (uint8_t*) calloc(1, 4);
//    otidStr->buf[0] = 0xaa;
//    otidStr->buf[1] = 0xbb;
//    otidStr->buf[2] = 0xcc;
//    otidStr->buf[3] = 0xdd;
//    otidStr->size = 4;
//
//    // dialogue portion
//    ////DialoguePortion_t* dialoguePort;
//    ////dialoguePort = (DialoguePortion_t*) calloc(1,sizeof(DialoguePortion_t));
//
////    // dialogue any
////    ANY_t* dial = (ANY_t*) calloc(1,sizeof(ANY_t));
////    dial->buf = (uint8_t*) calloc(1, 32);
////
////    // build ExternalPDU, DialoguePDU and AARQ apdu
//    ANY_t* anyAdapter = (ANY_t*) calloc(1, sizeof(ANY_t));
//    ExternalPDU_t* externalPDU = (ExternalPDU_t*) calloc(1,sizeof(ExternalPDU_t));
////    DialoguePDU_t* dialoguePDU = (DialoguePDU_t*) calloc(1,sizeof(DialoguePDU_t));
////    AARQ_apdu_t* aarq = (AARQ_apdu_t*) calloc(1,sizeof(AARQ_apdu_t));
////
////    ANY_fromType((ANY_t *) &externalPDU->dialog, &asn_DEF_DialoguePDU, dialoguePDU);
//    ANY_fromType(anyAdapter, &asn_DEF_ExternalPDU, externalPDU);
////
////    beginMsg->dialoguePortion = (OCTET_STRING_t*) anyAdapter;
////
////    // shortMsgMO-RelayContext-v3
////    //const uint8_t map_as_dialogue[] = { 0, 4, 0, 0, 1, 0, 21, 3 };
////
////    // shortMsgMT-RelayContext-v3
////    const uint8_t map_as_dialogue[] = { 0, 4, 0, 0, 1, 0, 25, 3 };
////
////    OBJECT_IDENTIFIER_set_arcs( &aarq->application_context_name, map_as_dialogue,
////                               sizeof(map_as_dialogue[0]), 8);
////
////    OBJECT_IDENTIFIER_set_arcs(&externalPDU§->oid,
////					   &tcap_id_as_dialogue,
////					   sizeof(tcap_id_as_dialogue[0]),
////            sizeof(tcap_id_as_dialogue)/sizeof(tcap_id_as_dialogue[0]));
//
////    aarq->protocol_version =  &dial_version1;
////    aarq->user_information = 0;
////
////
////    dialoguePDU->present = DialoguePDU_PR_dialogueRequest; // AARQ
////    dialoguePDU->choice.dialogueRequest = *aarq;
//
//
//    // component portion
//    ComponentPortion_t* componentPort;
//    componentPort = (ComponentPortion_t*) calloc(1,sizeof(ComponentPortion_t));
//
//    // component 1
//    Component_t* component;
//    component = (Component_t*) calloc(1,sizeof(Component_t));
//
//    // invoke for component 1
//    Invoke_t* invoke;
//    invoke = (Invoke_t*) calloc(1,sizeof(Invoke_t));
//    invoke->invokeID = 0; // typedef long     InvokeIdType_t
//
//    // operation for invoke 1 (for component)
//    OPERATION_t* operationType = (OPERATION_t*) calloc(1,sizeof(OPERATION_t));
//
////    INTEGER_t* intT = (INTEGER_t*) calloc(1,sizeof(INTEGER_t));
////    int rc = asn_long2INTEGER(intT, 44); // 46= moForward, 44=mtForward
////    if (rc != 0)						// OPERATION CODE NOLLAKSI
////        throw("can't convert long to INTEGER");
//
//    long int* intT = 0;
//
//    operationType->present = OPERATION_PR_localValue;
//    operationType->choice.localValue = *intT; // local operation code!
//
//    // MAP parameter into invoke
//
//    int mapSize = _componentData.size();
//    std::cout << "map operation size: " << mapSize << "\n";
//    ANY_t* parameter = (ANY_t*) calloc(1,sizeof(ANY_t));
//    parameter->buf = (uint8_t*) calloc(1, mapSize);
//
//
//        for (int i=0; i<mapSize; i++)
//        parameter->buf[i] = _componentData[i];
//
//    parameter->size = mapSize;
//
//    invoke->opCode = *operationType;
//    invoke->parameter = parameter;
//
//    component->present = Component_PR_invoke;
//    component->choice.invoke = *invoke;
//
//    // populate msg
//    tcMsg->present = TCMessage_PR_begin;
//
//    beginMsg->otid = *otidStr;
//
////    ANY_fromType((ANY_t *) &externalPDU->dialog, &asn_DEF_DialoguePDU, dialoguePDU);
//    ANY_fromType(anyAdapter, &asn_DEF_ExternalPDU, externalPDU);
//
////    beginMsg->dialoguePortion = (OCTET_STRING_t*) anyAdapter;
//    void* foo = &(componentPort->list);
//
//    int rc = ASN_SEQUENCE_ADD(foo, component);
//    if (rc != 0) {
//        std::cout << "asn_seq_add() = rc=" << rc << ",errno: " << errno;
//        throw("can't insert sequence");
//    }
//
//
//    beginMsg->components =  componentPort;
//
//    tcMsg->choice.begin = *beginMsg;
//
//    /* Encode the TCMessage type as BER (DER) */
//    ByteStream tmpbuf;
//    ec = der_encode(&asn_DEF_TCMessage, tcMsg, write_out, &tmpbuf);
//
//
//    std::cout << "TcapMessage::begin() der_encode.encoded="
//              << (int) ec.encoded << "\n";
//
//    if (ec.encoded != -1) {
//        if (tmpbuf[8] == 0x4b) {
//            tmpbuf[8] = 0x6b;
//            LOG ("APPLICATION 11 (4b) -> UNIVERSAL 11 (6b) FIX #1 applied") ;
//
//        }
//        if (tmpbuf[9] == 0x4b) {
//            tmpbuf[9] = 0x6b;
//            LOG ("APPLICATION 11 (4b) -> UNIVERSAL 11 (6b) FIX #1 applied") ;
//
//        }
//
//
//        if (tmpbuf[21] == 0x80) {
//            tmpbuf[21] = 0xA0;
//            // DialoguePortion / from External -> "Signel-Asn.1-Type"
//            LOG ("FIX #2 external -> asn1 type") ;
//
//        }
//        if (tmpbuf[22] == 0x80) {
//            tmpbuf[22] = 0xA0;
//            // DialoguePortion / from External -> "Signel-Asn.1-Type"
//            LOG ("FIX #2 external -> asn1 type") ;
//
//        }
//
//        asn_fprint(stdout, &asn_DEF_TCMessage, tcMsg);
//
//        msg = tmpbuf;
//
//    }
//
//    return msg;
//}

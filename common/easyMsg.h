/*
 * easyMsg.h
 *
 *  Created on: 2015年8月25日
 *      Author: millerzhao
 */

#ifndef EASYMSG_H_
#define EASYMSG_H_

#include <memory.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
namespace EasyMQ
{
	const int32_t MAX_TOPIC_LEN=32; //限定最长的topic长度
	struct Asn20Msg ;

    struct Msg
    {
        enum MsgType
        {
            MSG_TYPE_REQ_INIT_TOPIC = 0x10,
            MSG_TYPE_RESP_INIT_TOPIC = 0x20,

            MSG_TYPE_REQ_PUBLISH = 0x11,
            MSG_TYPE_RESP_PUBLISH = 0x21,

            MSG_TYPE_REQ_SUBSCRIBE = 0x12,
            MSG_TYPE_RESP_SUBSCRIBE = 0x22,

            MSG_TYPE_REQ_ECHO = 0x31,
            MSG_TYPE_RESP_ECHO = 0x32
        };

        enum MsgRet
        {
            MSG_RET_SUCC = 0xe0,
            MSG_RET_FAIL = 0xe1
        };

		int32_t len()
		{
			return sizeof(Msg) + uBufLen;
		}
        MsgType type;
        MsgRet retCode;
		uint32_t srcIP; //源ip地址,订阅时传递，后面可以转发消息
		short srcPort; //源port
		char topic[MAX_TOPIC_LEN];
        uint32_t uBufLen;//最后cBuf的大小
        char cBuf[0];
	};

	struct Asn20Msg
    {
		uint32_t len()
		{
			return sizeof(Asn20Msg)+msg.uBufLen;
		}
        uint32_t msgTag;
        uint32_t msgLen; //包含包头等所有的长度
		struct Msg msg;
    };
	Asn20Msg *getAsn20Msg(const char *);
	void freeAsn20Msg(Asn20Msg*);
}



#endif /* EASYMSG_H_ */

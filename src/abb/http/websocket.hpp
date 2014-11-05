// WebSocket, v1.00 2012-09-13
//
// Description: WebSocket FRC6544 codec, written in C++.
// Homepage: http://katzarsky.github.com/WebSocket
// Author: katzarsky@gmail.com

#ifndef __ABB_HTTP_WEBSOCKET_HPP_
#define	__ABB_HTTP_WEBSOCKET_HPP_

#include <assert.h>
#include <stdint.h> /* uint8_t */
#include <stdio.h> /* sscanf */
#include <ctype.h> /* isdigit */
#include <stddef.h> /* int */

// std c++
#include <vector> 
#include <string> 

#include "abb/base/buffer.hpp"


using namespace std;
namespace abb{
namespace http{
enum WebSocketFrameType {
	ERROR_FRAME=0xFF00,
	INCOMPLETE_FRAME=0xFE00,

	OPENING_FRAME=0x3300,
	CLOSING_FRAME=0x3400,

	INCOMPLETE_TEXT_FRAME=0x01,
	INCOMPLETE_BINARY_FRAME=0x02,

	TEXT_FRAME=0x81,
	BINARY_FRAME=0x82,

	PING_FRAME=0x19,
	PONG_FRAME=0x1A,

	CLOSE_FRAME=0x8
};
enum WebSocketState{
	STATE_HEAD,
	STATE_PAYLOAD_LENGTH,
	STATE_PAYLOAD_DATA,
};
class WebSocket
{
public:
	string resource;
	string host;
	string origin;
	string protocol;
	string key;
	std::string version;
	std::string path;

	WebSocket();
	string answerHandshake();

	int MakeFrame(WebSocketFrameType frame_type, const void* msg, int msg_len, abb::Buffer& buf);
	WebSocketFrameType GetFrame(abb::Buffer& buf, uint64_t* out_length);
	int state_;
	uint64_t payload_length_;
	unsigned char msg_opcode_;
	unsigned char msg_fin_;
	unsigned char msg_masked_;
	int length_field_;
};

}
}
#endif	/* WEBSOCKET_H */

// WebSocket, v1.00 2012-09-13
//
// Description: WebSocket FRC6544 codec, written in C++.
// Homepage: http://katzarsky.github.com/WebSocket
// Author: katzarsky@gmail.com

#include "websocket.hpp"


#include "abb/base/base64.hpp"
#include "abb/base/sha1.hpp"
#include "abb/base/log.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
namespace abb
{
namespace http
{
WebSocket::WebSocket( ):state_(STATE_HEAD){

}



string WebSocket::answerHandshake() 
{
    unsigned char digest[20]; // 160 bit sha1 digest

	string answer;
	answer += "HTTP/1.1 101 Switching Protocols\r\n";
	answer += "Upgrade: websocket\r\n";
	answer += "Connection: Upgrade\r\n";
	if(this->key.length() > 0) {
		string accept_key;
		accept_key += this->key;
		accept_key += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"; //RFC6544_MAGIC_KEY

		printf("INTERMEDIATE_KEY:(%s)\n", accept_key.data());

		SHA1 sha;
		sha.Input(accept_key.data(), accept_key.size());
		sha.Result((unsigned*)digest);
		
		printf("DIGEST:"); for(int i=0; i<20; i++) printf("%02x ",digest[i]); printf("\n");

		//little endian to big endian
		for(int i=0; i<20; i+=4) {
			unsigned char c;

			c = digest[i];
			digest[i] = digest[i+3];
			digest[i+3] = c;

			c = digest[i+1];
			digest[i+1] = digest[i+2];
			digest[i+2] = c;
		}

		printf("DIGEST:"); for(int i=0; i<20; i++) printf("%02x ",digest[i]); printf("\n");

		accept_key = base64_encode((const unsigned char *)digest, 20); //160bit = 20 bytes/chars

		answer += "Sec-WebSocket-Accept: "+(accept_key)+"\r\n";
	}
	if(this->protocol.length() > 0) {
		answer += "Sec-WebSocket-Protocol: "+(this->protocol)+"\r\n";
	}
	
	answer += "\r\n";
	return answer;

	//return WS_OPENING_FRAME;
}

int WebSocket::MakeFrame(WebSocketFrameType frame_type, const void* msg, int msg_len, abb::Buffer& buf)
{
	int size = msg_len; 
	buf << static_cast<unsigned char>(frame_type);
	if(size<=125) {
		buf << static_cast<unsigned char>(size);
	}else if(size<=65535) {
		buf << static_cast<unsigned char>(126);
		buf.NET_WriteUint16(size);
	}else { // >2^16-1
		buf << static_cast<unsigned char>(127);
		buf.NET_WriteUint64(size);
	}
	buf.Write(msg,size);
	return 0;
}

WebSocketFrameType WebSocket::GetFrame(abb::Buffer& buf, uint64_t* out_length)
{
	
	if(STATE_HEAD == state_){
		if(buf.ReadSize() < 2) return INCOMPLETE_FRAME;
		unsigned char a,b;
		buf >> a >> b;
		msg_opcode_ 	= a & 0x0F;
		msg_fin_ 	= (a >> 7) & 0x01;
		msg_masked_ = b & 0x80;
		// *** message decoding 
		length_field_ = b & (~0x80);

		state_ = STATE_PAYLOAD_LENGTH;
		LOG(DEBUG) << "opcode: " << msg_opcode_ 
		<< " masked: " << msg_masked_ 
		<< " msgfin: " << msg_fin_
		<< " length_field: " << length_field_;
	}
	if(state_ == STATE_PAYLOAD_LENGTH){
		if(length_field_ <= 125) {
			payload_length_ = length_field_;
		}
		else if(length_field_ == 126) { //msglen is 16bit!
			if(buf.ReadSize() < 2) return INCOMPLETE_FRAME;
			uint16_t ext_len = buf.HOST_ReadUint16();
			payload_length_ = ext_len;
		}
		else if(length_field_ == 127) { //msglen is 64bit!
			if(buf.ReadSize() < 8) return INCOMPLETE_FRAME;
			uint64_t ext_len = buf.HOST_ReadUint64();
			payload_length_ = ext_len;
		}
		printf("PAYLOAD_LEN: %08x\n", payload_length_);
		state_ = STATE_PAYLOAD_DATA;
	}
	if(state_ == STATE_PAYLOAD_DATA){
		if(msg_masked_) {
			printf("BUFFER_LEN: %08x\n", buf.ReadSize());
			if( buf.ReadSize() < payload_length_ + 4 ) {
				return INCOMPLETE_FRAME;
			}
			unsigned int mask;
			buf >> mask;
			printf("MASK: %08x\n", mask);

			// unmask data:
			unsigned char* c = (unsigned char*)buf.ReadPtr();
			for(uint64_t i=0; i<payload_length_; i++) {
				c[i] = c[i] ^ ((unsigned char*)(&mask))[i%4];
			}
		}else{
			if( buf.ReadSize() < payload_length_ ) {
				return INCOMPLETE_FRAME;
			}
		}
		*out_length = payload_length_;
		state_ = STATE_HEAD;
		if(msg_opcode_ == 0x0) return (msg_fin_)?TEXT_FRAME:INCOMPLETE_TEXT_FRAME; // continuation frame ?
		if(msg_opcode_ == 0x1) return (msg_fin_)?TEXT_FRAME:INCOMPLETE_TEXT_FRAME;
		if(msg_opcode_ == 0x2) return (msg_fin_)?BINARY_FRAME:INCOMPLETE_BINARY_FRAME;
		if(msg_opcode_ == 0x8) return CLOSE_FRAME;
		if(msg_opcode_ == 0x9) return PING_FRAME;
		if(msg_opcode_ == 0xA) return PONG_FRAME;
	}
	return ERROR_FRAME;
}

}}


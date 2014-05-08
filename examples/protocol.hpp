
#ifndef PROTOCOL_HPP_
#define PROTOCOL_HPP_

#include <abb/net/i_protocol.hpp>

using namespace abb::net;
using namespace abb::base;

class Protocol:public IProtocol{
public:
	virtual ~Protocol(){};
	void Encode(Buffer& buf,Msg msg){
	}
	Msg Decode(Buffer& buf){
		int size;
		buf >> size;
		return Msg(size);
	}
};

class ProtocolFactory:public IProtocolFactory{
public:
	virtual ~ProtocolFactory(){};
	IProtocol* CreateProtocol(){
		return new Protocol();
	}
	void DestroyProtocol(IProtocol*val){
		delete val;
	}
};

#endif /* PROTOCOL_HPP_ */

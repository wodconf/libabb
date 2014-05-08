#ifndef __ABB_I_PROTOCOL_HPP_
#define __ABB_I_PROTOCOL_HPP_

#include "abb/base/buffer.hpp"

namespace abb{
namespace net{
typedef void* Msg;
class IProtocol{
public:
	virtual ~IProtocol(){};
	void Encode(base::Buffer& buf,Msg msg)=0;
	Msg Decode(base::Buffer& buf)=0;
};

class IProtocolFactory{
public:
	virtual ~IProtocolFactory(){};
	IProtocol* CreateProtocol()=0;
	void DestroyProtocol(IProtocol*val)=0;
};
}
}
#endif

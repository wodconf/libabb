#ifndef __ABB_I_PROTOCOL_HPP_
#define __ABB_I_PROTOCOL_HPP_

#include "abb/base/buffer.hpp"

namespace abb{
namespace net{
typedef void* Msg;
class IProtocol{
public:
	virtual ~IProtocol(){};
	void Encode(base::Buffer& buf,Msg msg);
	Msg Decode(base::Buffer& buf);
};

class IProtocolFactory{
public:
	virtual ~IProtocolFactory(){};
	IProtocol* CreateProtocol();
	void DestroyProtocol(IProtocol*val);
};
}
}
#endif

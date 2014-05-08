#ifndef __ABB_I_PROTOCOL_HPP_
#define __ABB_I_PROTOCOL_HPP_

#include "abb/base/buffer.hpp"

namespace abb{
namespace net{
class IProtocol{
public:
	virtual ~IProtocol(){};
	virtual void Encode(base::Buffer& buf,void* msg)=0;
	virtual void* Decode(base::Buffer& buf)=0;
};

class IProtocolFactory{
public:
	virtual ~IProtocolFactory(){};
	virtual IProtocol* CreateProtocol()=0;
	virtual void DestroyProtocol(IProtocol*val)=0;
};
}
}
#endif

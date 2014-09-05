#ifndef __ABB_NET_LISTENER_HPP__
#define __ABB_NET_LISTENER_HPP__

#include "abb/base/buffer.hpp"

namespace abb{
namespace net{
class Connection;
class Connector;
class Acceptor;
class SocketAddress;
class ConnectionRef;
class IConnectionListener{
public:
	virtual ~IConnectionListener(){};
	virtual void L_Connection_OnOpen(Connection* self)=0;
	virtual void L_Connection_OnMessage(Connection* self,Buffer& buf)=0;
	virtual void L_Connection_WriteComplete(Connection* self)=0;
	virtual void L_Connection_OnClose(Connection* self,int error)=0;
};
class IConnectorListener{
public:
	virtual ~IConnectorListener(){};
	virtual void L_Connector_OnOpen(Connector* cotr,int fd)=0;
	virtual void L_Connector_OnClose(Connector* cotr,int error)=0;
};
class IAcceptorListener{
public:
	virtual ~IAcceptorListener(){};
	virtual void L_Acceptor_OnConnection(Acceptor* ptr,int fd,const SocketAddress& addr) = 0;
};

}
}

#endif

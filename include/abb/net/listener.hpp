#ifndef __ABB_NET_LISTENER_HPP__
#define __ABB_NET_LISTENER_HPP__

namespace abb{
namespace net{
class Connection;
class Connector;
class Acceptor;
class IPAddr;
class IConnectionListener{
public:
	virtual ~IConnectionListener(){};
	virtual void L_Connection_OnMessage(Connection* self,base::Buffer& buf)=0;
	virtual void L_Connection_OnClose(Connection* self,int error)=0;
};
class IConnectorListener{
public:
	virtual ~IConnectorListener(){};
	virtual void L_Connector_OnOpen(Connection* cotr,int fd)=0;
	virtual void L_Connector_OnClose(Connection* cotr,int error)=0;
};
class IAcceptorListener{
	virtual ~IAcceptorListener(){};
	virtual void L_Acceptor_OnConnection(Acceptor* ptr,int fd,const IPAddr& addr) = 0;
};
}
}

#endif
#ifndef __ABB_NET_TCP_SERVER_HPP__
#define __ABB_NET_TCP_SERVER_HPP__

#include <map>

#include "abb/net/ip_addr.hpp"
#include "abb/net/listener.hpp"
#include "abb/net/connection_ref.hpp"

namespace abb{
namespace net{
class EventLoopGroup;
class EventLoop;
class TcpServer:public IAcceptorListener,IConnectionListener{
public:
	class Listener{
	public:
		virtual ~Listener(){}
		virtual void L_TcpServer_OnConnection(ConnectionRef*) = 0;
		virtual void L_TcpServer_OnMesssage(ConnectionRef*,Buffer& buf)= 0;
		virtual void L_TcpServer_OnClose(ConnectionRef*,int error)= 0;
		virtual void L_TcpServer_WriteComplete(ConnectionRef*){}
	};
	TcpServer();
	virtual ~TcpServer();
	void Init(int num_thread,bool run_curent_thread);
	void SetListener(Listener* lis){lis_ = lis;}
	bool Bind(const SocketAddress& addr,int* save_error);
	void Start();
	void Pause();
	void Resume();
	void Close();
	const SocketAddress& GetAddr() const;
	EventLoopGroup* GetEventLoopGroup() const{
		return event_group_;
	}
private:
	virtual void L_Acceptor_OnConnection(Acceptor* ptr,int fd,const SocketAddress& addr);
	virtual void L_Connection_OnOpen(Connection* self);
	virtual void L_Connection_OnMessage(Connection* self,Buffer& buf);
	virtual void L_Connection_OnClose(Connection* self,int error);
	virtual void L_Connection_WriteComplete(Connection* self);
private:
	Listener* lis_;
	EventLoopGroup* event_group_;
	EventLoopGroup* accept_group_;
	Acceptor* acceptor_;
	EventLoop* loop_;
};
}
}

#endif

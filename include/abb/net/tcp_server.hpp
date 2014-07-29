#ifndef __ABB_NET_TCP_SERVER_HPP__
#define __ABB_NET_TCP_SERVER_HPP__

#include "./ip_addr.hpp"
#include "./listener.hpp"
#include "abb/base/mutex.hpp"
#include "connection_ref.hpp"
#include <map>
namespace abb{
namespace net{
class Context;
class TcpServer:public IAcceptorListener,IConnectionListener{
public:
	class Listener{
	public:
		virtual ~Listener(){}
		virtual void L_TcpServer_OnConnection(ConnectionRef*) = 0;
		virtual void L_TcpServer_OnMesssage(ConnectionRef*,base::Buffer& buf)= 0;
		virtual void L_TcpServer_OnClose(ConnectionRef*,int error)= 0;
	};
	TcpServer();
	virtual ~TcpServer();
	void Init(int num_thread,bool run_curent_thread);
	void SetListener(Listener* lis){lis_ = lis;}
	bool Bind(const IPAddr& addr,int* save_error);
	void Start();
	void Pause();
	void Resume();
	void Close();
	const IPAddr& GetAddr();
private:
	virtual void L_Acceptor_OnConnection(Acceptor* ptr,int fd,const IPAddr& addr);
	virtual void L_Connection_OnMessage(Connection* self,base::Buffer& buf);
	virtual void L_Connection_OnClose(Connection* self,int error);
private:
	Listener* lis_;
	Context* ctx_;
	Acceptor* acceptor_;
	typedef std::map<int,Connection*> ConnectionMap;
	ConnectionMap conn_map_;
	int id_;
	base::Mutex mtx_;
};
}
}

#endif
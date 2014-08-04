#ifndef __ABB_HTTP_SERVER_HPP__
#define __ABB_HTTP_SERVER_HPP__

#include "abb/net/tcp_server.hpp"

namespace abb{
namespace http{
class Request;
class Server:public net::TcpServer::Listener{
public:
	class Listener{
	public:
		virtual ~Listener(){};
		virtual void HandleRequest(Request* req) = 0;
	};
	Server();
	virtual ~Server();
	void Init(int num_thread,bool run_curent_thread){
		tcp_svr_.Init(num_thread,run_curent_thread);
	}
	void SetListener(Listener* lis){lis_ = lis;}
	bool Bind(const net::IPAddr& addr,int* save_error){
		return tcp_svr_.Bind(addr,save_error);
	}
	void Start(){
		tcp_svr_.Start();
	}
	void Pause(){
		tcp_svr_.Pause();
	}
	void Resume(){
		tcp_svr_.Resume();
	}
	void Close(){
		tcp_svr_.Close();
	}
	const net::IPAddr& GetAddr(){
		return tcp_svr_.GetAddr();
	}
private:
	virtual void L_TcpServer_OnConnection(net::ConnectionRef*);
	virtual void L_TcpServer_OnMesssage(net::ConnectionRef*,base::Buffer& buf);
	virtual void L_TcpServer_OnClose(net::ConnectionRef*,int error);
private:
	net::TcpServer tcp_svr_;
	Listener* lis_;
};

}}

#endif

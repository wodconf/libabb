
#ifndef TCP_CLIENT_MANAGER_HPP_
#define TCP_CLIENT_MANAGER_HPP_
#include "ip_addr.hpp"
#include "listener.hpp"
#include "../base/mutex.hpp"
#include "abb/net/connection_ref.hpp"
namespace abb {
namespace net {
class TcpClient;
class Context;
class TcpClientManager {
public:
	class Listener{
	public:
		virtual ~Listener(){}
		virtual void L_TcpClient_OnConnectFail(int error) = 0;
		virtual void L_TcpClient_OnConnection(ConnectionRef* conn) = 0;
		virtual void L_TcpClient_OnMessage(ConnectionRef* conn,base::Buffer& buf) = 0;
		virtual void L_TcpClient_OnClose(ConnectionRef* conn,int error) = 0;
	};
	TcpClientManager();
	virtual ~TcpClientManager();
	void Init(int num_thread,bool run_curent_thread);
	void Start();
	bool Connect(const IPAddr& addr,Listener*lis);
private:
	friend TcpClient;
	Context* ctx_;
};

} /* namespace net */
} /* namespace abb */

#endif /* TCP_CLIENT_MANAGER_HPP_ */

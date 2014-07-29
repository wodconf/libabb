
#ifndef __ABB_NET_TCP_CLIENT_HPP_
#define __ABB_NET_TCP_CLIENT_HPP_
#include "abb/net/listener.hpp"
#include "abb/net/connection_ref.hpp"
#include "abb/net/ip_addr.hpp"
namespace abb {
namespace net {

class Loop;
class ITcpClientListener{
public:
	virtual ~ITcpClientListener(){}
	virtual void L_TcpClient_OnConnectFail(int error) = 0;
	virtual void L_TcpClient_OnConnection(ConnectionRef* conn) = 0;
	virtual void L_TcpClient_OnMessage(ConnectionRef* conn,base::Buffer& buf) = 0;
	virtual void L_TcpClient_OnClose(ConnectionRef* conn,int error) = 0;
};
namespace tcp{
extern bool Connect(Loop* loop,const IPAddr& addr,int* save_error,ITcpClientListener* lis);
}

} /* namespace monprxoy */
} /* namespace adcloud */

#endif /* TCP_CLIENT_HPP_ */

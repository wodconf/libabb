#ifndef __ABB_HTTP_WS_CONNECTION_HPP__
#define __ABB_HTTP_WS_CONNECTION_HPP__

#include "abb/base/ref_object.hpp"
#include "abb/net/ip_addr.hpp"
#include "abb/base/buffer.hpp"
namespace abb
{
namespace net{
	class ConnectionRef;
	class EventLoop;
}
namespace http
{

class WebSocket;
class WSConnection:public abb::RefObject{
public:
	class Listener{
	public:
		virtual ~Listener(){};
		virtual void OnTextMessage(WSConnection* conn,const std::string& str) = 0;
		virtual void OnBinaryMessage(WSConnection* conn,void* buf,int size)  = 0;
		virtual void OnClose(WSConnection* conn,int error)  = 0;
	};

	WSConnection(net::ConnectionRef* ref,WebSocket* pro);
	~WSConnection();
	void SetListener(Listener* lis){lis_=lis;}
	void SendText(const std::string& text,bool flush = false);
	void SendBinary(const void* buf,int size,bool flush = false);
	void Flush();
	void Close();
	const std::string& GetOrigin() const ;
	const std::string& GetPath()  const ;
	net::EventLoop* GetEventLoop();
	const net::SocketAddress& GetLocalAddr() const;
	const net::SocketAddress& GetRemoteAddr() const;
	bool Connected() const ;
	//internal
	void OnMessage( Buffer& buf );
	void OnClose( int error );

	void* Data;
private:
	WebSocket* ws_;
	net::ConnectionRef* conn_ref_;
	Listener* lis_;
};
	
}
	
}


#endif

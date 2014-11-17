#ifndef __ABB_HTTP_RESPONCE_WRITER_HPP__
#define __ABB_HTTP_RESPONCE_WRITER_HPP__

#include "abb/http/responce.hpp"
#include "abb/base/ref_object.hpp"
#include "abb/net/ip_addr.hpp"
namespace abb{
namespace net{
	class ConnectionRef;
}
namespace http{

class ResponceWriter:public RefObject{
public:
	ResponceWriter(net::ConnectionRef* ref,const std::string& version);
	~ResponceWriter();
	Responce& GetResponce(){return rsp_;}
	void Flush();
	const net::SocketAddress& PeerAddr() const;
private:
	net::ConnectionRef* conn_ref_;
	Responce rsp_;
};
}
}

#endif

#ifndef __ABB_HTTP_RESPONCE_WRITER_HPP__
#define __ABB_HTTP_RESPONCE_WRITER_HPP__

#include "abb/http/http_responce.hpp"
#include "abb/base/ref_object.hpp"

namespace abb{
namespace net{
	class ConnectionRef;
}
namespace http{

class ResponceWriter:public base::RefObject{
public:
	ResponceWriter(net::ConnectionRef* ref,const std::string& version);
	~ResponceWriter();
	Responce& GetResponce(){return rsp_;}
	void Flush();
private:
	ConnectionRef* conn_ref_;
	Responce rsp_;
};
}
}

#endif
#include "abb/http/http_responce_writer.hpp"
#include "abb/net/connection_ref.hpp"
namespace abb{
namespace http{

ResponceWriter::ResponceWriter(net::ConnectionRef* ref,const std::string& version)
:conn_ref_(ref),rsp_(version){
	conn_ref_->Ref();
}
ResponceWriter::~ResponceWriter(){
	conn_ref_->UnRef();
}
void ResponceWriter::Flush(){
	abb::Buffer* buf;
	if( conn_ref_->LockWrite(&buf)){
		rsp_.Encode(*buf);
		conn_ref_->UnLockWrite(true);
		conn_ref_->CloseAfterWrite();
	}
}
}
}

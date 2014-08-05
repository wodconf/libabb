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
	abb::base::Buffer* buf;
	if( conn_ref_->LockWrite(&buf)){
		rsp_.Encode(*buf);
		LOG(DEBUG) << "DD " << std::string((char*)(buf->Data()),buf.Size());
		conn_ref_->UnLockWrite();
		conn_ref_->CloseAfterWrite();
	}
}
}
}

#include "abb/http/client.hpp"
#include "abb/net/tcp_client.hpp"
#include "abb/http/request.hpp"
#include "abb/http/responce.hpp"
#include "abb/http/const.hpp"
#include "abb/http/decoder.hpp"
#include "abb/base/thread.hpp"
#include "abb/base/log.hpp"
#include "abb/net/socket.hpp"
namespace abb{
namespace http{

class Client:public net::ITcpClientListener{
public:
	Client(Request* req,IRequestHandler* handler)
	:req_(req),
	handler_(handler),
	read_responced_(false),
	error_(-1)
	{
	}
	virtual ~Client(){
		req_->UnRef();
	}
	virtual void L_TcpClient_OnConnectFail(int error){
		handler_->HandleError(error);
		delete this;
	}
	virtual void L_TcpClient_OnConnection(net::ConnectionRef* conn){
		conn->SetNoDelay(true,NULL);
		abb::Buffer* buf;
		if( conn->LockWrite(&buf)){
			req_->Encode(*buf);
			conn->UnLockWrite(true);
			conn->Data = new ResponceDecoder();
		}
	}
	virtual void L_TcpClient_OnMessage(net::ConnectionRef* conn,Buffer& buf){
		ResponceDecoder* d = static_cast<ResponceDecoder*>(conn->Data);
		if( !d->Decode(buf) ){
			conn->Close();
			error_ = 1001;
		}else{
			Responce* rsp = d->GetResponce();
			if(rsp){
				read_responced_= true;
				if(this->handler_)this->handler_->HandleResponce(rsp);
				rsp->UnRef();
				conn->Close();
			}
		}
	}
	virtual void L_TcpClient_OnClose(net::ConnectionRef* conn,int error){
		if(!read_responced_){
			if(error_ > 0){
				if(this->handler_)this->handler_->HandleError(error_);
			}else{
				if(this->handler_)this->handler_->HandleError(error);
			}
		}
		delete this;
	}
private:
	Request* req_;
	IRequestHandler* handler_;
	bool read_responced_;
	int error_;
};

bool Post(net::EventLoop* loop,
				const std::string& url,
				const std::string& body_type,
				const void* body,
				int size,
				IRequestHandler* handler){
	Request* req = new Request(http::method::POST,"HTTP/1.1");
	if(!req->SetUrl(url)){
		req->UnRef();
		return false;
	}else{
		req->GetHeader().Set(abb::http::header::CONTENT_TYPE,body_type);
		req->Body().Write(body,size);
		return Do(loop,req,handler);
	}
}
bool Get(net::EventLoop* loop,const std::string& url,IRequestHandler* handler){
	Request* req = new Request(http::method::GET,"HTTP/1.1");
	if(!req->SetUrl(url)){
		return false;
	}else{
		return Do(loop,req,handler);
	}
}
bool Do(net::EventLoop* loop,Request* req,IRequestHandler* handler){
	net::SocketAddress addr;
	if( req->GetURL().Host.find(":") == std::string::npos){
		if(!addr.SetByString(req->GetURL().Host + ":80")){
			return false;
		}
	}else{
		if( !addr.SetByString(req->GetURL().Host) ){
			return false;
		}
	}
	Client* htc = new Client(req,handler);
	net::tcp::Connect(loop,addr,htc);
	return true;
}

Responce* SyncDo(Request& req,int* error,int ms_timeout){
	net::SocketAddress addr;
	if( req.GetURL().Host.find(":") == std::string::npos){
		if(!addr.SetByString(req.GetURL().Host + ":80")){
			return NULL;
		}
	}else{
		if( !addr.SetByString(req.GetURL().Host) ){
			return NULL;
		}
	}
	int fd;
	int timeout = ms_timeout;
	if(timeout <= 0){
		timeout = 10000;
	}
	if( ! abb::net::Socket::Connect(&fd,true,addr,error,ms_timeout) ){
		return NULL;
	}
	abb::net::Socket::SetNoDelay(fd,true,NULL);
	abb::Buffer buf;
	req.Encode(buf);
	int nw;
	if( !abb::net::Socket::Write(fd,buf.ReadPtr(),buf.ReadSize(),&nw,error) ){
		abb::net::Socket::Close(fd);
		return NULL;
	}
	abb::Buffer recvbuf;
	Responce* rsp;
	ResponceDecoder coder;
	int num_read = 0; ;
	if(ms_timeout > 0)
		abb::net::Socket::SetRecvTimeout(fd,ms_timeout);
	for(int i=0;i<10;i++){
		int nr;
		recvbuf.EnoughSize(4096);
		if( !abb::net::Socket::Read( fd,  (char*)recvbuf.WritePtr(),recvbuf.WriteSize(),&nr,error) || nr == 0 ){
			if(!rsp){
				LOG(WARN) << "decode not complete when close num_read:" << num_read << "  state:" << coder.State() << "left.size" << recvbuf.ReadSize() << "  " << *error;
			}
			break;
		}
		recvbuf.GaveWrite(nr);
		num_read += nr;

		if( !coder.Decode(recvbuf) ){
			break;
		}else{
			rsp = coder.GetResponce();
			if(rsp){
				break;
			}
		}
	}
	abb::net::Socket::Close(fd);
	return rsp;
}

Responce* SyncPost(const std::string& url,
						const std::string& body_type,
						const void* body,
						int size,int* error,int ms_timeout){
	Request req(http::method::POST,"HTTP/1.1");
	if(!req.SetUrl(url)){
		return NULL;
	}else{
		req.GetHeader().Set(abb::http::header::CONTENT_TYPE,body_type);
		req.Body().Write(body,size);
		return SyncDo(req,error,ms_timeout);
	}
}
Responce* SyncGet(const std::string& url,int* error,int ms_timeout){
	Request req(http::method::GET,"HTTP/1.1");
	if(!req.SetUrl(url)){
		return NULL;
	}else{
		return SyncDo(req,error,ms_timeout);
	}
}

}}

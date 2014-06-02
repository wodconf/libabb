
#include <abb/net/tcp_client_manager.hpp>
#include "abb/net/connection_ref.hpp"
#include "connection.hpp"
#include "context.hpp"
#include "connector.hpp"
namespace abb {
namespace net {
class TcpClient:public IConnectorListener,IConnectionListener{
public:
	TcpClient(TcpClientManager::Listener* lis,TcpClientManager* mgr):lis_(lis),mgr_(mgr){

	}
	virtual ~TcpClient(){}
	virtual void L_Connection_OnMessage(Connection* conn,base::Buffer& buf){
		ConnectionRef* ref = (ConnectionRef*)conn->GetData();
		if(ref->IsClosed()){
			buf.Clear();
		}else{
			this->lis_->L_TcpClient_OnMessage(ref,buf);
		}
	}
	virtual void L_Connection_OnClose(Connection* conn,int error){
		ConnectionRef* ref = (ConnectionRef*)conn->GetData();
		int id = conn->GetId();
		conn->ShutDown();
		conn->Destroy();
		if(ref->TestAndSetNULL()){
			this->lis_->L_TcpClient_OnClose(ref,error);
		}
		ref->UnRef();
		delete this;
	}
	virtual void L_Connector_OnOpen(Connector* cotr,int fd){
		Connection* conn = new Connection(	mgr_->ctx_->GetFreeLoop(),fd,cotr->GetIpAddr(),cotr->GetIpAddr(),0);
		ConnectionRef* ref = new ConnectionRef(conn);
		conn->SetData(ref);
		conn->SetListener(this);
		conn->SetEnable(true);
		cotr->Destroy();
		lis_->L_TcpClient_OnConnection(ref);
	}
	virtual void L_Connector_OnClose(Connector* cotr,int error){
		cotr->Destroy();
		lis_->L_TcpClient_OnConnectFail(error);
		delete this;
	}
	TcpClientManager::Listener* lis_;
	TcpClientManager* mgr_;
};
TcpClientManager::TcpClientManager():ctx_(NULL) {
}
TcpClientManager::~TcpClientManager() {
}
void TcpClientManager::Init(int num_thread,bool run_curent_thread){
	ctx_ = new Context();
	ctx_->SetNumThread(num_thread);
	ctx_->SetRunCureentThread(run_curent_thread);
	ctx_->Init();
}
void TcpClientManager::Start(){
	ctx_->Start();
}
bool TcpClientManager::Connect(const IPAddr& addr,Listener*lis){
	Connector* ctor = new Connector(this->ctx_->GetFreeLoop());
	bool b = ctor->Connect(addr,NULL);
	if(b){
		ctor->SetListener(new TcpClient(lis,this));
	}else{
		ctor->Destroy();
	}
	return b;
}
} /* namespace net */
} /* namespace abb */

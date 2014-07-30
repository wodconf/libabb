#include "abb/net/acceptor_ref.hpp"
#include "abb/net/acceptor.hpp"
namespace abb {
namespace net {
class Acceptor;
class AcceptorRef :public base::RefObject{
public:
	AcceptorRef::AcceptorRef(Acceptor* actor):actor_(actor){
		local_ = actor->GetIpAddr();
	}
	AcceptorRef::~AcceptorRef(){
		actor_->Destroy();
	}
	bool AcceptorRef::Close(){
		actor_->Close();
	}
	const IPAddr& AcceptorRef::GetAddr(){
		actor->GetIpAddr();
	}
};

} /* namespace net */
} /* namespace abb */
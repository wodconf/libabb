#include "abb/net/acceptor_ref.hpp"
#include "abb/net/acceptor.hpp"
namespace abb {
namespace net {

	AcceptorRef::AcceptorRef(Acceptor* actor):actor_(actor){
		
	}
	AcceptorRef::~AcceptorRef(){
		actor_->Destroy();
	}
	bool AcceptorRef::Close(){
		actor_->Close();
	}
	const IPAddr& AcceptorRef::GetAddr(){
		return actor_->GetIpAddr();
	}

} /* namespace net */
} /* namespace abb */

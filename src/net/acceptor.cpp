

#include "acceptor.hpp"

namespace abb {
namespace net {

Acceptor::Acceptor() {

}

Acceptor::~Acceptor() {
}
bool Acceptor::Bind(const SockAddr& addr){

}
void Acceptor::SetEnable(bool benable){

}
void Acceptor::OnEvent(int event){
	SockAddr addr;
	int alen = sizeof(addr.sa);
	accept(fd_, &addr.sa.sa, &alen);
}
} /* namespace translate */
} /* namespace adcloud */

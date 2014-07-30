

#ifndef ABB_NET_EPOLL_POLLER_H_
#define ABB_NET_EPOLL_POLLER_H_

#include <sys/epoll.h>

namespace abb {
namespace net {
class IOEvent;
class Poller {
public:
	Poller();
	~Poller();
	void Poll(int timeout);
	void Apply(IOEvent* event);
private:
static const int num_revs = 64;
	int error_;
	int efd_;
	struct epoll_event revs_[num_revs];
};

} /* namespace base */
} /* namespace abb */

#endif /* POLLER_H_ */

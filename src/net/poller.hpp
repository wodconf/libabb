/*
 * poller.h
 *
 *  Created on: 2014Äê4ÔÂ25ÈÕ
 *      Author: goalworld
 */

#ifndef POLLER_H_
#define POLLER_H_

namespace abb {
namespace net {

class PollerAble;
enum{
	POLLER_READ = 0x01,
	POLLER_WRITE = 0x02
};
class Poller {
public:
	Poller();
	~Poller();
	void SetEvent(int fd,int event,PollerAble* arg,bool bneedadd);
	void Poll();
private:
	int error;
	int efd_;
};

} /* namespace base */
} /* namespace abb */

#endif /* POLLER_H_ */

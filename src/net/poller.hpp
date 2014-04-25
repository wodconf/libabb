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
class Poller {
public:
	Poller();
	~Poller();
	void SetRead(PollerAble* proxy);
	void UnSetRead(PollerAble* proxy);
	void SetWirte(PollerAble* proxy);
	void UnSetWrite(PollerAble* proxy);
private:
	int error;
	int efd_;
};

} /* namespace base */
} /* namespace abb */

#endif /* POLLER_H_ */

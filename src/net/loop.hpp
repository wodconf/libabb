/*
 * loop.hpp
 *
 *  Created on: 2014年4月27日
 *      Author: wd
 */

#ifndef LOOP_HPP_
#define LOOP_HPP_
#include "poller.hpp"
namespace abb {
namespace net{
class Loop {
public:
	Loop();
	virtual ~Loop();
	void loop();
	void Stop(){
		stop_ = true;
	}
	Poller& GetPoller(){
		return poller_;
	}
private:
	Poller poller_;
	bool stop_;
};
}
}
#endif /* LOOP_HPP_ */

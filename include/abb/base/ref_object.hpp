

#ifndef ABB_BASE_REF_OBJECT_HPP_
#define ABB_BASE_REF_OBJECT_HPP_

#include "log.hpp"

namespace abb {
namespace base {

class RefObject {
public:
	RefObject():ref_(1){}
	virtual ~RefObject(){};
	RefObject *Ref() {
		__sync_add_and_fetch(&ref_,1);
		return this;
	}
	void UnRef() {
		LOG(INFO) << "UnRef" << ref_;
		if (__sync_sub_and_fetch(&ref_,1) == 0)
			delete this;
	}
private:
	int ref_;
};

}
}

#endif

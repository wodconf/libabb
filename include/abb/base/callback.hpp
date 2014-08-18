#ifndef __ABB_BASE_CALL_BACK_HPP__
#define __ABB_BASE_CALL_BACK_HPP__

namespace abb{
		class CallBack{
		public:
			virtual ~CallBack(){};
			virtual void Call() = 0;
		};
}

#endif

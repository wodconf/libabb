#ifndef ABB_BASE_CALL_BACK_HPP_
#define ABB_BASE_CALL_BACK_HPP_

namespace abb{
		class CallBack{
		public:
			virtual ~CallBack(){};
			virtual void Call() = 0;
		};
}

#endif

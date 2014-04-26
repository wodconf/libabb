#ifndef ABB_NET_I_POLLER_EVENT_
#define ABB_NET_I_POLLER_EVENT_
namespace abb{
	namespace net{
		class IPollerEvent{
			public:
				virtual ~IPollerEvent(){}
				virtual PollerEvent_OnRead();
				virtual PollerEvent_OnWrite();
		};
	}
}
#endif
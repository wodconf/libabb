#ifndef ABB_NET_I_POLLER_EVENT_
#define ABB_NET_I_POLLER_EVENT_
namespace abb{
namespace net{
class IPollerEvent{
public:
	virtual ~IPollerEvent(){}
	virtual void PollerEvent_OnRead()=0;
	virtual void PollerEvent_OnWrite()=0;
};
}
}
#endif

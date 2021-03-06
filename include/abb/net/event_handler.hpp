

#ifndef ABB_NET_I_POLLER_EVENT_
#define ABB_NET_I_POLLER_EVENT_
namespace abb{
namespace net{
enum{
	IO_EVENT_READ = 0X01,
	IO_EVENT_WRITE = 0X02,
	IO_EVENT_ERROR = 0X04
};
class IEventHandler{
public:
	virtual ~IEventHandler(){}
	virtual void HandleEvent(int event) = 0;
};
}
}
#endif



#ifndef ABB_HPP_
#define ABB_HPP_
#include "net/i_protocol.hpp"
#include "net/context_option.hpp"
namespace abb{
namespace net{
class Context;
}
extern net::Context* NewContext(const net::ContextOption& option,net::IProtocolFactory* fac);
extern void RunContext(net::Context* ctx);
void DeleteContext(net::Context* ctx);
}
#endif /* ABB_HPP_ */



#ifndef ABB_HPP_
#define ABB_HPP_
#include "net/context_option.hpp"
namespace abb{
namespace net{
class Context;
}
extern net::Context* NewContext(const net::ContextOption& option);
extern void RunContext(net::Context* ctx,bool run_cur_thread = false);
void DeleteContext(net::Context* ctx);
}
#endif /* ABB_HPP_ */



#ifndef ABB_HPP_
#define ABB_HPP_

namespace abb{
namespace net{
class Context;
}
extern net::Context* NewContext(int num_pool = 1,int num_dis = 4);
extern void RunContext(net::Context* ctx,bool run_cur_thread = false);
void DeleteContext(net::Context* ctx);
}
#endif /* ABB_HPP_ */

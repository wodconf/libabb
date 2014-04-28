/*
 * abb.hpp
 *
 *  Created on: 2014年4月27日
 *      Author: wd
 */

#ifndef ABB_HPP_
#define ABB_HPP_

namespace abb{
namespace net{
class Context;
}
net::Context* NewContext(int num_pool = 1,int num_dis = 4);
void RunContext(net::Context* ctx,bool run_cur_thread = false);
}
#endif /* ABB_HPP_ */

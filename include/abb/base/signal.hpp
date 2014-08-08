

#ifndef __ADCLOUD_COMMON_SIGNAL_HPP__
#define __ADCLOUD_COMMON_SIGNAL_HPP__

#include <signal.h>
#include <string>

namespace abb {

extern std::string signal_mask_to_str();

extern void block_signals(const int *siglist, sigset_t *old_sigset);

extern void restore_sigset(const sigset_t *old_sigset);


extern void unblock_all_signals(sigset_t *old_sigset);


}
#endif


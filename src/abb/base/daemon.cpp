#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "abb/base/daemon.hpp"
#include "abb/base/log.hpp"
#include <stdlib.h>
namespace abb{
int Daemon()
{
    int  fd;
    switch (fork()) {
    case -1:
        LOG(ERROR) << "fork() failed" << strerror(errno);
        return -errno;
    case 0:
        break;
    default:
        exit(0);
    }

    //pid_t pid = getpid();

    if (setsid() == -1) {
        LOG(ERROR) << "setsid() failed" << strerror(errno);
        return -errno;
    }

    umask(0);
    chdir("/");

    fd = open("/dev/null", O_RDWR);
    if (fd == -1) {
        LOG(ERROR) << "open(\"/dev/null\") failed" << strerror(errno);
        return -errno;
    }

    if (dup2(fd, STDIN_FILENO) == -1) {
        LOG(ERROR) << "dup2(STDIN) failed" << strerror(errno);
        return -errno;;
    }

    if (dup2(fd, STDOUT_FILENO) == -1) {
        LOG(ERROR) << "dup2(STDOUT) failed" << strerror(errno);
        return -errno;
    }

    if (fd > STDERR_FILENO) {
        if (close(fd) == -1) {
            LOG(ERROR) << "close() failed" << strerror(errno);
            return -errno;
        }
    }

    return 0;
}

}

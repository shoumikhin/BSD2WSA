#include <sys/types.h>

namespace redirected
{
//==================================================================================================
int socket(int domain, int type, int protocol);
int getsockopt(int sockfd, int level, int option_name, void *option_value, socklen_t *option_len);
int setsockopt(int sockfd, int level, int option_name, void const *option_value, socklen_t option_len);
int bind(int sockfd, struct sockaddr const *address, socklen_t addrlen);
int connect(int sockfd, sockaddr const *address, socklen_t addrlen);
int poll(struct pollfd *fds, nfds_t nfds, int timeout);
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, struct timeval *timeout);
int getsockname(int sockfd, struct sockaddr *address, socklen_t *addrlen);
int getpeername(int sockfd, struct sockaddr *address, socklen_t *addrlen);
int getaddrinfo(char const *node, char const *service, struct addrinfo const *hints, struct addrinfo **res);
ssize_t recv(int sockfd, void *buffer, size_t length, int flags);
ssize_t recvfrom(int sockfd, void *buffer, size_t length, int flags, struct sockaddr *from, socklen_t *fromlen);
ssize_t send(int sockfd, void const *buffer, size_t length, int flags);
ssize_t sendto(int sockfd, void const *buffer, size_t length, int flags, struct sockaddr const *to, socklen_t tolen);
int fcntl(int fd, int cmd, ...);
int ioctl(int fd, int cmd, ...);
int shutdown(int sockfd, int how);
int close(int fd);
//==================================================================================================
}
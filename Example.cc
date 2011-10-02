#include "Example.h"

#include "BSD2WSA.h"

#include <cerrno>
#include <sys/socket.h>
#include <poll.h>

/*
 Let's imagine we've got some kind of synchronous RPC to Windows with a set of proper packet types
 named RPC_FUNCTION_REQUEST or RPC_FUNCTION_RESPONSE, where FUNCTION is a name of some redirected
 library call.
 Also we've got all the real library network calls redirected to our implementations below instead
 of originals.
*/

namespace redirected
{
BSD2WSA::SocketsStorage g_sockets;
//==================================================================================================
//int socket(int domain, int type, int protocol)
//{
//    return ::socket(domain, type, protocol);
//}
int socket(int domain, int type, int protocol)
{
    int ret = reinterpret_cast<int>(INVALID_SOCKET);
        
    errno = 0;
    
    RPC_SOCKET_REQUEST request;
    RPC_SOCKET_RESPONSE response;
    
    request.af = BSD2WSA::domain2WSAdomain(domain);
    request.type = type;
    request.protocol = protocol;
        
    if (response = static_cast<RPC_SOCKET_RESPONSE>(sendSyncRequest(request)))
    {
        ret = g_sockets.addSocket(response.socket);
        
        if (INVALID_SOCKET == response.socket)
            errno = SD2WSA::WSA2errno(response->errno);
    }
    else
        errno = EAGAIN;

    return ret;
}
//--------------------------------------------------------------------------------------------------
//int getsockopt(int sockfd, int level, int option_name, void *option_value, socklen_t *option_len)
//{
//    return ::getsockopt(sockfd, level, option_name, option_value, option_len);
//}
int getsockopt(int sockfd, int level, int option_name, void *option_value, socklen_t *option_len)
{
    int ret = reinterpret_cast<int>(INVALID_SOCKET);
    
    errno = 0;
    
    RPC_GETSOCKOPT_REQUEST request;
    RPC_GETSOCKOPT_RESPONSE response;

    request.s = g_sockets.convert(sockfd);
    request.level = level;
    request.optname = option_name;
    request.optlen = option_len ? *option_len : 0;
    request.optval = option_value ? *option_value : 0;
    BSD2WSA::sockopt2WSA(&request.level, &request.optname);
    
    if (response = static_cast<RPC_GETSOCKOPT_RESPONSE>(sendSyncRequest(request)))
    {
        ret = response.ret;
            
        if (SOCKET_ERROR == ret)
            errno = BSD2WSA::WSA2errno(response.errno);
        else
        {
            if (option_len)
                *option_len = response.optlen;
                
            if (option_value)
                ::memcpy(option_value, response.optval, response.optlen);
        }
    }
    else
        errno = EAGAIN;
        
    return ret;
}
//--------------------------------------------------------------------------------------------------
//int setsockopt(int sockfd, int level, int option_name, void const *option_value, socklen_t option_len)
//{
//    return ::setsockopt(sockfd, level, option_name, option_value, option_len);
//}
int setsockopt(int sockfd, int level, int option_name, void const *option_value, socklen_t option_len)
{
    int ret = reinterpret_cast<int>(INVALID_SOCKET);
    
    errno = 0;
    
    RPC_SETSOCKOPT_REQUEST request;
    RPC_SETSOCKOPT_RESPONSE response;

    request.s = g_sockets.convert(sockfd);
    request.level = level;
    request.optname = option_name;
    request.optlen = option_len;
    request.optval = option_value ? *option_value : 0;
    BSD2WSA::sockopt2WSA(&request.level, &request.optname);
        
    if (response = static_cast<RPC_SETSOCKOPT_RESPONSE>(sendSyncRequest(request)))
    {
        ret = response.ret;
        
        if (SOCKET_ERROR == ret)
            errno = BSD2WSA::WSA2errno(response.errno);
    }
    else
        errno = EAGAIN;
        
    return ret;
}
//--------------------------------------------------------------------------------------------------
//int bind(int sockfd, struct sockaddr const *address, socklen_t addrlen)
//{
//    return ::bind(sockfd, address, addrlen);
//}
int bind(int sockfd, struct sockaddr const *address, socklen_t addrlen)
{
    int ret = reinterpret_cast<int>(INVALID_SOCKET);
    
    errno = 0;
    
    RPC_BIND_REQUEST request;
    RPC_BIND_RESPONSE response;
    
    request.s = g_sockets.convert(sockfd);
    request.namelen = addrlen;
    request.name = address ? *address : 0; 
    
    if (response = static_cast<RPC_BIND_RESPONSE>(sendSyncRequest(request)))
    {
        ret = response.ret;
        
        if (SOCKET_ERROR == ret)
            errno = BSD2WSA::WSA2errno(response.errno);
    }
    else
        errno = EAGAIN;
    
    return ret;
}
//--------------------------------------------------------------------------------------------------
//int connect(int sockfd, struct sockaddr const *address, socklen_t addrlen)
//{
//    return ::connect(sockfd, address, addrlen);
//}
int connect(int sockfd, struct sockaddr const *address, socklen_t addrlen)
{
    int ret = reinterpret_cast<int>(INVALID_SOCKET);
    
    errno = 0;
    
    RPC_CONNECT_REQUEST request;
    RPC_CONNECT_RESPONSE response;
    
    request.s = g_sockets.convert(sockfd);
    request.namelen = addrlen;
    request.name = address ? *address : 0; 
    
    if (response = static_cast<RPC_CONNECT_RESPONSE>(sendSyncRequest(request)))
    {
        ret = response.ret;
        
        if (SOCKET_ERROR == ret)
            errno = BSD2WSA::WSA2errno(response.errno);
    }
    else
        errno = EAGAIN;
    
    return ret;
}
//--------------------------------------------------------------------------------------------------
//int poll(struct pollfd *fds, nfds_t nfds, int timeout)
//{
//    return ::poll(fds, nfds, timeout);
//}
int poll(struct pollfd *fds, nfds_t nfds, int timeout)
{
    int ret = reinterpret_cast<int>(INVALID_SOCKET);
    
    errno = 0;
    
    RPC_POLL_REQUEST request;
    RPC_POLL_RESPONSE response;
    
    request.nfds = nfds;
    request.timeout = timeout;
        
    for (nfds_t i = 0; i < nfds; ++i)
    {
        if (g_sockets.hasSocket(fds[i].fd))
        {
            request.fds[i].fd = g_sockets.getSocket(fds[i].fd);
            request.fds[i].events = BSD2WSA::poll2WSAPoll(fds[i].events);
        }
        else
            request.fds[i].events = 0;
                
        request.fds[i].revents = 0;
    }
        
    if (response = static_cast<RPC_POLL_RESPONSE>(sendSyncRequest(request)))
    {
        ret = response.ret;
        
        if (SOCKET_ERROR == ret)
            errno = BSD2WSA::WSA2errno(response.errno);
        else
            for (nfds_t i = 0; i < nfds; ++i)
                if (g_sockets.hasSocket(fds[i].fd))
                    fds[i].revents = BSD2WSA::WSAPoll2poll(response.fds[i].revents);
    }
    else
        errno = EAGAIN;
       
    return ret;
}
//--------------------------------------------------------------------------------------------------
//int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, struct timeval *timeout)
//{
//    return ::select(nfds, readfds, writefds, errorfds, timeout);
//}
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, struct timeval *timeout)
{
    int ret = reinterpret_cast<int>(INVALID_SOCKET);
    
    errno = 0;

    RPC_SELECT_REQUEST request;
    RPC_SELECT_RESPONSE response;
    
    request.nfds = nfds;

    if (readfds)
        BSD2WSA::select2WSASelect(readfds, &request.readfds, s_sockets);
       
    if (writefds)
        BSD2WSA::select2WSASelect(writefds, &request.writefds, s_sockets);
        
    if (errorfds)
        BSD2WSA::select2WSASelect(errorfds, &request.exceptfds, s_sockets);
        
    if (timeout)
        ::memcpy(&request.timeout, timeout, sizeof(timeval));
        
    if (response = static_cast<RPC_SELECT_RESPONSE>(sendSyncRequest(request)))
    {
        ret = response.ret;
        
        if (SOCKET_ERROR == ret)
            errno = BSD2WSA::WSA2errno(response.errno);
        else
        {
            if (readfds)
                BSD2WSA::WSASelect2select(&response.readfds, readfds, s_sockets);
                
            if (writefds)
                BSD2WSA::WSASelect2select(&response.writefds, writefds, s_sockets);
                
            if (errorfds)
                BSD2WSA::WSASelect2select(&response.exceptfds, errorfds, s_sockets);
        }
    }
       
    return ret;
}
//--------------------------------------------------------------------------------------------------
//int getsockname(int sockfd, struct sockaddr *address, socklen_t *addrlen)
//{
//    return ::getsockname(sockfd, address, addrlen);
//}
int getsockname(int sockfd, struct sockaddr *address, socklen_t *addrlen)
{
    int ret = reinterpret_cast<int>(INVALID_SOCKET);
    
    errno = 0;
    
    RPC_GETSOCKNAME_REQUEST request;
    RPC_GETSOCKNAME_RESPONSE response;
    
    request.s = g_sockets.convert(sockfd);
    request.namelen = addrlen ? *addrlen : 0;
    request.name = address ? *address : 0; 
           
    if (response = static_cast<RPC_GETSOCKNAME_RESPONSE>(sendSyncRequest(request)))
    {
        ret = response.ret;
        
        if (SOCKET_ERROR == ret)
            errno = BSD2WSA::WSA2errno(response.errno);
        else
        {
            if (addrlen)
                *addrlen = response.namelen;
            
            if (address)
                ::memcpy(address, response.name, response.namelen);
        }
    }
    else
        errno = EAGAIN;
    
    return ret;
}
//--------------------------------------------------------------------------------------------------    
//int getpeername(int sockfd, struct sockaddr *address, socklen_t *addrlen)
//{    
//    return ::getpeername(sockfd, address, addrlen);
//}
int getpeername(int sockfd, struct sockaddr *address, socklen_t *addrlen)
{
    int ret = reinterpret_cast<int>(INVALID_SOCKET);
    
    errno = 0;

    RPC_GETPEERNAME_REQUEST request;
    RPC_GETPEERNAME_RESPONSE response;
    
    request.s = g_sockets.convert(sockfd);
    request.namelen = addrlen ? *addrlen : 0;
    request.name = address ? *address : 0; 
    
    if (response = static_cast<RPC_GETPEERNAME_RESPONSE>(sendSyncRequest(request)))
    {
        ret = response.ret;
        
        if (SOCKET_ERROR == ret)
            errno = BSD2WSA::WSA2errno(response.errno);
        else
        {
            if (addrlen)
                *addrlen = response.namelen;
            
            if (address)
                ::memcpy(address, response.name, response.namelen);
        }
    }
    else
        errno = EAGAIN;
    
    return ret;
}
//--------------------------------------------------------------------------------------------------    
//int getaddrinfo(char const *node, char const *service, struct addrinfo const *hints, struct addrinfo **res)
//{
//    return ::getaddrinfo(node, service, hints, res);
//}
int getaddrinfo(char const *node, char const *service, struct addrinfo const *hints, struct addrinfo **res)
{
    int ret = reinterpret_cast<int>(INVALID_SOCKET);
    
    errno = 0;
   
    RPC_GETADDRINFO_REQUEST request;
    RPC_GETADDRINFO_RESPONSE response;
        
    request.node = node;
    request.service = service;
    request.ai_flags = hints->ai_flags;
    request.ai_family = hints->ai_family;
    request.ai_socktype = hints->ai_socktype;
    request.ai_protocol = hints->ai_protocol;
    request.res = res;
       
    if (response = static_cast<RPC_GETADDRINFO_RESPONSE>(sendSyncRequest(request)))
    {
        ret = response.ret;
        
        if (SOCKET_ERROR == ret)
            errno = BSD2WSA::WSA2errno(response.errno);
        else
        {
            struct addrinfo *q = 0, *prev = 0;
            
            *res = 0;
            
            for (PADDRINFOA p = response.res; p; p = p->ai_next)
            {               
                q = (struct addrinfo *)::malloc(sizeof(struct addrinfo));
                ::memcpy(q, p, sizeof(struct addrinfo));
                
                if (p->ai_addr)
                {
                    q->ai_addr = (struct sockaddr *)::malloc(sizeof(struct sockaddr));
                    ::memcpy(q->ai_addr, p->ai_addr, sizeof(struct sockaddr) > p->ai_addrlen ? p->ai_addrlen : sizeof(struct sockaddr));
                }
                else
                {
                    q->ai_addr = 0;
                    q->ai_addrlen = 0;
                }
                
                if (p->ai_canonname)
                {
                    size_t len = ::strlen(p->ai_canonname);
                    
                    len = len > 0x100 ? 0x100 : len;  //if there was an error during transferring
                    
                    q->ai_canonname = (char *)::malloc(len + 1);
                    ::memcpy(q->ai_canonname, p->ai_canonname, len);
                    q->ai_canonname[len] = 0;
                }
                else
                    q->ai_canonname = 0;
                
                q->ai_next = 0;
                
                if (!*res)  //only for the first time
                    *res = q;
                
                if (prev)
                    prev->ai_next = q;
                
                prev = q;
            }
        }
    }
    else
        errno = EAGAIN;
        
        return ret;
}
//--------------------------------------------------------------------------------------------------
//ssize_t recv(int sockfd, void *buffer, size_t length, int flags)
//{
//    return ::recv(sockfd, buffer, length, flags);
//}
ssize_t recv(int sockfd, void *buffer, size_t length, int flags)
{
    int ret = reinterpret_cast<int>(INVALID_SOCKET);
    
    errno = 0;
   
    RPC_RECV_REQUEST request;
    RPC_RECV_RESPONSE response;
    
    request.s = g_sockets.convert(sockfd);
    request.len = length;
    request.flags = BSD2WSA::msgFlags2WSAmsgFlags(flags);
    request.buf = buffer;
        
    if (response = static_cast<RPC_RECV_RESPONSE>(sendSyncRequest(request)))
    {
        ret = response.ret;
        
        if (SOCKET_ERROR == ret)
            errno = BSD2WSA::WSA2errno(response.errno);
        else
        {
            if (buffer)
                ::memcpy(buffer, response.buf, ret);
        }
    }
    else
        errno = EAGAIN;
    
    return ret;
}
//--------------------------------------------------------------------------------------------------
//ssize_t recvfrom(int sockfd, void *buffer, size_t length, int flags, struct sockaddr *from, socklen_t *fromlen)
//{
//    return ::recvfrom(sockfd, buffer, length, flags, from, fromlen);
//}
ssize_t recvfrom(int sockfd, void *buffer, size_t length, int flags, struct sockaddr *from, socklen_t *fromlen)
{
    int ret = reinterpret_cast<int>(INVALID_SOCKET);
    
    errno = 0;
        
    RPC_RECVFROM_REQUEST request;
    RPC_RECVFROM_RESPONSE response;
    
    request.s = g_sockets.convert(sockfd);
    request.len = length;
    request.flags = BSD2WSA::msgFlags2WSAmsgFlags(flags);
    request.buf = buffer;
    request.from = from;
    request.fromlen = fromlen ? *fromlen : 0;
       
    if (response = static_cast<RPC_RECVFROM_RESPONSE>(sendSyncRequest(request)))
    {
        ret = response.ret;
        
        if (SOCKET_ERROR == ret)
            errno = BSD2WSA::WSA2errno(response.errno);
        else
        {           
            if (fromlen)
                *fromlen = response.fromlen;
            
            if (from)
                ::memcpy(from, response.from, response.fromlen);
            
            if (buffer)
                ::memcpy(buffer, response.buf, length);
        }
    }
    else
        errno = EAGAIN;
    
    return ret;
}
//--------------------------------------------------------------------------------------------------
//ssize_t send(int sockfd, const void *buffer, size_t length, int flags)
//{
//    return ::send(sockfd, buffer, length, flags);
//}
ssize_t send(int sockfd, const void *buffer, size_t length, int flags)
{
    int ret = reinterpret_cast<int>(INVALID_SOCKET);
    
    errno = 0;
    
    RPC_SEND_REQUEST request;
    RPC_SEND_RESPONSE response;
    
    request.s = g_sockets.convert(sockfd);
    request.len = length;
    request.flags = BSD2WSA::msgFlags2WSAmsgFlags(flags);
    request.buf = buffer;
    
    if (response = static_cast<RPC_SEND_RESPONSE>(sendSyncRequest(request)))
    {
        ret = response.ret;
        
        if (SOCKET_ERROR == ret)
            errno = BSD2WSA::WSA2errno(response.errno);
    }
    else
        errno = EAGAIN;
        
    return ret;
}
//--------------------------------------------------------------------------------------------------
//ssize_t sendto(int sockfd, void const *buffer, size_t length, int flags, struct sockaddr const *to, socklen_t tolen)
//{
//    return ::sendto(sockfd, buffer, length, flags, to, tolen);
//}
ssize_t sendto(int sockfd, void const *buffer, size_t length, int flags, struct sockaddr const *to, socklen_t tolen)
{
    int ret = reinterpret_cast<int>(INVALID_SOCKET);
    
    errno = 0;
    
    RPC_SENDTO_REQUEST request;
    RPC_SENDTO_RESPONSE response;
    
    request.s = g_sockets.convert(sockfd);
    request.len = length;
    request.flags = BSD2WSA::msgFlags2WSAmsgFlags(flags);
    request.buf = buffer;
    request.tolen = tolen;
    request.to = to;
        
    if (response = static_cast<RPC_SENDTO_RESPONSE>(sendSyncRequest(request)))
    {
        ret = response.ret;
        
        if (SOCKET_ERROR == ret)
            errno = BSD2WSA::WSA2errno(response.errno);
    }
    else
        errno = EAGAIN;    
    
    return ret;
} 
//--------------------------------------------------------------------------------------------------
//int fcntl(int fd, int cmd, ...)
//{
//    int st;
//    struct stat buf;
//    va_list ap;
//    
//    st = ::fstat(fd, &buf);    
//    va_start(ap, cmd);
//    
//    switch (cmd)
//    {
//        case F_DUPFD :
//        case F_SETFD :
//        case F_SETFL :
//        case F_SETOWN :
//        {
//            int integer = va_arg(ap, int);
//            
//            va_end(ap);
//            
//            if (!st && S_ISSOCK(buf.st_mode))
//                return ::fcntl(fd, cmd, integer);
//        }
//            
//        case F_GETLK :
//        case F_SETLK :
//        case F_SETLKW :
//        {
//            struct fmutex *mutex32 = va_arg(ap, struct fmutex *);
//            
//            va_end(ap);
//            
//            if (!st && S_ISSOCK(buf.st_mode))
//                return ::fcntl(fd, cmd, mutex32);
//        }
//    }
//    
//    if (!st && S_ISSOCK(buf.st_mode))
//        return ::fcntl(fd, cmd);
//}
int fcntl(int fd, int cmd, ...)
{
    int ret = reinterpret_cast<int>(INVALID_SOCKET);
    
    errno = 0;
    
    va_list ap;
        
    va_start(ap, cmd);
        
    switch (cmd)
    {
        case F_DUPFD :
        case F_SETFD :
        case F_SETFL :
        case F_SETOWN :
        {
            int integer = va_arg(ap, int);
                
            va_end(ap);

            if (g_sockets.hasSocket(fd))
            {
                errno = 0;

                if (F_SETFL == cmd)  //let's support only this feature
                {
                    RPC_IOCTLSOCKET_REQUEST request;
                    RPC_IOCTLSOCKET_RESPONSE response;
 
                    request.s = g_sockets.convert(fd);
                    request.cmd = _FIONBIO;  
                    request.argp = integer;
                            
                    if (response = static_cast<RPC_IOCTLSOCKET_RESPONSE>(sendSyncRequest(request)))
                    {
                        ret = response.ret;
                        
                        if (SOCKET_ERROR == ret)
                            errno = BSD2WSA::WSA2errno(response.errno);
                    }
                    else
                        errno = EAGAIN;  
                }
            }
            else
            {
                errno = 0;
                ret = ::fcntl(fd, cmd, integer);
            }
                
            return ret;
        }
                
        case F_GETLK :
        case F_SETLK :
        case F_SETLKW :
        {
            struct fmutex *mutex32 = va_arg(ap, struct fmutex *);
                
            va_end(ap);

            if (g_sockets.hasSocket(fd))
            {
                errno = 0;
                //UNIMPLEMENTED
            }
            else
            {
                errno = 0;
                ret = ::fcntl(fd, cmd, mutex32);
            }
                
            return ret;
        }
    }

    if (g_sockets.hasSocket(fd))
    {
        errno = 0;

        if (F_GETFL == cmd)
            ret = O_RDWR;
    }
    else
    {
        errno = 0;
        ret = ::fcntl(fd, cmd);
    }
        
    return ret;
}
//--------------------------------------------------------------------------------------------------
//int ioctl(int fd, int cmd, ...)
//{
//    int st;
//    struct stat buf;
//    
//    st = ::fstat(fd, &buf);
//    
//    va_list ap;
//    
//    va_start(ap, cmd);
//    
//    switch (cmd)
//    {
//        case SIOCATMARK :
//        case SIOCSPGRP :
//        case SIOCGPGRP :
//        {
//            int integer = va_arg(ap, int);
//            
//            va_end(ap);
//          
//            if (!st && S_ISSOCK(buf.st_mode)) 
//                return ::ioctl(fd, cmd, integer);
//        }
//            
//        case SIOCGIFCONF :
//        case SIOCSIFFLAGS :
//        case SIOCGIFFLAGS :
//        case SIOCSIFMETRIC :
//        case SIOCGIFMETRIC :
//        case SIOCADDMULTI :
//        case SIOCDELMULTI :
//        case SIOCSIFMTU :
//        case SIOCGIFMTU :
//        {
//            caddr_t *caddr = va_arg(ap, caddr_t *);
//            
//            va_end(ap);
//            
//            if (!st && S_ISSOCK(buf.st_mode)) 
//                return ::ioctl(fd, cmd, caddr);
//        }
//            
//        case SIOCSIFADDR :
//        case SIOCDIFADDR :
//        case SIOCGIFADDR :
//        case SIOCSIFDSTADDR :
//        case SIOCGIFDSTADDR :
//        case SIOCSIFNETMASK :
//        case SIOCGIFNETMASK :
//        case SIOCSIFBRDADDR :
//        case SIOCGIFBRDADDR :
//        {
//            caddr_t *caddr = va_arg(ap, caddr_t *);
//            size_t size = va_arg(ap, size_t);
//            
//            va_end(ap);
//            
//            if (!st && S_ISSOCK(buf.st_mode)) 
//                return ::ioctl(fd, cmd, caddr, size);
//        }            
//    }
//}
int ioctl(int fd, int cmd, ...)
{
    int ret = reinterpret_cast<int>(INVALID_SOCKET);
    
    errno = 0;

    st = ::fstat(fd, &buf);

    va_list ap;
        
    va_start(ap, cmd);
        
    switch (cmd)
    {
        case SIOCATMARK :
        case SIOCSPGRP :
        case SIOCGPGRP :
        {
            int integer = va_arg(ap, int);
            
            va_end(ap);

            if (g_sockets.hasSocket(fd))
            {
                errno = 0;
                //UNIMPLEMENTED
            }
            else
            {
                errno = 0;
                ret = ::ioctl(fd, cmd, integer);
            }
                
            return ret;
        }
    
        case SIOCGIFCONF :
        case SIOCSIFFLAGS :
        case SIOCGIFFLAGS :
        case SIOCSIFMETRIC :
        case SIOCGIFMETRIC :
        case SIOCADDMULTI :
        case SIOCDELMULTI :
        case SIOCSIFMTU :
        case SIOCGIFMTU :
        {
            caddr_t *caddr = va_arg(ap, caddr_t *);
            
            va_end(ap);

            if (isSocket(fd))
            {
                errno = 0;
                //UNIMPLEMENTED                
            }
            else
            {
                errno = 0;
                ret = ::ioctl(fd, cmd, caddr);
            }
                
            return ret;
        }
                
        case SIOCSIFADDR :
        case SIOCDIFADDR :
        case SIOCGIFADDR :
        case SIOCSIFDSTADDR :
        case SIOCGIFDSTADDR :
        case SIOCSIFNETMASK :
        case SIOCGIFNETMASK :
        case SIOCSIFBRDADDR :
        case SIOCGIFBRDADDR :
        {
            caddr_t *caddr = va_arg(ap, caddr_t *);
            size_t size = va_arg(ap, size_t);
            
            va_end(ap);

            if (g_sockets.hasSocket(fd))
            {
                errno = 0;
                //UNIMPLEMENTED
            }
            else
            {
                errno = 0;
                ret = ::ioctl(fd, cmd, caddr, size);
            }
            
            return ret;
        }
            
        default :
            //UNKNOWN COMMAND
    }
    
    return ret;
}
//--------------------------------------------------------------------------------------------------
//int shutdown(int sockfd, int how)
//{
//    return ::shutdown(sockfd, how);
//}
int shutdown(int sockfd, int how)
{
    int ret = reinterpret_cast<int>(INVALID_SOCKET);
    
    errno = 0;
   
    RPC_SHUTDOWN_REQUEST request;
    RPC_SHUTDOWN_RESPONSE response;
        
    request.s = g_sockets.convert(sockfd);
    request.how = how;
        
    if (response = static_cast<RPC_SHUTDOWN_RESPONSE>(sendSyncRequest(request)))
    {
        ret = response.ret;
        
        if (SOCKET_ERROR == ret)
            errno = BSD2WSA::WSA2errno(response.errno);
    }
    else
        errno = EAGAIN;    
    
    return ret;
}
//--------------------------------------------------------------------------------------------------
//int close(int fd)
//{
//    int st;
//    struct stat buf;
//    
//    st = ::fstat(fd, &buf);
//    
//    if (!st && S_ISSOCK(buf.st_mode))
//        return ::close(fd);
//}
int close(int fd)
{
    int ret = reinterpret_cast<int>(INVALID_SOCKET);
    
    errno = 0;

    if (g_sockets.hasSocket(fd))
    {
        errno = 0;
        
        RPC_CLOSESOCKET_REQUEST request;
        RPC_CLOSESOCKET_RESPONSE response;

        request.s = g_sockets.convert(fd);

        if (response = static_cast<RPC_CLOSESOCKET_RESPONSE>(sendSyncRequest(request)))
        {
            ret = response.ret;
            
            if (SOCKET_ERROR == ret)
                errno = BSD2WSA::WSA2errno(response.errno);
        }
        else
            errno = EAGAIN;  
    }
    else
    {
        errno = 0;
        ret = ::close(fd);
    }
        
    return ret;
}    
//==================================================================================================
}

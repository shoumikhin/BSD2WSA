#include "BSD2WSA.h"

#include <cerrno>
#include <sys/socket.h>
#include <poll.h>

namespace BSD2WSA
{
//==================================================================================================
bool SocketsStorage::hasSocket(int const fd)
{
    sockets_map::iterator i = map_.find(fd);
    
    return map_.end() != i ? true : false;
}
//--------------------------------------------------------------------------------------------------
int SocketsStorage::addSocket(SOCKET const handle)
{
    if (INVALID_SOCKET == handle)
        return reinterpret_cast<int>(INVALID_SOCKET);

    static int const min = FD_SETSIZE - FD_SETSIZE / 4;  //big enought to avoid file descriptors conflict but less than FD_SETSIZE
    static int const max = FD_SETSIZE;
    
    for (int fd = min; fd < max; ++fd)
    {
        sockets_map::iterator i = map_.find(fd);
            
            if (map_.end() == i)
            {
                map_[fd] = handle;
                
                return fd;
            }
        }
    }
      
    return reinterpret_cast<int>(INVALID_SOCKET);
}
//--------------------------------------------------------------------------------------------------
void SocketsStorage::removeSocket(int const fd)
{
    s_sockets.erase(fd);
}
//--------------------------------------------------------------------------------------------------
SOCKET SocketsStorage::convert(int const fd)
{
    return hasSocket(fd) ? map_[fd] : INVALID_SOCKET;
}
//--------------------------------------------------------------------------------------------------
int SocketsStorage::convert(SOCKET const handle)
{
    sockets_map::iterator i = map_.begin();
    sockets_map::const_iterator end = map_.end();
    
    while (end != i)
        if (socket == (*i).second)
            return (*i).first;
    
    return reinterpret_cast<int>(INVALID_SOCKET);
}
//--------------------------------------------------------------------------------------------------
bool FD_IsSet(SOCKET fd, fd_set_ *set)
{
    for  (int i = set->fd_count; i; --i)
        if (set->fd_array[i] == fd)
            return 1;
    
    return 0;
}
//--------------------------------------------------------------------------------------------------
#define FD_ZERO_(set) (((fd_set_ *)(set))->fd_count=0)
#define FD_SET_(fd, set)\
do\
{ \
if (((fd_set_ *)(set))->fd_count < FD_SETSIZE_)\
((fd_set_ *)(set))->fd_array[((fd_set_ *)(set))->fd_count++]=(fd);\
}\
while (0)
#define FD_ISSET_(fd, set) FD_IsSet((SOCKET)(fd), (fd_set_ *)(set))

void select2WSASelect(fd_set *bsd, fd_set_ *win, sockets_map &sockets)
{
    sockets_map::iterator i = sockets.begin();
    sockets_map::const_iterator end = sockets.end();
    
    FD_ZERO_(win);
    
    while (end != i)
    {
        if (FD_ISSET((*i).first, bsd))
            FD_SET_((*i).second, win);
        
        ++i;
    }
}
//--------------------------------------------------------------------------------------------------
void WSASelect2select(fd_set_ *win, fd_set *bsd, sockets_map &sockets)
{
    sockets_map::iterator i = sockets.begin();
    sockets_map::const_iterator end = sockets.end();
    
    FD_ZERO(bsd);
    
    while (end != i)
    {
        if (FD_ISSET_((*i).second, win))
            FD_SET((*i).first, bsd);
        
        ++i;
    }
}

#undef FD_ZERO_
#undef FD_SET_
#undef FD_ISSET_
//--------------------------------------------------------------------------------------------------
int WSA2errno(int WSA)
{
    int ret = 0;
    
    switch (WSA)
    {
        case 0 :
            
            break;
            
        case WSANOTINITIALISED :
        case WSAEINVAL :
        case WSAEINVALIDPROVIDER :
        case WSAEINVALIDPROCTABLE :
            
            ret = EINVAL;
            
            break;
            
        case WSAENETRESET :
        case WSAECONNABORTED :
        case WSAEDISCON :
            
            ret = ECONNREFUSED;
            
            break;
            
        case WSAEPROTONOSUPPORT :
        case WSAEPROTOTYPE :
            
            ret = EPROTONOSUPPORT;
            
            break;
            
        case WSAEADDRNOTAVAIL :
            
            ret = EADDRNOTAVAIL;
            
            break;
            
        case WSAEDESTADDRREQ :
            
            ret = EDESTADDRREQ;
            
            break;
            
        case WSAESOCKTNOSUPPORT :
            
            ret = ESOCKTNOSUPPORT;
            
            break;
            
        case WSAESHUTDOWN :
            
            ret = EPIPE;
            
            break;
            
        case WSAENETDOWN :
            
            ret = ENOSR;
            
            break;
            
        case WSAEMSGSIZE :
            
            ret = EMSGSIZE;
            
            break;
            
        case WSAEINPROGRESS :
            
            ret = EINPROGRESS;
            
            break;
            
        case WSAEFAULT :
            
            ret = EFAULT;
            
            break;
            
        case WSAEADDRINUSE :
            
            ret = EADDRINUSE;
            
            break;
            
        case WSAENOPROTOOPT :
            
            ret = ENOPROTOOPT;
            
            break;
            
        case WSAEALREADY :
            
            ret = EALREADY;
            
            break;
            
        case WSAEAFNOSUPPORT :
            
            ret = EAFNOSUPPORT;
            
            break;
            
        case WSAEHOSTUNREACH :
            
            ret = EHOSTUNREACH;
            
            break;
            
        case WSAENETUNREACH :
            
            ret = ENETUNREACH;
            
            break;
            
        case WSAEISCONN :
            
            ret = EISCONN;
            
            break;
            
        case WSAEMFILE :
            
            ret = EMFILE;
            
            break;
            
        case WSAENOBUFS :
            
            ret = ENOBUFS;
            
            break;
            
        case WSAECONNREFUSED :
            
            ret = ECONNREFUSED;
            
            break;
            
        case WSAEINTR :
            
            ret = EINTR;
            
            break;
            
        case WSAENOTSOCK :
            
            ret = EBADF;
            
            break;
            
        case WSAEOPNOTSUPP :
            
            ret = EOPNOTSUPP;
            
            break;
            
        case WSAEWOULDBLOCK :
            
            ret = EWOULDBLOCK;
            
            break;
            
        case WSAETIMEDOUT :
            
            ret = ETIMEDOUT;
            
            break;
            
        case WSAEACCES :
            
            ret = EACCES;
            
            break;
            
        case WSAENOTCONN :
            
            ret = ENOTCONN;
            
            break;
            
        case WSAECONNRESET :
            
            ret = ECONNRESET;
            
            break;
            
        default :
            
            ret = EINVAL;
    }
    
    return ret;
}
//--------------------------------------------------------------------------------------------------
int domain2WSAdomain(int domain)
{
    switch (domain)
    {
        case AF_UNIX :
            
            return _AF_UNIX;
            
        case AF_INET :
            
            return _AF_INET;
            
        case AF_IPX :
            
            return _AF_IPX;
            
        case AF_SNA :
            
            return _AF_SNA;
            
        case AF_DECnet :
            
            return _AF_DECnet;
            
        case AF_APPLETALK :
            
            return _AF_APPLETALK;
            
        case AF_INET6 :
            
            return _AF_INET6;
    }
    
    return domain;
}
//--------------------------------------------------------------------------------------------------
int WSAdomain2domain(int WSA)
{
    switch (WSA)
    {
        case _AF_UNIX :
            
            return AF_UNIX;
            
        case _AF_INET :
            
            return AF_INET;
            
        case _AF_IPX :
            
            return AF_IPX;
            
        case _AF_SNA :
            
            return AF_SNA;
            
        case _AF_DECnet :
            
            return AF_DECnet;
            
        case _AF_APPLETALK :
            
            return AF_APPLETALK;
            
        case _AF_INET6 :
            
            return AF_INET6;
    }
    
    return WSA;
}
//--------------------------------------------------------------------------------------------------
void WSA2sockopt(int *level, int *option)
{
    switch (*level)
    {
        case _SOL_SOCKET :
            
            *level = SOL_SOCKET;
            
            switch (*option)
        {
            case _SO_DEBUG :
                
                *option = SO_DEBUG;
                
                break;
                
            case _SO_ACCEPTCONN :
                
                *option = SO_ACCEPTCONN;
                
                break;
                
            case _SO_REUSEADDR :
                
                *option = SO_REUSEADDR;
                
                break;
                
            case _SO_KEEPALIVE :
                
                *option = SO_KEEPALIVE;
                
                break;
                
            case _SO_DONTROUTE :
                
                *option = SO_DONTROUTE;
                
                break;
                
            case SO_BROADCAST :
                
                *option = SO_BROADCAST;
                
                break;
                
            case _SO_LINGER :
                
                *option = SO_LINGER;
                
                break;
                
            case _SO_OOBINLINE :
                
                *option = SO_OOBINLINE;
                
                break;
                
            case _SO_SNDBUF :
                
                *option = SO_SNDBUF;
                
                break;
                
            case _SO_RCVBUF :
                
                *option = SO_RCVBUF;
                
                break;
                
            case _SO_SNDLOWAT :
                
                *option = SO_SNDLOWAT;
                
                break;
                
            case _SO_RCVLOWAT :
                
                *option = SO_RCVLOWAT;
                
                break;
                
            case _SO_SNDTIMEO :
                
                *option = SO_SNDTIMEO;
                
                break;
                
            case _SO_RCVTIMEO :
                
                *option = SO_RCVTIMEO;
                
                break;
                
            case _SO_ERROR :
                
                *option = SO_ERROR;
                
                break;
                
            case _SO_TYPE :
                
                *option = SO_TYPE;
                
                break;
        }
            
            break;
            
        case _IPPROTO_TCP :
            
            *option = (bool)*option;
            
            break;
    }
}
//--------------------------------------------------------------------------------------------------
void sockopt2WSA(int *level, int *option)
{
    switch (*level)
    {
        case SOL_SOCKET :
            
            *level = _SOL_SOCKET;
            
            switch (*option)
        {
            case SO_DEBUG :
                
                *option = _SO_DEBUG;
                
                break;
                
            case SO_ACCEPTCONN :
                
                *option = _SO_ACCEPTCONN;
                
                break;
                
            case SO_REUSEADDR :
                
                *option = _SO_REUSEADDR;
                
                break;
                
            case SO_KEEPALIVE :
                
                *option = _SO_KEEPALIVE;
                
                break;
                
            case SO_DONTROUTE :
                
                *option = _SO_DONTROUTE;
                
                break;
                
            case SO_BROADCAST :
                
                *option = _SO_BROADCAST;
                
                break;
                
            case SO_LINGER :
                
                *option = _SO_LINGER;
                
                break;
                
            case SO_OOBINLINE :
                
                *option = _SO_OOBINLINE;
                
                break;
                
            case SO_SNDBUF :
                
                *option = _SO_SNDBUF;
                
                break;
                
            case SO_RCVBUF :
                
                *option = _SO_RCVBUF;
                
                break;
                
            case SO_SNDLOWAT :
                
                *option = _SO_SNDLOWAT;
                
                break;
                
            case SO_RCVLOWAT :
                
                *option = _SO_RCVLOWAT;
                
                break;
                
            case SO_SNDTIMEO :
                
                *option = _SO_SNDTIMEO;
                
                break;
                
            case SO_RCVTIMEO :
                
                *option = _SO_RCVTIMEO;
                
                break;
                
            case SO_ERROR :
                
                *option = _SO_ERROR;
                
                break;
                
            case SO_TYPE :
                
                *option = _SO_TYPE;
                
                break;
        }
            
            break;
            
        case _IPPROTO_TCP :
            
            *option = (bool)*option;
            
            break;
    }
}
//--------------------------------------------------------------------------------------------------
short WSAPoll2poll(short flags)
{
    short ret = 0;
    
    if (_POLLRDNORM & flags)
        ret |= POLLRDNORM;
    
    if (_POLLRDBAND & flags)
        ret |= POLLRDBAND;
    
    if (_POLLIN & flags)
        ret |= POLLIN;
    
    if (_POLLPRI & flags)
        ret |= POLLPRI;
    
    if (_POLLWRNORM & flags)
        ret |= POLLWRNORM;
    
    if (_POLLOUT & flags)
        ret |= POLLOUT;
    
    if (_POLLWRBAND & flags)
        ret |= POLLWRBAND;
    
    if (_POLLERR & flags)
        ret |= POLLERR;
    
    if (_POLLHUP & flags)
        ret |= POLLHUP;
    
    if (_POLLNVAL & flags)
        ret |= POLLNVAL;
    
    return ret;
}
//--------------------------------------------------------------------------------------------------
short poll2WSAPoll(short flags)
{
    short ret = 0;
    
    if (POLLRDNORM & flags)
        ret |= _POLLRDNORM;
    
    if (POLLRDBAND & flags)
        ret |= _POLLRDBAND;
    
    if (POLLIN & flags)
        ret |= _POLLIN;
    
    if (POLLPRI & flags)
        ret |= _POLLPRI;
    
    if (POLLWRNORM & flags)
        ret |= _POLLWRNORM;
    
    if (POLLOUT & flags)
        ret |= _POLLOUT;
    
    if (POLLWRBAND & flags)
        ret |= _POLLWRBAND;
    
    if (POLLERR & flags)
        ret |= _POLLERR;
    
    if (POLLHUP & flags)
        ret |= _POLLHUP;
    
    if (POLLNVAL & flags)
        ret |= _POLLNVAL;
    
    return ret;
}
//--------------------------------------------------------------------------------------------------
int msgFlags2WSAmsgFlags(int flags)
{
    int ret = 0;
    
    if (MSG_OOB & flags)
        ret |= _MSG_OOB;
    
    if (MSG_PEEK & flags)
        ret |= _MSG_PEEK;
    
    if (MSG_DONTROUTE & flags)
        ret |= _MSG_DONTROUTE;
    
    if (MSG_WAITALL & flags)
        ret |= _MSG_WAITALL;
    
    return ret;
}
//--------------------------------------------------------------------------------------------------
int WSAmsgFlags2msgFlags(int flags)
{
    int ret = 0;
    
    if (_MSG_OOB & flags)
        ret |= MSG_OOB;
    
    if (_MSG_PEEK & flags)
        ret |= MSG_PEEK;
    
    if (_MSG_DONTROUTE & flags)
        ret |= MSG_DONTROUTE;
    
    if (_MSG_WAITALL & flags)
        ret |= MSG_WAITALL;
    
    return ret;
}
//==================================================================================================
}
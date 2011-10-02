#pragma once

#include <map>
#include <sys/types.h>

#include "WSATypes.h"

namespace BSD2WSA
{
//==================================================================================================
//socket types converting

class SocketsStorage
{
public :
    bool hasSocket(int const fd);
    int addSocket(SOCKET const handle);
    void removeSocket(int const fd);
    SOCKET convert(int const fd);
    int convert(SOCKET const handle);
    
private :
    typedef std::map<int, SOCKET> sockets_map;
    sockets_map map_;
};    
    
//constants converting
void select2WSASelect(fd_set *bsd, fd_set_ *win, sockets_map &sockets);
void WSASelect2select(fd_set_ *win, fd_set *bsd, sockets_map &sockets);
int WSA2errno(int const WSA);
int domain2WSAdomain(int domain);
int WSAdomain2domain(int domain);
void WSA2sockopt(int *level, int *option);
void sockopt2WSA(int *level, int *option);
short WSAPoll2poll(short const flags);
short poll2WSAPoll(short const flags);
int msgFlags2WSAmsgFlags(int flags);
int WSAmsgFlags2msgFlags(int flags);
//==================================================================================================
}
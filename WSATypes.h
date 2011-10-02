#pragma once

#include <stddef.h>

//==================================================================================================
typedef unsigned DWORD;
typedef unsigned UINT;
typedef int INT;
typedef long LONG;
typedef unsigned long ULONG;
typedef short SHORT;
typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef char CHAR;
typedef char * LPSTR;

#define MAX_PROTOCOL_CHAIN 7
#define FD_MAX_EVENTS 10
#define WSAPROTOCOL_LEN 255
#define FD_SETSIZE_ 64

#define WSADESCRIPTION_LEN      256
#define WSASYS_STATUS_LEN       128
#define WSAEVENT                HANDLE
#define WSAOVERLAPPED           OVERLAPPED

#define WSABASEERR              10000
#define WSAEINTR                (WSABASEERR+4)
#define WSAEBADF                (WSABASEERR+9)
#define WSAEACCES               (WSABASEERR+13)
#define WSAEFAULT               (WSABASEERR+14)
#define WSAEINVAL               (WSABASEERR+22)
#define WSAEMFILE               (WSABASEERR+24)
#define WSAEWOULDBLOCK          (WSABASEERR+35)
#define WSAEINPROGRESS          (WSABASEERR+36)
#define WSAEALREADY             (WSABASEERR+37)
#define WSAENOTSOCK             (WSABASEERR+38)
#define WSAEDESTADDRREQ         (WSABASEERR+39)
#define WSAEMSGSIZE             (WSABASEERR+40)
#define WSAEPROTOTYPE           (WSABASEERR+41)
#define WSAENOPROTOOPT          (WSABASEERR+42)
#define WSAEPROTONOSUPPORT      (WSABASEERR+43)
#define WSAESOCKTNOSUPPORT      (WSABASEERR+44)
#define WSAEOPNOTSUPP           (WSABASEERR+45)
#define WSAEPFNOSUPPORT         (WSABASEERR+46)
#define WSAEAFNOSUPPORT         (WSABASEERR+47)
#define WSAEADDRINUSE           (WSABASEERR+48)
#define WSAEADDRNOTAVAIL        (WSABASEERR+49)
#define WSAENETDOWN             (WSABASEERR+50)
#define WSAENETUNREACH          (WSABASEERR+51)
#define WSAENETRESET            (WSABASEERR+52)
#define WSAECONNABORTED         (WSABASEERR+53)
#define WSAECONNRESET           (WSABASEERR+54)
#define WSAENOBUFS              (WSABASEERR+55)
#define WSAEISCONN              (WSABASEERR+56)
#define WSAENOTCONN             (WSABASEERR+57)
#define WSAESHUTDOWN            (WSABASEERR+58)
#define WSAETOOMANYREFS         (WSABASEERR+59)
#define WSAETIMEDOUT            (WSABASEERR+60)
#define WSAECONNREFUSED         (WSABASEERR+61)
#define WSAELOOP                (WSABASEERR+62)
#define WSAENAMETOOLONG         (WSABASEERR+63)
#define WSAEHOSTDOWN            (WSABASEERR+64)
#define WSAEHOSTUNREACH         (WSABASEERR+65)
#define WSAENOTEMPTY            (WSABASEERR+66)
#define WSAEPROCLIM             (WSABASEERR+67)
#define WSAEUSERS               (WSABASEERR+68)
#define WSAEDQUOT               (WSABASEERR+69)
#define WSAESTALE               (WSABASEERR+70)
#define WSAEREMOTE              (WSABASEERR+71)
#define WSASYSNOTREADY          (WSABASEERR+91)
#define WSAVERNOTSUPPORTED      (WSABASEERR+92)
#define WSANOTINITIALISED       (WSABASEERR+93)
#define WSAEDISCON              (WSABASEERR+101)
#define WSAENOMORE              (WSABASEERR+102)
#define WSAECANCELLED           (WSABASEERR+103)
#define WSAEINVALIDPROCTABLE    (WSABASEERR+104)
#define WSAEINVALIDPROVIDER     (WSABASEERR+105)
#define WSAEPROVIDERFAILEDINIT  (WSABASEERR+106)
#define WSASYSCALLFAILURE       (WSABASEERR+107)
#define WSASERVICE_NOT_FOUND    (WSABASEERR+108)
#define WSATYPE_NOT_FOUND       (WSABASEERR+109)
#define WSA_E_NO_MORE           (WSABASEERR+110)
#define WSA_E_CANCELLED         (WSABASEERR+111)
#define WSAEREFUSED             (WSABASEERR+112)

#define INVALID_SOCKET (SOCKET)(-1)
#define SOCKET_ERROR   (-1)

#define _AF_UNIX         1
#define _AF_INET         2
#define _AF_IPX          6
#define _AF_SNA          11
#define _AF_DECnet       12
#define _AF_APPLETALK    16
#define _AF_INET6        23
#define _AF_IRDA         26

#define _SOL_SOCKET     0xffff
#define _IPPROTO_TCP    6
#define _FIONBIO        0x8004667e

#define _SO_DEBUG       0x0001
#define _SO_ACCEPTCONN  0x0002
#define _SO_REUSEADDR   0x0004
#define _SO_KEEPALIVE   0x0008
#define _SO_DONTROUTE   0x0010
#define _SO_BROADCAST   0x0020
#define _SO_USELOOPBACK 0x0040
#define _SO_LINGER      0x0080
#define _SO_OOBINLINE   0x0100
#define _SO_REUSEPORT   0x0200
#define _SO_SNDBUF      0x1001
#define _SO_RCVBUF      0x1002
#define _SO_SNDLOWAT    0x1003
#define _SO_RCVLOWAT    0x1004
#define _SO_SNDTIMEO    0x1005
#define _SO_RCVTIMEO    0x1006
#define _SO_ERROR       0x1007
#define _SO_TYPE        0x1008

#define _MSG_OOB         0x1
#define _MSG_PEEK        0x2
#define _MSG_DONTROUTE   0x4
#define _MSG_WAITALL     0x8

#define _POLLRDNORM 0x0100
#define _POLLRDBAND 0x0200
#define _POLLIN (_POLLRDNORM | _POLLRDBAND)
#define _POLLPRI 0x0400

#define _POLLWRNORM 0x0010
#define _POLLOUT (_POLLWRNORM)
#define _POLLWRBAND 0x0020

#define _POLLERR 0x0001
#define _POLLHUP 0x0002
#define _POLLNVAL 0x0004

typedef struct _GUID {
    ULONG Data1;
    WORD Data2;
    WORD Data3;
    BYTE Data4[8];
} GUID;

struct WSAPROTOCOLCHAIN {
    INT ChainLen;
    DWORD ChainEntries[MAX_PROTOCOL_CHAIN];
};

struct WSANETWORKEVENTS {
    LONG lNetworkEvents;
    INT iErrorCode[FD_MAX_EVENTS];
};

struct WSAPROTOCOL_INFOA {
    DWORD dwServiceFlags1;
    DWORD dwServiceFlags2;
    DWORD dwServiceFlags3;
    DWORD dwServiceFlags4;
    DWORD dwProviderFlags;
    GUID ProviderId;
    DWORD dwCatalogEntryId;
    WSAPROTOCOLCHAIN ProtocolChain;
    INT iVersion;
    INT iAddressFamily;
    INT iMaxSockAddr;
    INT iMinSockAddr;
    INT iSocketType;
    INT iProtocol;
    INT iProtocolMaxOffset;
    INT iNetworkByteOrder;
    INT iSecurityScheme;
    DWORD dwMessageSize;
    DWORD dwProviderReserved;
    CHAR szProtocol[WSAPROTOCOL_LEN + 1];
};

typedef struct sockaddr_ {
    WORD sa_family;
    CHAR sa_data[14];
} SOCKADDR, *PSOCKADDR, *LPSOCKADDR;

typedef struct fd_set_ {
    UINTINT fd_count;
    SOCKET  fd_array[FD_SETSIZE_];
} fd_set_;

typedef struct addrinfo_ {
    INT ai_flags;
    INT ai_family;
    INT ai_socktype;
    INT ai_protocol;
    UINT ai_addrlen;
    LPSTR ai_canonname;
    struct sockaddr_ *ai_addr;
    struct addrinfo_ *ai_next;
} ADDRINFOA, *PADDRINFOA;

typedef struct pollfd_ {

SOCKET fd;
SHORT events;
SHORT revents;

} WSAPOLLFD, *PWSAPOLLFD, *LPWSAPOLLFD;

typedef struct timeval_ {
        LONG    tv_sec;
        LONG    tv_usec;
} timeval_;

typedef struct  hostent_ {
        LPSTR h_name;
        LPSTR h_aliases[];
        SHORT h_addrtype;
        SHORT   h_length;
        LPSTR h_addr_list[];
} hostent_;

typedef hostent_ HOSTENT;

typedef struct  servent_ {
        LPSTR s_name;
        LPSTR s_aliases[];
        SHORT s_port;
        LPSTR s_proto;
} servent_;
//==================================================================================================

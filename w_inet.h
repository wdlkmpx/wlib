/*
 * Public Domain
 */

/// Structure to contain information about address of a service provider.
// struct addrinfo
// {
//   int ai_flags;             // Input flags
//   int ai_family;            // Protocol family for socket: PF_UNSPEC, PF_INET, PF_INET6
//   int ai_socktype;          // Socket type
//   int ai_protocol;          // Protocol for socket
//   socklen_t ai_addrlen;     // Length of socket address
//   struct sockaddr *ai_addr; // Socket address for socket
//   char *ai_canonname;       // Canonical name for service location
//   struct addrinfo *ai_next; // Pointer to next in list
// };
//
// struct sockaddr
// {
//   unsigned short sa_family;  // Protocol family for socket: AF_UNSPEC, AF_INET, AF_INET6
//   char sa_data[14];
// };
//
// struct sockaddr_in
// {
//   unsigned short sin_family;  // Must be AF_INET
//   unsigned short sin_port;    // Port
//   struct in_addr sin_addr;    // IPv4 address
//   unsigned char  sin_zero[8];
// };
// struct in_addr {
//   uint32_t s_addr;    // IPv4 address
// };
//
// struct sockaddr_in6
// {
//   unsigned short sin_family;  // must be AF_INET6
//   unsigned short sin6_port;   // Port
//   uint32_t sin6_flowinfo;     // IPv6 flow information
//   struct in6_addr sin6_addr;  // IPv6 address
//   uint32_t sin6_scope_id;     // IPv6 scope id
// };



#ifndef __W_SOCKET_H
#define __W_SOCKET_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <sys/socket.h>
#include <netdb.h>
#include <stdint.h>

enum w_inet_socktype
{
    W_INET_SOCKTYPE_TCP = SOCK_STREAM,
    W_INET_SOCKTYPE_UDP = SOCK_DGRAM,
};

enum w_inet_ipversion
{
    W_INET_IPVALL = AF_UNSPEC,
    W_INET_IPV6   = AF_INET6,
    W_INET_IPV4   = AF_INET,
};

struct addrinfo * w_inet_addrinfo_dup1 (struct addrinfo * addri); // must be freed with freeaddrinfo

struct sockaddr * w_sockaddr_new (int is_ipv6); // must be freed with free()
int  w_sockaddr_get_port   (struct sockaddr * saddr);
void w_sockaddr_get_ip_str (struct sockaddr * saddr, char * outbuf, int size);
void * w_sockaddr_get_addr (struct sockaddr * saddr);
socklen_t w_sockaddr_get_size (struct sockaddr * saddr);
void w_sockaddr_reset (struct sockaddr * saddr);
void w_sockaddr_set_port (struct sockaddr * saddr, int port);
int w_sockaddr_set_ip_from_str (struct sockaddr * saddr, const char * ipstr);

struct addrinfo * w_inet_lookup_host (const char * service, /* NULL */
                                      const char * host,
                                      unsigned int port,
                                      enum w_inet_ipversion ip_version,
                                      enum w_inet_socktype socktype);

struct addrinfo * w_inet_lookup_host2 (const char * host, struct addrinfo *hints);

int w_inet_connection_new (struct addrinfo * addri);

int w_sockaddr_set_ipv4_addr (struct sockaddr * saddr, uint32_t addr);
void w_inet_ipv4_num_to_str (uint32_t ipnum, char *buf, int buflen);
uint32_t w_inet_ipv4_str_to_num (const char * ipstr);

#ifdef __cplusplus
}
#endif

#endif /* __W_SOCKET_H */

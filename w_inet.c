/*
 * Public Domain
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <arpa/inet.h> // inet_ntop()
#include <fcntl.h>     // fcntl()
#include <unistd.h>    // close()
#include "w_inet.h"


/// Duplicate addrinfo, it must contain all the required info for new connections
/// Only addrinfo->ai_addr may need changes (change ip and port)
/// Must be freed with freeaddrinfo()
struct addrinfo * w_inet_addrinfo_dup1 (struct addrinfo * addri) 
{
    struct addrinfo * addri_new;
    if (!addri || !addri->ai_addr) {
        fprintf (stderr, "source addrinfo or ai_addr is NULL!!\n");
        return NULL;
    }
    if ((addri->ai_family != AF_INET) && (addri->ai_family != AF_INET6)) {
        fprintf (stderr, "Invalid addrinfo\n");
        return NULL;
    }
    addri_new = malloc (sizeof(*addri));
    memcpy (addri_new, addri, sizeof (*addri));

    addri_new->ai_addr = malloc (addri->ai_addrlen);
    memcpy (addri_new->ai_addr, addri->ai_addr, addri->ai_addrlen);

    // fix for freeaddrinfo()
    addri_new->ai_canonname = NULL;
    addri_new->ai_next = NULL;
    return addri_new;
}


/// Create a new sockaddr and initialize it with the proper AF_INET*
/// This makes more sense when creating a socket to listen for incoming connections...
/// Must freed with free()
struct sockaddr * w_sockaddr_new (int is_ipv6)
{
    struct sockaddr * saddr;
    if (is_ipv6) {
        saddr = calloc (1, sizeof(struct sockaddr_in6));
        saddr->sa_family = AF_INET6;
    } else {
        saddr = calloc (1, sizeof(struct sockaddr_in));
        saddr->sa_family = AF_INET;
    }
    return saddr;
}


int w_sockaddr_get_port (struct sockaddr * saddr)
{
    unsigned short sin_port;
    if (saddr->sa_family == AF_INET) {
        sin_port = (((struct sockaddr_in*)saddr)->sin_port);
    } else { /* ipv6 */
        sin_port = (((struct sockaddr_in6*)saddr)->sin6_port);
    }
    return (ntohs (sin_port));
}


void w_sockaddr_get_ip_str (struct sockaddr * saddr, char * outbuf, int size)
{
    void * sin_addr;
    *outbuf = 0;
    if (saddr->sa_family == AF_INET) {
        sin_addr = &(((struct sockaddr_in*)saddr)->sin_addr);
    } else { /* ipv6 */
        sin_addr = &(((struct sockaddr_in6*)saddr)->sin6_addr);
    }
    inet_ntop (saddr->sa_family, sin_addr, outbuf, size);
}


void * w_sockaddr_get_addr (struct sockaddr * saddr)
{
    void * sin_addr;
    if (saddr->sa_family == AF_INET) {
        sin_addr = &(((struct sockaddr_in*)saddr)->sin_addr);
    } else { /* ipv6 */
        sin_addr = &(((struct sockaddr_in6*)saddr)->sin6_addr);
    }
    return sin_addr;
}


socklen_t w_sockaddr_get_size (struct sockaddr * saddr)
{
    if (saddr->sa_family == AF_INET) {
        return sizeof(struct sockaddr_in);
    } else { /* ipv6 */
        return sizeof(struct sockaddr_in6);
    }
}


void w_sockaddr_reset (struct sockaddr * saddr)
{
    if (saddr->sa_family == AF_INET) {
        memset (saddr, 0, sizeof(struct sockaddr_in));
        saddr->sa_family = AF_INET;
    } else { /* ipv6 */
        memset (saddr, 0, sizeof(struct sockaddr_in6));
        saddr->sa_family = AF_INET6;
    }
}


void w_sockaddr_set_port (struct sockaddr * saddr, int port)
{
    if (saddr->sa_family == AF_INET) {
        puts ("ipv4");
        ((struct sockaddr_in*)saddr)->sin_port = htons (port);
    } else { /* ipv6 */
        puts ("ipv6");
        ((struct sockaddr_in6*)saddr)->sin6_port = htons (port);
    }
}


int w_sockaddr_set_ip_from_str (struct sockaddr * saddr, const char * ipstr)
{
    int ret;
    void * addr = w_sockaddr_get_addr (saddr);
    ret = inet_pton (saddr->sa_family, ipstr, addr);
    return ret; // 1=ok
}


struct addrinfo * w_inet_lookup_host (const char * service, /* NULL */
                                      const char * host,
                                      unsigned int port,
                                      enum w_inet_ipversion ip_version,
                                      enum w_inet_socktype socktype)
{
    // this is where everything about the connection is specified
    // the resulting addrinfo will contain info according to the params specified here
    struct addrinfo hints, *hostp;
    char serv[8];
    int ret;

    memset (&hints, 0, sizeof (hints));
    hints.ai_flags = AI_CANONNAME;

    switch (ip_version)
    {
        case W_INET_IPV4: hints.ai_family = AF_INET;   break;
        case W_INET_IPV6: hints.ai_family = AF_INET6;  break;
        default:          hints.ai_family = PF_UNSPEC; break;
    }

    switch (socktype)
    {
        case W_INET_SOCKTYPE_UDP: hints.ai_socktype = SOCK_DGRAM;  break; /* UDP */
        case W_INET_SOCKTYPE_TCP: hints.ai_socktype = SOCK_STREAM; break; /* TCP */
    }

    if (!port && service) {
        snprintf (serv, sizeof (serv), "%s", service); 
    } else {
        snprintf (serv, sizeof (serv), "%d", port);
    }

    ret = getaddrinfo (host, serv, &hints, &hostp);
    if (ret != 0)
    {
        fprintf (stderr, "getaddrinfo: %s\n", gai_strerror(ret));
        return NULL;
    }

    return (hostp);
}



struct addrinfo * w_inet_lookup_host2 (const char * host, struct addrinfo *hints)
{
    int ret;
    struct addrinfo * hostp;
    ret = getaddrinfo (host, NULL, hints, &hostp);
    if (ret != 0)
    {
        fprintf (stderr, "getaddrinfo: %s\n", gai_strerror(ret));
        return NULL;
    }
    return (hostp);
}


int w_inet_connection_new (struct addrinfo * addri)
{
    // this is usually called after w_inet_lookup_host()
    //   using the dynamically allocated addrinfo*
    // returns fd that must be closed with close()
    int sock, ret;
    struct timeval timeout;
    timeout.tv_sec = 30; /* connection timeout in seconds */
    timeout.tv_usec = 0;

    sock = socket (addri->ai_family, addri->ai_socktype, 0);
    if (sock < 0)
    {
        perror("Could not create socket");
        return -1;
    }

    if (fcntl (sock, F_SETFD, 1) == -1)
    {
        close (sock);
        return -1;
    }

    setsockopt (sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

    ret = connect (sock, addri->ai_addr, addri->ai_addrlen);
    if (ret == -1)
    {
        perror("Could not connect");
        close (sock);
        return -1;
    }

    return sock;
}


int w_sockaddr_set_ipv4_addr (struct sockaddr * saddr, uint32_t addr)
{
    if (saddr->sa_family == AF_INET) {
        ((struct sockaddr_in *)saddr)->sin_addr.s_addr = addr;
        return 1; // ok
    } else {
        return 0; // error
    }
}

uint32_t w_inet_ipv4_str_to_num (const char * ipstr)
{
    uint32_t n[4], addr;
    int ret;
    ret = sscanf (ipstr, "%u.%u.%u.%u", &n[0], &n[1], &n[2], &n[3]);
    if (ret != 4) {
        return 0; // error
    }
    addr = ((n[0] & 0xff) << 24) | ((n[1] & 0xff) << 16) | ((n[2] & 0xff) <<  8) | (n[3] & 0xff);
    return addr;
}


void w_inet_ipv4_num_to_str (uint32_t ipnum, char *buf, int buflen)
{
    snprintf (buf, buflen-1, "%u.%u.%u.%u",
              ipnum >> 24 & 0xff, ipnum >> 16 & 0xff, ipnum >> 8 & 0xff, ipnum & 0xff);
}

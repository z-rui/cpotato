#ifndef UTIL_H
#define UTIL_H

#include "nbuf.h"

#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>

#define die(fmt, ...) ( \
	fprintf(stderr, fmt"\n", ##__VA_ARGS__), \
	exit(1))

#define CHECK(exp) ((exp) ? (void) 0 : die("CHECK(%s) failed", #exp))
#define CHECK_SYS(exp) ({\
	int _rc; \
	if ((_rc = (exp)) < 0) { \
		perror(#exp); \
		exit(1); \
	}; \
	_rc; })

extern void
tcp_(int (*fn)(int, const struct sockaddr *, socklen_t),
	int sockfd, const char *host, int port);

extern int getport(int sockfd);
extern void send_buf(int sockfd, struct nbuf_buffer *buf);
extern int recv_buf(int sockfd, struct nbuf_buffer *buf);
extern int init_fdset(fd_set *fds, ...);

#endif  /* UTIL_H */

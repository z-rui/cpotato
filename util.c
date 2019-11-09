#include "util.h"

#include <stdarg.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>

void
tcp_(int (*fn)(int, const struct sockaddr *, socklen_t),
	int sockfd, const char *host, int port)
{
	char portbuf[20];
	struct addrinfo hints = {0};
	struct addrinfo *res = NULL, *rp;
	int rc;

	CHECK(0 <= port && port <= 65535);
	sprintf(portbuf, "%u", port);
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if ((rc = getaddrinfo(host, portbuf, &hints, &res)) != 0)
		die("getaddrinfo: %s", gai_strerror(rc));
	for (rp = res; rp != NULL; rp = rp->ai_next)
		if ((*fn)(sockfd, rp->ai_addr, rp->ai_addrlen) == 0)
			break;
	CHECK(rp != NULL);
	freeaddrinfo(res);
}

int
getport(int sockfd)
{
	struct sockaddr_in saddr;
	socklen_t slen = sizeof saddr;

	CHECK_SYS(getsockname(sockfd, (struct sockaddr *) &saddr, &slen));
	return ntohs(saddr.sin_port);
}

void
send_buf(int sockfd, struct nbuf_buffer *buf)
{
	char b[4];

	CHECK(buf->len >= 8);
	CHECK(buf->len <= 0xffffffffU);
	nbuf_write_int_unsafe(b, 4, buf->len);
	CHECK_SYS(send(sockfd, b, 4, 0));
	CHECK_SYS(send(sockfd, buf->base+4, buf->len-4, 0));
}

int
recv_buf(int sockfd, struct nbuf_buffer *buf)
{
	char b[4];
	size_t n;

	n = recv(sockfd, b, 4, MSG_WAITALL);
	if (n == 0)
		return 0;
	CHECK(n == 4);

	n = nbuf_read_int_unsafe(b, 4);
	CHECK(n >= 8);

	nbuf_clear(buf);
	CHECK(nbuf_reserve(buf, n));
	nbuf_write_int_unsafe(buf->base, 4, 1);
	buf->len += 4;
	buf->len += recv(sockfd, buf->base+4, n-4, MSG_WAITALL);
	CHECK(buf->len == n);
	return n;
}

int
init_fdset(fd_set *fds, ...)
{
	va_list ap;
	int nfds = 0, fd;

	FD_ZERO(fds);
	va_start(ap, fds);
	for (;;) {
		fd = va_arg(ap, int);
		if (fd < 0)
			break;
		FD_SET(fd, fds);
		if (fd >= nfds)
			nfds = fd + 1;
	}
	va_end(ap);

	return nfds;
}


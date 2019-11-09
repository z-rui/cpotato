#include "potato.nb.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>

static Potato
append_trace(struct nbuf_buffer *buf, Potato *potato, int id)
{
	Potato potato1;
	size_t n, i;
	TraceEntry entry, entry1;

	nbuf_clear(buf);
	potato1 = new_Potato(buf);
	Potato_set_ttl(&potato1, Potato_ttl(potato) - 1);
	n = Potato_trace_size(potato);
	Potato_init_trace(&potato1, n+1);
	CHECK(Potato_trace_size(&potato1) == n+1);
	for (i = 0; i < n; i++) {
		entry = Potato_trace(potato, i);
		entry1 = Potato_trace(&potato1, i);
		TraceEntry_set_id(&entry1, TraceEntry_id(&entry));
	}
	entry1 = Potato_trace(&potato1, n);
	TraceEntry_set_id(&entry1, id);
	return potato1;
}

int
main(int argc, char *argv[])
{
	const char *host;
	int port;
	int rm_sock, ln_sock, cn_sock, in_conn;
	struct nbuf_buffer buf, buf1;
	Address addr;
	PlayerInfo info;
	Potato potato, potato1;
	int n_player;
	int self_id;
	fd_set rlist;
	int nfds = 0;

	srand(time(NULL));

	CHECK(argc == 3);
	host = argv[1];
	CHECK(sscanf(argv[2], "%d", &port) == 1);

	nbuf_init_write(&buf, NULL, 0);
	nbuf_init_write(&buf1, NULL, 0);

	rm_sock = CHECK_SYS(socket(AF_INET, SOCK_STREAM, 0));
	tcp_(connect, rm_sock, host, port);

	ln_sock = CHECK_SYS(socket(AF_INET, SOCK_STREAM, 0));
	tcp_(bind, ln_sock, NULL, 0);
	listen(ln_sock, 10);

	addr = new_Address(&buf);
	Address_set_port(&addr, getport(ln_sock));
	send_buf(rm_sock, &buf);

	recv_buf(rm_sock, &buf);
	info = get_PlayerInfo(&buf);
	n_player = PlayerInfo_total(&info);
	self_id = PlayerInfo_id(&info);
	CHECK(n_player > 0);

	addr = PlayerInfo_next(&info);

	cn_sock = CHECK_SYS(socket(AF_INET, SOCK_STREAM, 0));
	tcp_(connect, cn_sock, Address_host(&addr, NULL), Address_port(&addr));

	in_conn = CHECK_SYS(accept(ln_sock, NULL, NULL));

	nfds = init_fdset(&rlist, rm_sock, cn_sock, in_conn, -1);
	for (;;) {
		int fd = -1;

		FD_SET(rm_sock, &rlist);
		FD_SET(cn_sock, &rlist);
		FD_SET(in_conn, &rlist);
		CHECK_SYS(select(nfds, &rlist, NULL, NULL, NULL));
		if (FD_ISSET(rm_sock, &rlist))
			fd = rm_sock;
		else if (FD_ISSET(cn_sock, &rlist))
			fd = cn_sock;
		else if (FD_ISSET(in_conn, &rlist))
			fd = in_conn;
		CHECK(fd != -1);

		if (recv_buf(fd, &buf) == 0)
			break;
		potato = get_Potato(&buf);
		CHECK(Potato_ttl(&potato) > 0);
		potato1 = append_trace(&buf1, &potato, self_id);
		if (Potato_ttl(&potato1) == 0) {
			puts("I'm it.");
			fd = rm_sock;
		} else {
			int id;
			if (rand() & 1) {
				fd = cn_sock;
				id = self_id + 1;
			} else {
				fd = in_conn;
				id = self_id - 1;
			}
			id = (id + n_player) % n_player;
			printf("Sending potato to player %d\n", id);
		}
		send_buf(fd, &buf1);
		if (fd == rm_sock)
			break;
	}

	nbuf_free(&buf);
	nbuf_free(&buf1);

	return 0;
}

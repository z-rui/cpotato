#include "potato.nb.h"
#include "util.h"

#include <stdlib.h>
#include <time.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>

static void
set_address(Address *addr, const struct sockaddr_in *saddr)
{
	char buf[100];

	inet_ntop(AF_INET, &saddr->sin_addr, buf, 100);
	Address_set_host(addr, buf, -1);
	Address_set_port(addr, ntohs(saddr->sin_port));
}

static void
print_trace(Potato *potato)
{
	size_t n, i;
	TraceEntry entry;

	n = Potato_trace_size(potato);
	printf("Trace:");
	for (i = 0; i < n; i++) {
		entry = Potato_trace(potato, i);
		printf(" %d", TraceEntry_id(&entry));
	}
	printf("\n");
}

int
main(int argc, char *argv[])
{
	int port;
	int n_player, n_hop;
	int ln_sock;
	int *pl_socks;
	struct sockaddr_in *pl_addrs;
	struct nbuf_buffer buf;
	PlayerInfo info;
	Address prev, next;
	Potato potato;
	int i;

	srand(time(NULL));

	CHECK(argc == 4);
	CHECK(sscanf(argv[1], "%d", &port) == 1);
	CHECK(sscanf(argv[2], "%d", &n_player) == 1);
	CHECK(sscanf(argv[3], "%d", &n_hop) == 1);

	CHECK(n_player > 0);
	CHECK(n_hop >= 0);

	nbuf_init_write(&buf, NULL, 0);
	pl_socks = calloc(n_player, sizeof pl_socks[0]);
	pl_addrs = calloc(n_player, sizeof pl_addrs[0]);

	ln_sock = CHECK_SYS(socket(AF_INET, SOCK_STREAM, 0));
	tcp_(bind, ln_sock, NULL, port);
	listen(ln_sock, 100);

	for (i = 0; i < n_player; i++) {
		socklen_t slen = sizeof pl_addrs[i];

		pl_socks[i] = CHECK_SYS(accept(ln_sock, (struct sockaddr *) &pl_addrs[i], &slen));
		CHECK(slen == sizeof pl_addrs[i]);
		recv_buf(pl_socks[i], &buf);
		Address addr = get_Address(&buf);
		pl_addrs[i].sin_port = htons(Address_port(&addr));
	}

	nbuf_clear(&buf);
	info = new_PlayerInfo(&buf);
	PlayerInfo_set_total(&info, n_player);
	prev = PlayerInfo_init_prev(&info);
	next = PlayerInfo_init_next(&info);
	for (i = 0; i < n_player; i++) {
		int j, k;
		
		j = (i-1 + n_player) % n_player;
		k = (i+1 + n_player) % n_player;
		PlayerInfo_set_id(&info, i);
		set_address(&prev, &pl_addrs[j]);
		set_address(&next, &pl_addrs[k]);
		send_buf(pl_socks[i], &buf);
	}

	nbuf_clear(&buf);
	potato = new_Potato(&buf);
	Potato_set_ttl(&potato, n_hop);

	if (n_hop > 0) {
		int pick;
		fd_set rlist;
		int nfds = 0;

		pick = rand() % n_player;
		printf("Sending potato to player %d\n", pick);
		send_buf(pl_socks[pick], &buf);
		for (i = 0; i < n_player; i++) {
			FD_SET(pl_socks[i], &rlist);
			if (pl_socks[i] >= nfds)
				nfds = pl_socks[i] + 1;
		}
		CHECK_SYS(select(nfds, &rlist, NULL, NULL, NULL));
		for (i = 0; i < n_player; i++)
			if (FD_ISSET(pl_socks[i], &rlist))
				break;
		CHECK(i != n_player);
		if (recv_buf(pl_socks[i], &buf) > 0) {
			potato = get_Potato(&buf);
			print_trace(&potato);
		}
	}

	free(pl_socks);
	free(pl_addrs);
	nbuf_free(&buf);
}

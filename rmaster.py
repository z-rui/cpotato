#!/usr/bin/python3

import random
import select
import socket
import sys

from potato_nb import Address, PlayerInfo, Potato
import util

def copy_address(dst, src):
    dst.set_host(src.host())
    dst.set_port(src.port())

def print_trace(trace):
    ids = (entry.id() for entry in trace)
    print("Trace:", *ids)

def main(argv):
    if len(argv) < 3:
        print("need 2 arguments")
        return 1

    ln_port = int(argv[1])
    n_player = int(argv[2])
    n_hop = int(argv[3])

    assert n_player > 0
    assert n_hop >= 0

    ln_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    ln_sock.bind(('', ln_port))
    ln_sock.listen()

    pl_socks = []
    pl_addrs = []

    for i in range(n_player):
        pl_sock, pl_addr = ln_sock.accept()
        addr = util.recv(pl_sock, Address)
        addr.set_host(pl_addr[0])
        pl_socks.append(pl_sock)
        pl_addrs.append(addr)

    for i in range(n_player):
        info = PlayerInfo.new()
        info.set_id(i)
        info.set_total(n_player)
        j = i-1 if i > 0 else n_player - 1
        k = i+1 if i < n_player - 1 else 0
        copy_address(info.init_prev(), pl_addrs[j])
        copy_address(info.init_next(), pl_addrs[k])
        util.send(pl_socks[i], info)

    potato = Potato.new()
    potato.set_ttl(n_hop)

    if n_hop > 0:
        pick = random.randrange(0, n_player)
        print("Sending potato to player", pick)
        util.send(pl_socks[pick], potato)
        rlist, wlist, xlist = select.select(pl_socks, [], [])
        for sock in rlist:
            potato = util.recv(sock, Potato)
            if potato is not None:
                print_trace(potato.trace())
    return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv))

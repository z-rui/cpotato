#!/usr/bin/python3

import random
import select
import socket
import sys

from potato_nb import Address, PlayerInfo, Potato
import util

def append_trace(potato, self_id):
    potato1 = Potato.new()
    potato1.set_ttl(potato.ttl() - 1)
    trace = potato.trace()
    trace1 = potato1.init_trace(len(trace) + 1)
    for i, entry in enumerate(trace):
        trace1[i].set_id(entry.id())
    trace1[len(trace)].set_id(self_id)
    return potato1

def main(argv):
    if len(argv) < 3:
        print("need 2 arguments")
        return 1

    # ringmaster socket
    host, port = (sys.argv[1], int(sys.argv[2]))
    rm_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    rm_sock.connect((host, port))

    # listening socket
    ln_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    ln_sock.bind(('', 0))
    ln_sock.listen()

    ln_addr = Address.new()
    host, port = ln_sock.getsockname()
    ln_addr.set_host(host)
    ln_addr.set_port(port)

    util.send(rm_sock, ln_addr)

    info = util.recv(rm_sock, PlayerInfo)
    n_player = info.total()
    self_id = info.id()

    # connecting socket
    cn_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    cn_sock.connect((info.next().host(), info.next().port()))

    # incoming connection
    in_conn, in_addr = ln_sock.accept()

    while True:
        rlist, wlist, xlist = select.select([rm_sock, cn_sock, in_conn], [], [])
        for sock in rlist:
            potato = util.recv(sock, Potato)
            if potato is None:  # shutdown?
                return 0
            assert potato.ttl() > 0
            potato1 = append_trace(potato, self_id)
            if potato1.ttl() == 0:
                print("I'm it.")
                pick_fd = rm_sock
            else:
                pick_dir = 2*random.randrange(0, 2) - 1
                pick = (self_id + pick_dir + n_player) % n_player
                print("Sending potato to player", pick)
                pick_fd = cn_sock if pick_dir > 0 else in_conn
            util.send(pick_fd, potato1)
            if pick_fd == rm_sock:
                return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv))

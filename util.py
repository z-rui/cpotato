import socket
import struct

def send(sock, msg):
    view = memoryview(msg._buf)
    n = len(view)
    assert n >= 8
    sock.sendall(struct.pack('<I', n))
    sock.sendall(view[4:])

def recv(sock, cls):
    buf = sock.recv(4, socket.MSG_WAITALL)
    if not buf:
        return None
    (n,) = struct.unpack('<I', buf)
    assert n >= 8
    buf = bytearray(n)
    view = memoryview(buf)
    struct.pack_into('<I', view, 0, 1)
    sock.recv_into(view[4:], 0, socket.MSG_WAITALL)
    return cls.from_buf(buf)
